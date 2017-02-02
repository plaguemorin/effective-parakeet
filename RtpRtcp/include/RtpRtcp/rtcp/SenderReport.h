

#ifndef RTPRTCP_SENDERREPORT_H
#define RTPRTCP_SENDERREPORT_H

#include <RtpRtcp/rtcp/Payload.h>

namespace rtp {
    namespace rtcp {
        class SenderReport : public Payload {
        public:
            bool ReadPacket(const rtp::RtcpPacket *basePacket, const std::vector<uint8_t> &raw_data) override;

        private:
            uint32_t ssrc_of_sender;
            uint64_t ntp_timestamp;
            uint32_t rtp_timestamp;
            uint32_t senders_packet_count;
            uint32_t senders_octet_count;
        };
    }
}

#endif
