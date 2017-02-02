

#ifndef RTPRTCP_RTCPPACKET_H
#define RTPRTCP_RTCPPACKET_H

#include <vector>
#include <cstdint>
#include <cstddef>

#include <RtpRtcp/rtcp/Payload.h>

namespace rtp {
    class RtcpPacket {
    public:
        RtcpPacket();

        ~RtcpPacket();

        bool ReadPacket(const std::vector<uint8_t> &raw_data);

        bool Padding() const { return padding; }

        uint8_t ReportCount() const { return report_count; }

        uint8_t PayloadType() const { return payload_type; }

        size_t Length() const { return length; }

    private:
        uint8_t version = 0;
        bool padding = 0;
        uint8_t report_count = 0;
        uint8_t payload_type = 0;
        size_t length = 0;

        rtcp::Payload *payload = nullptr;
    };
}

#endif
