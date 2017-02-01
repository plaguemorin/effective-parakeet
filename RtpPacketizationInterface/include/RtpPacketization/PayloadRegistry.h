

#ifndef RTPPACKETIZATION_PAYLOADREGISTRY_H
#define RTPPACKETIZATION_PAYLOADREGISTRY_H

#include <cstdint>
#include <memory>
#include <map>

#include <RtpPacketization/EncodedFrame.h>
#include <RtpPacketization/PacketizedData.h>
#include <RtpPacketization/PayloadTypeFactory.h>
#include <RtpPacketization/RtpDataReceiver.h>

namespace rtp {
    namespace packetization {
        class PayloadRegistry {
        public:
            PayloadRegistry();

            ~PayloadRegistry();

            std::unique_ptr<RtpDataReceiver> CreateEncodedFramesSink(uint8_t payloadType);

            std::weak_ptr<PayloadTypeFactory> GetFactory(uint8_t payload) const;

            bool RegisterFactory(uint8_t payloadType, std::shared_ptr<PayloadTypeFactory> factory);

        private:
            std::map<uint8_t, std::shared_ptr<PayloadTypeFactory>> factories;
        };
    }
}

#endif
