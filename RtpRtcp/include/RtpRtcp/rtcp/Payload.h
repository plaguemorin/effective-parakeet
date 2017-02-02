

#ifndef RTPRTCP_RTCPPAYLOAD_H
#define RTPRTCP_RTCPPAYLOAD_H

#include <cstdint>
#include <vector>

namespace rtp {
    class RtcpPacket;

    namespace rtcp {
        class Payload {
        public:
            Payload();

            virtual ~Payload();

            virtual bool ReadPacket(const rtp::RtcpPacket *basePacket, const std::vector<uint8_t> &raw_data);
        };
    }
}

#endif //EFFECTIVE_PARAKEET_RTCPPAYLOAD_H
