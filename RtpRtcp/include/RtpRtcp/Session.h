#ifndef RTPRTCP_SESSION_H
#define RTPRTCP_SESSION_H

#include <memory>
#include <map>
#include <set>
#include <thread>
#include <vector>
#include <mutex>

#include <RtpRtcp/SinkStream.h>
#include <RtpRtcp/PayloadRegistry.h>
#include <RtpTransport/State.h>

namespace rtp {
    class Session {
    public:
        using SendDataCallback = std::function<bool(const std::vector<uint8_t> &&raw_data)>;
        using SsrcCallback = std::function<void(const uint32_t& ssrc)>;

        Session();

        virtual ~Session();

        /**
         * Creates a stream configured for receiving packets
         * @return the stream
         */
        std::weak_ptr<SinkStream> CreateSinkStream(const Stream::Config &config);

        /**
         * Creates a stream configured for sending packets
         * @return the stream
         */
        std::weak_ptr<Stream> CreateSourceStream(const Stream::Config &config);

        void SendRtpDataCallback(SendDataCallback callback) { sendRtp = callback; }

        void SendRtcpDataCallback(SendDataCallback callback) { sendRtcp = callback; }

        void UnknownSsrcCallback(SsrcCallback callback) { unknownSsrc = callback; }

        void ParseRtcpPacket(const std::vector<uint8_t> &&raw_data);

        void ParseRtpPacket(const std::vector<uint8_t> &&raw_data);

        void TransportStateChanged(rtptransport::State state);

        bool RegisterRtpPayload(uint8_t payloadType, std::shared_ptr<rtp::PayloadTypeFactory> factory);

    private:
        bool active = true;
        std::thread housekeeping;
        std::mutex sink_ssrc_lock;
        std::set<uint32_t> seen_sinks;
        std::map<uint32_t, std::shared_ptr<SinkStream>> sink_streams;
        std::shared_ptr<PayloadRegistry> payloadRegistry;
        SendDataCallback sendRtp;
        SendDataCallback sendRtcp;
        SsrcCallback unknownSsrc;

        void HousekeepingOnThread();

        std::shared_ptr<SinkStream> findSinkStream(const uint32_t ssrc) const;

        void ProcessUnknownSsrc(const RtpPacket &&packet);
    };
}

#endif
