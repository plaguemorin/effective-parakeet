

#ifndef RTPRTCP_STREAM_H
#define RTPRTCP_STREAM_H

#include <cstdint>
#include <vector>
#include <string>

#include <RtpRtcp/Extension.h>
#include <RtpRtcp/RtpPacket.h>

namespace rtp {
    /**
     *
     */
    class Stream {
    public:
        enum MediaType : uint8_t {
            ANY,
            AUDIO,
            VIDEO,
            DATA
        };

        struct Config {
            struct Rtp {
                // Sender SSRC.
                uint32_t ssrc = 0;

                // RTP header extensions used for the sent stream.
                std::vector<Extension> extensions;

                // RTCP CNAME, see RFC 3550.
                std::string c_name;
            } rtp;

            MediaType type;
        };

        Stream(const Config &config);

        virtual ~Stream();

        virtual bool ProcessRtpPacket(const RtpPacket &&packet) = 0;


    };
}

#endif
