
#include "RtpRtcp/RtpPacket.h"
#include "BufferNetworkHelper.h"

namespace {

}

namespace rtp {

    bool RtpPacket::ReadPacket(const std::vector<uint8_t> &&raw_data) {
      auto p = raw_data.begin();
      // The first byte contains version (2 bits), padding (1 bit), extensions (1 bit) and CSRC count (4 bits)
      // Before anything else, verify we have V=2 (mask: 0b1100 0000 or 0xC0, shift right 6)
      if ((*p & 0xC0) == 0) {
        // TODO: Wrong RTP version or not an RTP packet
        return false;
      }

      version = *p >> 6;
      padding = (*p & 0x20) > 0;
      extensions = (0x10) > 0;
      auto local_cc = (uint8_t) (*p & 0xF);
      ++p;

      marker = (*p & 0x80) > 0;
      payload_type = (uint8_t) (*p & 0x7F);
      ++p;

      /* 16-bit Sequence Number */
      SetSequenceNumber(FromNetworkOrderRead<uint16_t>(&(*p)));
      p += 2;
      /* 32-bit Timestamp */
      SetTimeStamp(FromNetworkOrderRead<uint32_t>(&(*p)));
      p += 4;

      /* 32-bit SSRC */
      SetSSRC(FromNetworkOrderRead<uint32_t>(&(*p)));
      p += 4;

      /* Read CSRCs */
      cc = 0;
      for (auto i = 0; i < local_cc; i++) {
        AddCSRC(FromNetworkOrderRead<uint32_t>(&(*p)));
        p += 4;
      }

      /* If we have extensions, read but skip them for now */
      if (extensions) {
        auto extension_profile = FromNetworkOrderRead<uint16_t>(&(*p));
        p += 2;
        auto extension_num_words = FromNetworkOrderRead<uint16_t>(&(*p));
        const size_t length = (size_t) (extension_num_words * 4);
        p += 2;

        p += length;
      }

      /* If there's padding, the last byte of the packet contains the number of byte used as padding */
      if (padding) {
        padding_size = raw_data.back();
        /* If there's padding, it MUST be more than 0 */
        if (padding_size == 0) {
          return false;
        }
      }

      /* There's there's the payload */
      payload = std::vector<uint8_t>(p, raw_data.end());

      return true;
    }
}
