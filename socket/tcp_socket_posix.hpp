//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_TCP_SOCKET_POSIX_HPP
#define DRAWBOARD_TCP_SOCKET_POSIX_HPP

#include <thread>
#include <netinet/in.h>
#include "tcp_socket.hpp"

class TcpSocketPosix : public TcpSocket {
public:
    TcpSocketPosix() = default;
    TcpSocketPosix(int existing_fd, struct sockaddr_in remote_addr);
    virtual ~TcpSocketPosix() override;
    virtual void SetCallback(OnConnectedCallback on_connect,
                             OnDisconnectCallback on_disconnect,
                             OnDataArrivalCallback on_arrival,
                             OnErrorCallback on_error) override;
    virtual bool Connect(std::string connect_addr, uint16_t port) override;
    virtual bool Send(const uint8_t* buffer, size_t length) override;
    virtual bool Shutdown() override;
private:
    void ThreadWorker();
private:
    int fd_;
    struct sockaddr_in remote_addr_;
    std::thread thread_;
    OnConnectedCallback on_connect_;
    OnDisconnectCallback on_disconnect_;
    OnDataArrivalCallback on_arrival_;
    OnErrorCallback on_error_;
};


#endif // DRAWBOARD_TCP_SOCKET_POSIX_HPP
