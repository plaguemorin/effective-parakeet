#ifndef EFFECTIVE_PARAKEET_SINKSTREAM_H
#define EFFECTIVE_PARAKEET_SINKSTREAM_H

#include <mutex>
#include <functional>
#include <memory>


#include <RtpRtcp/Stream.h>
#include <RtpPacketization/PayloadRegistry.h>

namespace rtp {
    class SinkStream : public Stream {
    public:
        using CompleteEncodedFrame = std::function<void(std::unique_ptr<packetization::EncodedFrame> &&)>;

        SinkStream(const Stream::Config &config, std::shared_ptr<packetization::PayloadRegistry> registry);

        virtual ~SinkStream();

        bool ProcessRtpPacket(const RtpPacket &&packet);

        void CompleteEncodedFrameCallback(CompleteEncodedFrame callback) { completeEncodedFrameSink = callback; }

    protected:
        bool isPacketNewFrame(const RtpPacket &packet);

        void EncodedFrameReceived(std::unique_ptr<packetization::EncodedFrame> &&);

    private:
        CompleteEncodedFrame completeEncodedFrameSink;

        std::shared_ptr<packetization::PayloadRegistry> payloadRegistry;
        std::unique_ptr<packetization::RtpDataReceiver> packetizedDataSink;

        std::mutex criticalSectionRtpReceiver;
        uint16_t last_received_sequence_number = 0;
    };

}

#endif //EFFECTIVE_PARAKEET_SINKSTREAM_H
