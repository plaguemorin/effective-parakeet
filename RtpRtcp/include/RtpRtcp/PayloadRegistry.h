

#ifndef EFFECTIVE_PARAKEET_PAYLOADREGISTRY_H
#define EFFECTIVE_PARAKEET_PAYLOADREGISTRY_H

#include <cstdint>
#include <memory>
#include <vector>

namespace rtp {
    class PacketizedRtpDataReceiver;

    class PayloadRegistry {
    public:
        PayloadRegistry();

        ~PayloadRegistry();

        std::unique_ptr<PacketizedRtpDataReceiver> CreateEncodedFramesSink(uint8_t payloadType);
    };

    class PacketizedRtpDataReceiver {
    public:
        virtual bool OnPacketizedData(
            const std::vector<uint8_t> & payload,
            size_t payloadDataLength,
            bool marker,
            uint32_t rtpTimestamp,
            bool newFrame,
            bool inOrder
        ) = 0;
    };
}

#endif //EFFECTIVE_PARAKEET_PAYLOADREGISTRY_H
