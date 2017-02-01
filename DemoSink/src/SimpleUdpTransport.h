

#ifndef EFFECTIVE_PARAKEET_SIMPLEUDPTRANSPORT_H
#define EFFECTIVE_PARAKEET_SIMPLEUDPTRANSPORT_H

#include <thread>
#include <sys/socket.h>

#include <RtpTransport/Transport.h>

class SimpleUdpTransport : public rtptransport::Transport {
public:
    SimpleUdpTransport();

    bool SendRtp(const std::vector<uint8_t>&& data) override;

    bool SendRtcp(const std::vector<uint8_t>&& data) override;

    std::error_code ListenOn(uint16_t port);

    virtual ~SimpleUdpTransport();

private:
    std::thread recv_thread;
    int fd = -1;

    void SocketThread();

    void ConnectTo(const sockaddr_storage &addr, const socklen_t addr_len);
};


#endif //EFFECTIVE_PARAKEET_SIMPLEUDPTRANSPORT_H
