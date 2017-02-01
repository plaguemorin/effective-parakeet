

#include <RtpRtcp/Session.h>
#include "RtpVideoSink.h"

namespace rtp {
    Session::Session()
        : housekeeping(std::thread(&Session::HousekeepingOnThread, this)),
          payloadRegistry(std::make_shared<packetization::PayloadRegistry>()) {

    }

    Session::~Session() {
      /* Wait for the thread to finish */
      if (housekeeping.joinable()) {
        housekeeping.join();
      }
    }

    std::weak_ptr<SinkStream> Session::CreateSinkStream(const Stream::Config &config) {
      if (config.rtp.ssrc == 0) {
        return std::weak_ptr<SinkStream>();
      }

      std::lock_guard<std::mutex> lock_guard(sink_ssrc_lock);
      // If this ssrc was in the seen section, remove it as it's known now
      auto seen = seen_sinks.find(config.rtp.ssrc);
      if (seen != seen_sinks.end()) {
        seen_sinks.erase(seen);
      }

      switch (config.type) {
        case Stream::ANY:break;
        case Stream::AUDIO:break;
        case Stream::VIDEO: {
          auto videoSink = new internal::RtpVideoSink(config, payloadRegistry);

          sink_streams.emplace(config.rtp.ssrc, std::shared_ptr<internal::RtpVideoSink>(videoSink));
        }
          break;
        case Stream::DATA:break;
      }


      return findSinkStream(config.rtp.ssrc);
    }

    std::weak_ptr<Stream> Session::CreateSourceStream(const Stream::Config &config) {
      return std::weak_ptr<Stream>();
    }

    bool Session::ParseRtcpPacket(const std::vector<uint8_t> &&raw_data) {
      return false;
    }

    bool Session::ParseRtpPacket(const std::vector<uint8_t> &&raw_data) {
      if (raw_data.size() < RtpPacket::kMinimalRtpPacketSize) {
        // TODO: Error, packet is too darn small!
        return false;
      }

      RtpPacket packet;
      if (!packet.ReadPacket(std::move(raw_data))) {
        // Unable to parse packet
        return false;
      }

      auto stream = findSinkStream(packet.SSRC());
      if (!stream) {
        ProcessUnknownSsrc(std::move(packet));
        return true;
      }

      return stream->ProcessRtpPacket(std::move(packet));
    }

    void Session::TransportStateChanged(rtp::transport::State state) {

    }

    void Session::HousekeepingOnThread() {

    }

    std::shared_ptr<SinkStream> Session::findSinkStream(const uint32_t ssrc) const {
      auto item = sink_streams.find(ssrc);
      if (item != sink_streams.end()) {
        return item->second;
      }

      return nullptr;
    }

    void Session::ProcessUnknownSsrc(const RtpPacket &&packet) {
      std::lock_guard<std::mutex> lock_guard(sink_ssrc_lock);
      auto item = seen_sinks.find(packet.SSRC());
      if (item == seen_sinks.end()) {
        seen_sinks.insert(packet.SSRC());

        if (unknownSsrc) {
          std::thread dispatch(unknownSsrc, packet.SSRC());
          dispatch.detach();
        }
      }
    }

    bool Session::RegisterRtpPayload(uint8_t payloadType, std::shared_ptr<packetization::PayloadTypeFactory> factory) {
      return payloadRegistry->RegisterFactory(payloadType, factory);
    }

    void Session::ReportEncodedFrameResult(uint32_t streamSsrc, uint32_t frameId, bool result) {
      auto stream = findSinkStream(streamSsrc);
      if (stream) {
        stream->ReportEncodedFrameResult(frameId, result);
      }
    }
}