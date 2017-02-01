
#include <RtpPacketization/PayloadRegistry.h>

namespace rtp {
    namespace packetization {
        PayloadRegistry::PayloadRegistry() {

        }

        PayloadRegistry::~PayloadRegistry() {

        }

        std::unique_ptr<StreamReconstructor> PayloadRegistry::CreateEncodedFramesSink(uint8_t payloadType) {
          auto item = factories.find(payloadType);
          if (item != factories.end()) {
            return std::move(item->second->CreateSink(payloadType));
          }
          return nullptr;
        }

        bool PayloadRegistry::RegisterFactory(uint8_t payloadType, std::shared_ptr<PayloadTypeFactory> factory) {
          factories.emplace(payloadType, factory);

          return true;
        }

        std::weak_ptr<PayloadTypeFactory> PayloadRegistry::GetFactory(uint8_t payload) const {
          auto item = factories.find(payload);
          if (item != factories.end()) {
            return item->second;
          }

          return std::weak_ptr<PayloadTypeFactory>();
        }

    }
}

