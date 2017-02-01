
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>

#include <vector>

#include "SimpleUdpTransport.h"

SimpleUdpTransport::SimpleUdpTransport() {
  fd = socket(AF_INET6, SOCK_DGRAM, 0);
  recv_thread = std::thread(&SimpleUdpTransport::SocketThread, this);
}

SimpleUdpTransport::~SimpleUdpTransport() {
  close(fd);
  fd = -1;

  if (recv_thread.joinable()) {
    recv_thread.join();
  }
}

bool SimpleUdpTransport::SendRtp(const std::vector<uint8_t> &&data) {
  const uint8_t *p = data.data();
  size_t len = data.size();

  while (len > 0) {
    auto sent = send(fd, p, len, 0);
    p += sent;
    len -= sent;
  }

  return true;
}

bool SimpleUdpTransport::SendRtcp(const std::vector<uint8_t> &&data) {
  return SendRtp(std::move(data));
}

std::error_code SimpleUdpTransport::ListenOn(uint16_t port) {
  struct addrinfo hints = {0};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG | AI_PASSIVE;

  struct addrinfo *result;
  auto port_s = std::to_string(port);
  auto s = getaddrinfo(nullptr, port_s.c_str(), &hints, &result);
  if (s != 0) {
    // TODO: Lookup didn't work error
    SetNewState(rtp::transport::kTransportClosed);
    return std::error_code();
  }

  /* Loop until we find a result that binds */
  for (struct addrinfo *rp = result; rp != NULL; rp = rp->ai_next) {
    if (bind(fd, rp->ai_addr, rp->ai_addrlen) != -1) {
      SetNewState(rtp::transport::kTransportRecvReady);
      return std::error_code();
    }
  }

  auto ec = std::error_code(errno, std::system_category());
  ReportError(ec);
  return ec;
}

void SimpleUdpTransport::SocketThread() {
  while (fd >= 0) {
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    fd_set set;
    FD_ZERO(&set);
    FD_SET(fd, &set);

    /* Select will block and wait until we have data to read, or until timeout */
    auto r = ::select(fd + 1, &set, nullptr, nullptr, &tv);
    if (r == -1) {
      ReportError(std::error_code(errno, std::system_category()));
      continue;
    }

    /* Select might have timed-out or spurious wakeup */
    if (!FD_ISSET(fd, &set)) {
      continue;
    }

    /* Try to acquire bytes available */
    int bytes_available;
    if (ioctl(fd, FIONREAD, &bytes_available) == -1) {
      ReportError(std::error_code(errno, std::system_category()));
      continue;
    }

    /* 0 bytes available signals the socket is closed */
    if (bytes_available == 0) {
      SetNewState(rtp::transport::kTransportClosed);
      continue;
    }

    /* Try to read data */
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);
    std::vector<uint8_t> data(bytes_available);
    auto bytes_read = ::recvfrom(fd, data.data(), bytes_available, 0, (sockaddr *) &addr, &addrlen);

    /* The return value could be an error... */
    switch (bytes_read) {
      case -1:ReportError(std::error_code(errno, std::system_category()));
      case 0:SetNewState(rtp::transport::kTransportClosed);
        break;

      default:
        /* If we didn't set our remote address, set it to whom ever sent us a packet */
        if (!CurrentState() != rtp::transport::kTransportReady) {
          ConnectTo(addr, addrlen);
        }

        /* Truncate data and pass it to the demux method */
        data.resize(bytes_read);
        OnData(std::move(data));
    }
  }
}

void SimpleUdpTransport::ConnectTo(const sockaddr_storage &addr, const socklen_t addr_len) {
  if (connect(fd, (const sockaddr *) &addr, addr_len) == -1) {
    ReportError(std::error_code(errno, std::system_category()));
    return;
  }

  SetNewState(rtp::transport::kTransportReady);
}
