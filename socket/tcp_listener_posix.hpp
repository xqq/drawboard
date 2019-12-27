//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_TCP_LISTENER_POSIX_HPP
#define DRAWBOARD_TCP_LISTENER_POSIX_HPP

#include <thread>
#include <atomic>
#include "tcp_listener.hpp"

class TcpListenerPosix : public TcpListener {
public:
    TcpListenerPosix(OnAcceptedCallback on_accept, OnErrorCallback on_error)
        : on_accept_(on_accept), on_error_(on_error), thread_(), shutdown_flag_(false), listen_fd_(0) {}
    virtual ~TcpListenerPosix() override;
    virtual bool StartListen(std::string bind_addr, uint16_t port) override;
    virtual bool EndListen() override;
private:
    void ThreadWorker();
private:
    OnAcceptedCallback on_accept_;
    OnErrorCallback on_error_;
    std::thread thread_;
    std::atomic<bool> shutdown_flag_;
    int listen_fd_;
};


#endif // DRAWBOARD_TCP_LISTENER_POSIX_HPP
