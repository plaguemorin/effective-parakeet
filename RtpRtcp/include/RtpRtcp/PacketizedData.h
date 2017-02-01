

#ifndef EFFECTIVE_PARAKEET_PACKETIZEDDATA_H
#define EFFECTIVE_PARAKEET_PACKETIZEDDATA_H

#include <cstdint>
#include <cstddef>
#include <chrono>

namespace rtp {
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

#endif //EFFECTIVE_PARAKEET_PACKETIZEDDATA_H
