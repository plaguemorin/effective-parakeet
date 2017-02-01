#ifndef RTPTRASNPORT_TRANSPORT_H
#define RTPTRASNPORT_TRANSPORT_H

#include <cstdint>
#include <functional>
#include <vector>
#include <system_error>

#include <RtpTransport/State.h>

namespace rtp {
    namespace transport {

        /**
     * Transport class provides transport adapters for RTP/RTCP sessions
     */
        class Transport {
        public:
            using OnPacketCallback = std::function<void(const std::vector<uint8_t> &&)>;
            using OnTransportStateChanged = std::function<void( const State)>;

            Transport();

            virtual bool SendRtp(const std::vector<uint8_t> &&) = 0;

            virtual bool SendRtcp(const std::vector<uint8_t> &&) = 0;

            void RtpReceiveCallback(OnPacketCallback callback) { sink_rtp = callback; }

            void RtcpReceiveCallback(OnPacketCallback callback) { sink_rtcp = callback; }

            void StateChangedCallback(OnTransportStateChanged callback) { state_changed = callback; }

            const State CurrentState() const { return state; }

        protected:
            virtual ~Transport() {}

            void OnData(const std::vector<uint8_t> &&) const;

            void OnRtpData(const std::vector<uint8_t> &&) const;

            void OnRtcpData(const std::vector<uint8_t> &&) const;

            void SetNewState(const State new_state);

            void ReportError(const std::error_code &error_code);

        private:
            OnPacketCallback sink_rtp;
            OnPacketCallback sink_rtcp;
            OnTransportStateChanged state_changed;

            State state;
        };
    }
}

#endif
