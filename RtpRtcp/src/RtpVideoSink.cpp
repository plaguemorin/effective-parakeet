

#include "RtpVideoSink.h"

namespace rtp {
    namespace internal {
        RtpVideoSink::RtpVideoSink(const rtp::Stream::Config &config, std::shared_ptr<PayloadRegistry> registry)
            : Stream(config), payloadRegistry(registry) {

        }

        RtpVideoSink::~RtpVideoSink() {

        }

        bool RtpVideoSink::ProcessRtpPacket(const RtpPacket &&packet) {
          // Normally, we should check that our SSRC is the same.
          // SSRCs of streams can change (and must be reflected in the RTCP)
          // sinks can track the new SSRC by using the CNAME which is unique

          // We need to check if our payload changed
          const auto isSamePayload = isPayloadTypeEqual(packet);
          if (!isSamePayload) {
            // Since payload type isn't the same
            // We'll need to change our de-packetizer
            // And inform the sink that the content payload has changed

            packetizedDataSink = std::move(payloadRegistry->CreateEncodedFramesSink(packet.PayloadType()));
          }

          // Make sure we have a de-packetizer
          if (!packetizedDataSink) {
            return false;
          }

          // Check if the sequence number is in order or not
          const auto inOrder = last_received_sequence_number + 1 == packet.SequenceNumber();

          // Send all this to the depacketizer
          bool ret = packetizedDataSink->OnPacketizedData(
              packet.Payload(), packet.PayloadDataLength(),
              packet.MarkerFlag(),
              packet.TimeStamp(),
              isPacketNewFrame(packet),
              inOrder
          );

          // Don't update stats if the process failed
          if (!ret) {
            return false;
          }

          // Update the stats before returning success
          std::lock_guard<std::mutex> lock_guard(critical_section_rtp_receiver);
          last_received_payload_length = packet.PaddingLength();
          last_received_time = std::chrono::system_clock::now();

          if (inOrder) {
            if (last_received_rtp_timestamp != packet.TimeStamp()) {
              last_received_rtp_timestamp = packet.TimeStamp();
              last_received_frame_time = std::chrono::system_clock::now();
            }
            last_received_sequence_number = packet.SequenceNumber();
          }

          return true;
        }

        bool RtpVideoSink::isPayloadTypeEqual(const RtpPacket &packet) {
          std::lock_guard<std::mutex> lock_guard(critical_section_rtp_receiver);
          return !(processed_one_packet && packet.PayloadType() != last_received_rtp_payload);
        }

        bool RtpVideoSink::isPacketNewFrame(const RtpPacket &packet) {
          std::lock_guard<std::mutex> lock_guard(critical_section_rtp_receiver);
          if (!processed_one_packet) {
            return true;
          }

          return last_received_sequence_number + 1 == packet.SequenceNumber()
                 && last_received_rtp_timestamp != packet.TimeStamp();
        }

    }
}
