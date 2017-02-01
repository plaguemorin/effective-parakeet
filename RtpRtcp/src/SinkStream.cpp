
#include <RtpRtcp/SinkStream.h>

namespace rtp {

    SinkStream::SinkStream(const Stream::Config &config, std::shared_ptr<packetization::PayloadRegistry> registry)
        : Stream(config), payloadRegistry(registry) {

    }

    SinkStream::~SinkStream() {
    }

    bool SinkStream::ProcessRtpPacket(const RtpPacket &&packet) {
      // Normally, we should check that our SSRC is the same.
      // SSRCs of streams can change (and must be reflected in the RTCP)
      // sinks can track the new SSRC by using the CNAME which is unique


      // Check if the sequence number is in order or not
      const auto inOrder = last_received_sequence_number + 1 == packet.SequenceNumber() || !SeenOnePacket();

      // Create our container
      packetization::PacketizedData data;
      data.lastKeyframeRtpTimestamp = LastKeyframeRtpTimestamp();
      data.localTimestamp = std::chrono::system_clock::now();
      data.inOrder = inOrder;
      data.marker = packet.MarkerFlag();
      data.newFrame = isPacketNewFrame(packet);
      data.rtpPayloadType = packet.PayloadType();
      data.rtpTimestamp = packet.TimeStamp();
      data.payloadDataLength = packet.PayloadDataLength();

      ReportPacketizedData(data);

      // We need to check if our payload changed
      const auto isSamePayload = packet.PayloadType() == LastRtpPayloadType() || !SeenOnePacket();
      if (!isSamePayload) {
        // Since payload type isn't the same
        // We'll need to change our de-packetizer
        // And inform the sink that the content payload has changed

        packetizedDataSink = std::move(payloadRegistry->CreateEncodedFramesSink(packet.PayloadType()));
      }

      // Make sure we have a de-packetizer
      if (!packetizedDataSink) {
        // Not having a depacketizer is the end of the world
        return false;
      }

      // Send all this to the depacketizer
      auto response = packetizedDataSink->ProcessPacketizedData(data, packet.Payload());

      // If we had a completed encoded frame, dispatch it here, even if we didn't succeed
      if (response.frame_complete) {
        EncodedFrameReceived(std::move(packetizedDataSink->GetCompletedFrame()));
      }

      // Early exit if the depacketization didn't work
      if (!response.success) {
        // Don't update stats if the process failed
        return false;
      }

      // Update the stats before returning success
      // This needs to happen in order
      std::lock_guard<std::mutex> lock_guard(criticalSectionRtpReceiver);
      if (inOrder) {
        last_received_sequence_number = packet.SequenceNumber();
      }

      return true;
    }

    bool SinkStream::isPacketNewFrame(const RtpPacket &packet) {
      std::lock_guard<std::mutex> lock_guard(criticalSectionRtpReceiver);
      if (!SeenOnePacket()) {
        return true;
      }

      return last_received_sequence_number + 1 == packet.SequenceNumber()
             && LastRtpTimestamp() != packet.TimeStamp();
    }

    void SinkStream::EncodedFrameReceived(std::unique_ptr<packetization::EncodedFrame> &&frame) {
      // Enforce RTP type
      frame->rtpPayloadType = LastRtpPayloadType();

      // Report it for stats
      ReportEncodedFrame(*frame.get());

      // Make sure we have a sink to send it to...
      // otherwise we discard the data, what a waste
      if (completeEncodedFrameSink) {
        completeEncodedFrameSink(std::move(frame));
      }
    }
}
