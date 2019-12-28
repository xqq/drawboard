//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_TCP_LISTENER_WINSOCK_HPP
#define DRAWBOARD_TCP_LISTENER_WINSOCK_HPP

#include <atomic>
#include <thread>
#include <winsock2.h>
#include "tcp_listener.hpp"


class TcpListenerWinsock : public TcpListener {
public:
    TcpListenerWinsock(OnAcceptedCallback on_accept, OnErrorCallback on_error);
    virtual ~TcpListenerWinsock() override;
    virtual bool StartListen(std::string bind_addr, uint16_t port) override;
    virtual bool EndListen() override;
private:
    void ThreadWorker();
private:
    OnAcceptedCallback on_accept_;
    OnErrorCallback on_error_;
    std::thread thread_;
    std::atomic<bool> shutdown_flag_;
    SOCKET listen_socket_;
};


#endif // DRAWBOARD_TCP_LISTENER_WINSOCK_HPP
