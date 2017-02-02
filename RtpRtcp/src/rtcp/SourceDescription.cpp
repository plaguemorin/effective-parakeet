
#include <RtpRtcp/rtcp/SourceDescription.h>
#include <RtpRtcp/RtcpPacket.h>

namespace rtp {
    namespace rtcp {
        bool SourceDescription::ReadPacket(const rtp::RtcpPacket *basePacket, const std::vector<uint8_t> &raw_data) {
          return Payload::ReadPacket(basePacket, std::move(raw_data));
        }
    }

}

