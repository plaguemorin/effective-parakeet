

#ifndef RTPPACKETIZATION_PAYLOADTYPEFACTORY_H
#define RTPPACKETIZATION_PAYLOADTYPEFACTORY_H

#include <cstdint>
#include <memory>

#include <RtpPacketization/RtpDataReceiver.h>

namespace rtp {
    namespace packetization {
        class PayloadTypeFactory {
        public:
            virtual std::unique_ptr<RtpDataReceiver> CreateSink(uint8_t payloadType) = 0;
        };

    }
}
#endif
