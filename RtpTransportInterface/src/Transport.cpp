#include <RtpTransport/Transport.h>

#include <cstdio>

namespace {
    constexpr auto kRtpVersion = 2;
}

namespace rtptransport {
    Transport::Transport() : state(kTransportNotReady) {

    }

    void Transport::OnData(const std::vector<uint8_t> &&data) const {
      // Figure out if we're dealing with RTP or RTCP
      auto common_header = data[0];

      // The version is common to every type
      const auto version = common_header >> 6;
      if (version != kRtpVersion) {
        // This isn't anything we understand
        // Discard this packet
        return;
      }

      // Decode RTP payload type
      const auto rtp_payload_type = (uint8_t) (common_header & 0x7F);

      // RTP payload size 72 -> 76 are actually RTPC
      if (rtp_payload_type >= 72 && rtp_payload_type <= 76) {
        OnRtcpData(std::move(data));
      } else {
        OnRtpData(std::move(data));
      }
    }

    void Transport::OnRtpData(const std::vector<uint8_t> &&data) const {
      if (sink_rtp) {
        sink_rtp(std::move(data));
      }
    }

    void Transport::OnRtcpData(const std::vector<uint8_t> &&data) const {
      if (sink_rtcp) {
        sink_rtcp(std::move(data));
      }
    }

    void Transport::SetNewState(const State new_state) {
      const auto state_has_changed = new_state != state;

      state = new_state;

      if (state_changed && state_has_changed) {
        state_changed(new_state);
      }
    }

    void Transport::ReportError(const std::error_code &error_code) {
      // TODO: do something with the error
      fprintf(stderr, "Socket Error: %d, %s\n", error_code.value(), error_code.message().c_str());
      SetNewState(kTransportClosed);
    }
}

