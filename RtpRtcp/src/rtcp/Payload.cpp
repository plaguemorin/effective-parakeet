

#include <RtpRtcp/rtcp/Payload.h>
#include <RtpRtcp/RtcpPacket.h>

namespace rtp {
    namespace rtcp {
        Payload::Payload() {

        }

        Payload::~Payload() {

        }

        bool Payload::ReadPacket(const rtp::RtcpPacket *basePacket, const std::vector<uint8_t> &raw_data) {
          return false;
        }

    }
}
