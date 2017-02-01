

#ifndef RTPPACKETIZATION_FRAME_H
#define RTPPACKETIZATION_FRAME_H

#include <vector>
#include <cstdint>

namespace rtp {
    namespace packetization {
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
    }
}

#endif
