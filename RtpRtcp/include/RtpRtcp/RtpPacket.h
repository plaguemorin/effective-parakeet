

#ifndef RTPRTCP_RTPPACKET_H
#define RTPRTCP_RTPPACKET_H

#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>

namespace rtp {
    class RtpPacket {
    public:
        static const size_t kMinimalRtpPacketSize = 12;
        static const size_t kMaximumCSRCs = 15;

        RtpPacket() {};

        RtpPacket(const RtpPacket &other) = delete;

        bool PaddingFlag() const { return padding; }

        void SetPaddingFlag(bool b) { padding = b; }

        bool MarkerFlag() const { return marker; }

        void SetMarkerFlag(bool m) { marker = m; }

        uint8_t PayloadType() const { return payload_type; }

        uint16_t SequenceNumber() const { return sequence_number_; }

        void SetSequenceNumber(uint16_t s) { sequence_number_ = s; }

        uint32_t TimeStamp() const { return timestamp_; }

        void SetTimeStamp(uint32_t t) { timestamp_ = t; }

        uint32_t SSRC() const { return ssrc_; }

        void SetSSRC(const uint32_t ssrc) { ssrc_ = ssrc; }

        void AddCSRC(uint32_t csrc) { csrc_[cc++] = csrc; }

        const std::array<uint32_t, kMaximumCSRCs> &CSRC() const { return csrc_; }

        void SetExtensionFlag(bool i) { extensions = i; }

        ssize_t CsrcCount() { return cc; }

        size_t PayloadLength() const { return payload.size(); }

        size_t PaddingLength() const { return padding_size; }

        size_t PayloadDataLength() const { return payload.size() - padding ? padding_size : 0; }

        const std::vector<uint8_t> &Payload() const { return payload; }

        bool ReadPacket(const std::vector<uint8_t> &&raw_data);

    private:
        uint8_t version = 0;
        bool padding = false;
        bool extensions = false;
        uint8_t cc = 0;
        bool marker = false;
        uint8_t payload_type = 0;
        uint16_t sequence_number_ = 0;
        uint32_t timestamp_ = 0;
        uint32_t ssrc_ = 0;
        std::array<uint32_t, kMaximumCSRCs> csrc_ = {{0}};

        std::vector<uint8_t> payload;
        uint8_t padding_size = 0;
    };
}

#endif