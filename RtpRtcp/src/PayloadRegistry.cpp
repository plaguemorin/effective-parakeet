
#include <RtpRtcp/PayloadRegistry.h>

namespace rtp {
    PayloadRegistry::PayloadRegistry() {

    }

    PayloadRegistry::~PayloadRegistry() {

    }

    std::unique_ptr<PacketizedRtpDataReceiver> PayloadRegistry::CreateEncodedFramesSink(uint8_t payloadType) {
      return std::unique_ptr<PacketizedRtpDataReceiver>();
    }
}

