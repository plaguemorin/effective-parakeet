

#ifndef RTPPACKETIZATION_ENCODEDVIDEOFRAME_H
#define RTPPACKETIZATION_ENCODEDVIDEOFRAME_H

#include <RtpPacketization/EncodedFrame.h>

namespace rtp {
    namespace packetization {
        class EncodedVideoFrame : public EncodedFrame {
        public:
            Type FrameType() const { return kVideoFrame; }

            int width;
            int height;
        };
    }
}
#endif
