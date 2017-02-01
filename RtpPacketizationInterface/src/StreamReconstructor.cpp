
#include <RtpPacketization/StreamReconstructor.h>

namespace rtp {
    namespace packetization {
        StreamReconstructor::StreamReconstructor(uint8_t payloadType)
            : payloadType(payloadType) {

        }

        std::unique_ptr<EncodedFrame> StreamReconstructor::GetNextCompleteFrame() {
          std::lock_guard<std::mutex> lock_guard(dequeueCriticalSection);
          if (completedFrames.empty()) {
            return nullptr;
          }
          auto retval = std::move(completedFrames.front());
          completedFrames.pop_front();
          return retval;
        }

        void StreamReconstructor::QueueCompletedFrame(std::unique_ptr<EncodedFrame> &&frame) {
          completedFrames.emplace_back(std::move(frame));
        }

        bool StreamReconstructor::HasCompleteFramesQueued() const {
          return !completedFrames.empty();
        }
    }
}

