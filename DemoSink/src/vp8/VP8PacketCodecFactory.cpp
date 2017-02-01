

#include "VP8PacketCodecFactory.h"
#include "VP8PacketReconstructor.h"

std::unique_ptr<rtp::packetization::RtpDataReceiver> VP8PacketCodecFactory::CreateSink(uint8_t payloadType) {
  return std::unique_ptr<VP8PacketReconstructor>(new VP8PacketReconstructor(payloadType));
}
