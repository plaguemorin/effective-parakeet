

#ifndef RTPPACKETIZATION_RTPDATARECEIVER_H
#define RTPPACKETIZATION_RTPDATARECEIVER_H

#include <cstdint>
#include <vector>
#include <deque>
#include <memory>
#include <mutex>

#include <RtpPacketization/PayloadDescriptor.h>
#include <RtpPacketization/EncodedFrame.h>

namespace rtp {
    namespace packetization {
        class StreamReconstructor {
        public:
            virtual bool ProcessPacketizedData(const PayloadDescriptor &data, const std::vector<uint8_t> &payload) = 0;

            std::unique_ptr<EncodedFrame> GetNextCompleteFrame();

            bool HasCompleteFramesQueued() const;

            uint8_t PayloadType() const { return payloadType; }

        protected:
            StreamReconstructor(uint8_t payloadType);

            void QueueCompletedFrame(std::unique_ptr<EncodedFrame> &&frame);

        private:
            uint8_t payloadType;
            std::mutex dequeueCriticalSection;
            std::deque<std::unique_ptr<EncodedFrame>> completedFrames;
        };
    }
}

#endif
