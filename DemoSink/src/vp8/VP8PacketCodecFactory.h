

#ifndef EFFECTIVE_PARAKEET_VP8PACKETCODECFACTORY_H
#define EFFECTIVE_PARAKEET_VP8PACKETCODECFACTORY_H

#include <memory>
#include <RtpRtcp/PayloadRegistry.h>

class VP8PacketCodecFactory : public rtp::PayloadTypeFactory {
public:
    std::unique_ptr<rtp::PacketizedRtpDataReceiver> CreateSink(uint8_t payloadType) override;
};

#endif //EFFECTIVE_PARAKEET_VP8PACKETCODECFACTORY_H
