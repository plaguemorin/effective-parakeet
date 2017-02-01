

#include "RtpVideoSink.h"

namespace rtp {
    namespace internal {
        RtpVideoSink::RtpVideoSink(const rtp::Stream::Config &config, std::shared_ptr<packetization::PayloadRegistry> registry)
            : SinkStream(config, registry) {

        }

        RtpVideoSink::~RtpVideoSink() {

        }


    }
}
