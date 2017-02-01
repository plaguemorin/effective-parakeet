

#ifndef RTPPACKETIZATION_PACKETIZEDDATA_H
#define RTPPACKETIZATION_PACKETIZEDDATA_H

#include <cstdint>
#include <cstddef>
#include <chrono>

namespace rtp {
    namespace packetization {
        struct PayloadDescriptor {
            size_t payloadDataLength;
            uint32_t rtpTimestamp;
            uint8_t rtpPayloadType;
            bool marker;
            bool newFrame;
            bool inOrder;

            std::chrono::system_clock::time_point localTimestamp;
            uint32_t ssrc;
            uint32_t frameId;
            uint16_t packetDelta;
        };
    }
}

#endif
