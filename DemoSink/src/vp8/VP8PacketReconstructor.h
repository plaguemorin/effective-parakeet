

#ifndef EFFECTIVE_PARAKEET_VP8PACKETRECONSTRUCTOR_H
#define EFFECTIVE_PARAKEET_VP8PACKETRECONSTRUCTOR_H


#include <RtpRtcp/PayloadRegistry.h>

class VP8PacketReconstructor : public rtp::PacketizedRtpDataReceiver {
public:
    VP8PacketReconstructor(uint8_t pt);

    virtual ~VP8PacketReconstructor();

    Response ProcessPacketizedData(const rtp::PacketizedData &data, const std::vector<uint8_t> &payload) override;

    std::unique_ptr<rtp::EncodedFrame> GetCompletedFrame() override;

private:
    class VP8EncodedImage : public rtp::EncodedVideoFrame {

    };

    std::unique_ptr<VP8EncodedImage> next_frame_;
    int last_width = -1;
    int last_height = -1;
    bool seen_one_keyframe = false;
};

#endif //EFFECTIVE_PARAKEET_VP8PACKETRECONSTRUCTOR_H