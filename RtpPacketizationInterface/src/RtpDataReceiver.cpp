
#include <RtpPacketization/RtpDataReceiver.h>

namespace rtp {
    namespace packetization {
        RtpDataReceiver::Response::Response(bool simpleResponse)
            : frame_complete(false), success(simpleResponse) {

        }


        RtpDataReceiver::Response::Response(bool success, bool completedFrame) :
            frame_complete(completedFrame), success(success) {

        }

        RtpDataReceiver::RtpDataReceiver(uint8_t payloadType)
            : payloadType(payloadType) {

        }
    }
}

