
#include <cstdio>

#include <RtpRtcp/rtcp/SenderReport.h>
#include <RtpRtcp/RtcpPacket.h>
#include <RtpRtcp/rtcp/SourceDescription.h>

#include "BufferNetworkHelper.h"

namespace rtp {

    RtcpPacket::RtcpPacket() {

    }

    RtcpPacket::~RtcpPacket() {
      if (payload) {
        delete payload;
      }
    }

    bool RtcpPacket::ReadPacket(const std::vector<uint8_t> &raw_data) {
      auto p = raw_data.data();

      version = *p >> 6;
      padding = (*p & 0b00100000) > 0;
      report_count = static_cast<uint8_t>(*p & 0b11111);
      p++;

      payload_type = *p;
      p++;

      length = FromNetworkOrderRead<uint16_t>(p);
      p += 2;

      switch (payload_type) {
        case 200: /* Sender Report */
          payload = new rtcp::SenderReport();
          break;

        case 202: /* Source Description RTCP Packet (SDES) */
          payload = new rtcp::SourceDescription();
          break;
      }

      return payload ? payload->ReadPacket(this, raw_data) : false;
    }
}
