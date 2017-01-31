

#ifndef EFFECTIVE_PARAKEET_RTPVIDEOSINK_H
#define EFFECTIVE_PARAKEET_RTPVIDEOSINK_H

#include <memory>
#include <chrono>
#include <mutex>

#include <RtpRtcp/PayloadRegistry.h>
#include <RtpRtcp/Stream.h>

namespace rtp {
    namespace internal {
        class RtpVideoSink : public Stream {
        public:
            RtpVideoSink(const Config &config, std::shared_ptr<PayloadRegistry> registry);

            virtual ~RtpVideoSink();

            bool ProcessRtpPacket(const RtpPacket &&packet) override;

        protected:
            bool isPayloadTypeEqual(const RtpPacket& packet);

            bool isPacketNewFrame(const RtpPacket &packet);

        private:
            std::shared_ptr<PayloadRegistry> payloadRegistry;
            std::unique_ptr<PacketizedRtpDataReceiver> packetizedDataSink;

            std::mutex critical_section_rtp_receiver;
            bool processed_one_packet = false;
            std::chrono::system_clock::time_point last_received_frame_time;
            std::chrono::system_clock::time_point last_received_time;
            uint16_t last_received_sequence_number = 0;
            uint32_t last_received_rtp_timestamp = 0;
            uint8_t last_received_rtp_payload = 0;
            size_t last_received_payload_length = 0;
        };
    }
}



#endif //EFFECTIVE_PARAKEET_RTPVIDEOSINK_H
