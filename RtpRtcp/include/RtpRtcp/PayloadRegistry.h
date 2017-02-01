

#ifndef EFFECTIVE_PARAKEET_PAYLOADREGISTRY_H
#define EFFECTIVE_PARAKEET_PAYLOADREGISTRY_H

#include <cstdint>
#include <memory>
#include <vector>
#include <functional>
#include <chrono>
#include <map>

#include <RtpRtcp/Frame.h>
#include <RtpRtcp/PacketizedData.h>

namespace rtp {
    class PacketizedRtpDataReceiver;

    class PayloadTypeFactory {
    public:
        virtual std::unique_ptr<PacketizedRtpDataReceiver> CreateSink(uint8_t payloadType) = 0;
    };

    class PayloadRegistry {
    public:
        PayloadRegistry();

        ~PayloadRegistry();

        std::unique_ptr<PacketizedRtpDataReceiver> CreateEncodedFramesSink(uint8_t payloadType);

        std::weak_ptr<PayloadTypeFactory> GetFactory(uint8_t payload) const;

        bool RegisterFactory(uint8_t payloadType, std::shared_ptr<PayloadTypeFactory> factory);

    private:
        std::map<uint8_t, std::shared_ptr<PayloadTypeFactory>> factories;
    };


    class PacketizedRtpDataReceiver {
    public:
        PacketizedRtpDataReceiver(uint8_t payloadType);

        struct Response {
            explicit Response(bool simpleResponse);

            explicit Response(bool success, bool completedFrame);

            bool success;
            bool frame_complete;
        };

        virtual Response ProcessPacketizedData(const PacketizedData &data, const std::vector<uint8_t> &payload) = 0;

        virtual std::unique_ptr<EncodedFrame> GetCompletedFrame() = 0;

        uint8_t PayloadType() const { return payloadType; }
    private:
        uint8_t payloadType;
    };
}

#endif //EFFECTIVE_PARAKEET_PAYLOADREGISTRY_H
