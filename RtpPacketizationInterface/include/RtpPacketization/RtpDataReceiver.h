

#ifndef RTPPACKETIZATION_RTPDATARECEIVER_H
#define RTPPACKETIZATION_RTPDATARECEIVER_H

#include <cstdint>
#include <vector>
#include <memory>

#include <RtpPacketization/PacketizedData.h>
#include <RtpPacketization/EncodedFrame.h>

namespace rtp {
    namespace packetization {
        class RtpDataReceiver {
        public:
            RtpDataReceiver(uint8_t payloadType);

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
}

#endif
