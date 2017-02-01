

#ifndef EFFECTIVE_PARAKEET_RTPVIDEOSINK_H
#define EFFECTIVE_PARAKEET_RTPVIDEOSINK_H

#include <memory>
#include <chrono>
#include <mutex>

#include <RtpRtcp/PayloadRegistry.h>
#include <RtpRtcp/SinkStream.h>

namespace rtp {
    namespace internal {
        class RtpVideoSink : public SinkStream {
        public:
            RtpVideoSink(const Config &config, std::shared_ptr<PayloadRegistry> registry);

            virtual ~RtpVideoSink();



        private:

        };
    }
}

#endif //EFFECTIVE_PARAKEET_RTPVIDEOSINK_H
