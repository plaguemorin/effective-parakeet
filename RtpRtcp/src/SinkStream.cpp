
#include <RtpRtcp/SinkStream.h>

namespace rtp {

    SinkStream::SinkStream(const Stream::Config &config, std::shared_ptr<packetization::PayloadRegistry> registry)
        : Stream(config), payloadRegistry(registry) {

    }

    SinkStream::~SinkStream() {
      DestroyCurrentStreamReconstructor();
    }

    bool SinkStream::ProcessRtpPacket(const RtpPacket &&packet) {
      // Normally, we should check that our SSRC is the same.
      // SSRCs of streams can change (and must be reflected in the RTCP)
      // sinks can track the new SSRC by using the CNAME which is unique

      // We need to check if our payload changed to create a packetizedDataSink
      if (!MaybeCreateStreamReconstructorForPacket(packet)) {
        // Not having a depacketizer is the end of the world
        // Notify to make sure we don't get threads that are waiting forever
        DestroyCurrentStreamReconstructor();
        return false;
      }
      // Make sure this happens in order
      std::lock_guard<std::mutex> lock_guard(criticalSectionRtpReceiver);

      // Create our container
      const auto payloadDescriptor = CreateAndReportDescriptor(packet);

      // Update the stats
      if (packet.SequenceNumber() > largestSequenceNumberSeen) {
        largestSequenceNumberSeen = packet.SequenceNumber();
      }

      // check for wraparound
      if (last_received_sequence_number > packet.SequenceNumber()
          && payloadDescriptor.packetDelta <= 5) {
        numSequenceWrappedAround++;
        largestSequenceNumberSeen = packet.SequenceNumber();
      }

      last_received_sequence_number = packet.SequenceNumber();

      // Notify the encoded frame thread to wake up, but since we hold the lock to
      // criticalSectionRtpReceiver, it will block right away until we exit this scope
      encodedFramesAvailable.notify_all();

      // Send all this to the depacketizer
      return packetizedDataSink->ProcessPacketizedData(payloadDescriptor, packet.Payload());
    }

    packetization::PayloadDescriptor SinkStream::CreateAndReportDescriptor(const RtpPacket &packet) {
      // Check if the sequence number is in order or not
      const auto inOrder = last_received_sequence_number + 1 == packet.SequenceNumber() || !SeenOnePacket();

      packetization::PayloadDescriptor data;
      data.localTimestamp = std::chrono::system_clock::now();
      data.inOrder = inOrder;
      data.marker = packet.MarkerFlag();
      data.newFrame = isPacketNewFrame(packet);
      data.rtpPayloadType = packet.PayloadType();
      data.rtpTimestamp = packet.TimeStamp();
      data.payloadDataLength = packet.PayloadDataLength();

      data.ssrc = SSRC();
      data.frameId = (uint32_t) ((numSequenceWrappedAround << 16) + packet.SequenceNumber());
      data.packetDelta = packet.SequenceNumber() - last_received_sequence_number;

      ReportPacketizedData(data);
      return data;
    }

    bool SinkStream::MaybeCreateStreamReconstructorForPacket(const RtpPacket &packet) {
      if (packet.PayloadType() != LastRtpPayloadType() || !SeenOnePacket()) {
        // Since payload type isn't the same
        // We'll need to change our de-packetizer
        // And inform the sink that the content payload has changed
        DestroyCurrentStreamReconstructor();

        // Create the new sink
        packetizedDataSink = move(payloadRegistry->CreateEncodedFramesSink(packet.PayloadType()));

        // Create drain thread
        encodedFramesDrainThread = std::thread(&SinkStream::ProcessEncodedFramesOnThread, this);
      }

      return (bool) packetizedDataSink;
    }

    void SinkStream::DestroyCurrentStreamReconstructor() {
      packetizedDataSink = nullptr;

      // Cleanup previous drain thread
      if (encodedFramesDrainThread.joinable()) {
        encodedFramesAvailable.notify_all();
        encodedFramesDrainThread.join();
      }
    }

    bool SinkStream::isPacketNewFrame(const RtpPacket &packet) {
      if (!SeenOnePacket()) {
        return true;
      }

      return last_received_sequence_number + 1 == packet.SequenceNumber()
             && LastRtpTimestamp() != packet.TimeStamp();
    }

    void SinkStream::ProcessEncodedFramesOnThread() {
      while (packetizedDataSink) {
        std::unique_lock<std::mutex> criticalSection(criticalSectionRtpReceiver);
        encodedFramesAvailable.wait(criticalSection);
        criticalSection.unlock();

        while (packetizedDataSink && packetizedDataSink->HasCompleteFramesQueued()) {
          auto frame = packetizedDataSink->GetNextCompleteFrame();

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
    }

    void SinkStream::ReportEncodedFrameResult(uint32_t frameId, bool result) {

    }
}
