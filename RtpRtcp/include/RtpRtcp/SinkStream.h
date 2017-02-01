#ifndef EFFECTIVE_PARAKEET_SINKSTREAM_H
#define EFFECTIVE_PARAKEET_SINKSTREAM_H

#include <mutex>
#include <functional>
#include <memory>
#include <condition_variable>
#include <thread>

#include <RtpRtcp/Stream.h>
#include <RtpPacketization/PayloadRegistry.h>

namespace rtp {
    class SinkStream : public Stream {
    public:
        using CompleteEncodedFrame = std::function<void(std::unique_ptr<packetization::EncodedFrame> &&)>;

        virtual ~SinkStream();

        bool ProcessRtpPacket(const RtpPacket &&packet);

        void ReportEncodedFrameResult(uint32_t frameId, bool result);

        void CompleteEncodedFrameCallback(CompleteEncodedFrame callback) { completeEncodedFrameSink = callback; }

    protected:
        SinkStream(const Stream::Config &config, std::shared_ptr<packetization::PayloadRegistry> registry);

        bool isPacketNewFrame(const RtpPacket &packet);

    private:
        CompleteEncodedFrame completeEncodedFrameSink;

        const std::shared_ptr<packetization::PayloadRegistry> payloadRegistry;
        std::unique_ptr<packetization::StreamReconstructor> packetizedDataSink;

        std::mutex criticalSectionRtpReceiver;
        uint16_t last_received_sequence_number = 0;
        uint16_t largestSequenceNumberSeen = 0;
        uint16_t numSequenceWrappedAround = 0;

        std::thread encodedFramesDrainThread;
        std::condition_variable encodedFramesAvailable;

        void ProcessEncodedFramesOnThread();

        void DestroyCurrentStreamReconstructor();

        bool MaybeCreateStreamReconstructorForPacket(const RtpPacket &packet);

        packetization::PayloadDescriptor CreateAndReportDescriptor(const RtpPacket &packet);

    };

}

#endif //EFFECTIVE_PARAKEET_SINKSTREAM_H
