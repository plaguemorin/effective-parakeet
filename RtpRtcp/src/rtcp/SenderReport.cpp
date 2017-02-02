

#include <RtpRtcp/rtcp/SenderReport.h>
#include <RtpRtcp/RtcpPacket.h>
#include <BufferNetworkHelper.h>

namespace rtp {
    namespace rtcp {
        bool SenderReport::ReadPacket(const rtp::RtcpPacket *basePacket, const std::vector<uint8_t> &raw_data) {
          auto p = raw_data.data() + 4;

          ssrc_of_sender = FromNetworkOrderRead<uint32_t>(p);
          p += 4;

          ntp_timestamp = FromNetworkOrderRead<uint64_t>(p);
          p += 8;

          rtp_timestamp = FromNetworkOrderRead<uint32_t>(p);
          p += 4;

          senders_packet_count = FromNetworkOrderRead<uint32_t>(p);
          p += 4;

          senders_octet_count = FromNetworkOrderRead<uint32_t>(p);
          p += 4;

          /* Read Report blocks */
          for (auto i = 0; i < basePacket->ReportCount(); i++) {
            /* Read Reports */
          }

          return true;
        }
    }

}