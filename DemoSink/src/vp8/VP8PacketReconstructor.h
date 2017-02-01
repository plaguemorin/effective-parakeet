

#ifndef EFFECTIVE_PARAKEET_VP8PACKETRECONSTRUCTOR_H
#define EFFECTIVE_PARAKEET_VP8PACKETRECONSTRUCTOR_H

#include <RtpPacketization/PayloadRegistry.h>
#include <RtpPacketization/EncodedVideoFrame.h>

class VP8PacketReconstructor : public rtp::packetization::StreamReconstructor {
public:
    VP8PacketReconstructor(uint8_t pt);

    virtual ~VP8PacketReconstructor();

    bool ProcessPacketizedData(const rtp::packetization::PayloadDescriptor &data, const std::vector<uint8_t> &payload) override;

private:
    class VP8EncodedImage : public rtp::packetization::EncodedVideoFrame {

    };

    std::unique_ptr<VP8EncodedImage> next_frame_;
    int last_width = -1;
    int last_height = -1;
    bool seen_one_keyframe = false;
};

#endif //EFFECTIVE_PARAKEET_VP8PACKETRECONSTRUCTOR_H
