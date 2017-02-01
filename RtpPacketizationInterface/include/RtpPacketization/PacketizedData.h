

#ifndef RTPPACKETIZATION_PACKETIZEDDATA_H
#define RTPPACKETIZATION_PACKETIZEDDATA_H

#include <cstdint>
#include <cstddef>
#include <chrono>

namespace rtp {
    namespace packetization {
        struct PacketizedData {
            size_t payloadDataLength;
            uint32_t rtpTimestamp;
            uint8_t rtpPayloadType;
            bool marker;
            bool newFrame;
            bool inOrder;

            std::chrono::system_clock::time_point localTimestamp;
            uint32_t lastKeyframeRtpTimestamp;
        };
    }
}

#endif
