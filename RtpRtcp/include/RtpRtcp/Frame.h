

#ifndef EFFECTIVE_PARAKEET_FRAME_H
#define EFFECTIVE_PARAKEET_FRAME_H

#include <vector>

namespace rtp {
    class EncodedFrame {
    public:
        enum Type {
            kVideoFrame,
            kAudioFrame,
            kDataFrame
        };

        virtual Type FrameType() const = 0;

        uint8_t rtpPayloadType = 0;
        bool keyFrame;
        uint32_t timestamp;

        std::vector<uint8_t> payload;
    };

    class EncodedVideoFrame : public EncodedFrame {
    public:
        Type FrameType() const { return kVideoFrame; }

        int width;
        int height;
    };
}

#endif //EFFECTIVE_PARAKEET_FRAME_H
