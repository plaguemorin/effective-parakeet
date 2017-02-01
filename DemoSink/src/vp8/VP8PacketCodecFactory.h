

#ifndef EFFECTIVE_PARAKEET_VP8PACKETCODECFACTORY_H
#define EFFECTIVE_PARAKEET_VP8PACKETCODECFACTORY_H

#include <memory>
#include <RtpPacketization/PayloadRegistry.h>

class VP8PacketCodecFactory : public rtp::packetization::PayloadTypeFactory {
public:
    std::unique_ptr<rtp::packetization::StreamReconstructor> CreateSink(uint8_t payloadType) override;
};

#endif //EFFECTIVE_PARAKEET_VP8PACKETCODECFACTORY_H
