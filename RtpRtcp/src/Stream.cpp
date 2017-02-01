#include <RtpRtcp/Stream.h>

namespace rtp {
    Stream::Stream(const Stream::Config &config) {

    }

    Stream::~Stream() {

    }

    void Stream::ReportPacketizedData(const PacketizedData &data) {
      std::lock_guard<std::mutex> lock_guard(statsCriticalLock);

      if (data.inOrder) {
        processedOnePacket = true;
        lastRtpPayloadType = data.rtpPayloadType;
        lastPayloadLength = data.payloadDataLength;
        lastRtpTimestamp = data.rtpTimestamp;
        lastPacketizedReportTime = std::chrono::system_clock::now();
      }
    }

    void Stream::ReportEncodedFrame(const EncodedFrame &frame) {
      std::lock_guard<std::mutex> lock_guard(statsCriticalLock);

      if (frame.keyFrame) {
        lastKeyframeRtpTimestamp = frame.timestamp;
        lastKeyFrameLocalTime = std::chrono::system_clock::now();
      }
    }


}