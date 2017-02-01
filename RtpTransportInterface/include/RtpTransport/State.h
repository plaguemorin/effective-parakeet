

#ifndef RTPTRANSPORT_STATE_H
#define RTPTRANSPORT_STATE_H
namespace rtptransport {
    enum State {
        kTransportNotReady, // Transport cannot send or receive
        kTransportRecvReady, // Transport can only receive
        kTransportSendReady, // Transport can only send
        kTransportReady, // Transport can send and receive
        kTransportClosed // Transport is closed and is considered dead
    };
}
#endif //EFFECTIVE_PARAKEET_STATE_H
