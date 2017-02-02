

#ifndef RTPRTCP_SOURCEDESCRIPTION_H
#define RTPRTCP_SOURCEDESCRIPTION_H

#include <RtpRtcp/rtcp/Payload.h>

namespace rtp {
    namespace rtcp {
        class SourceDescription : public Payload {
        public:
            bool ReadPacket(const rtp::RtcpPacket *basePacket, const std::vector<uint8_t> &raw_data) override;
        };
    }
}

#endif
