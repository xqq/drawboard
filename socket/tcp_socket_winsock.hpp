//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_TCP_SOCKET_WINSOCK_HPP
#define DRAWBOARD_TCP_SOCKET_WINSOCK_HPP

#include <thread>
#include <winsock2.h>
#include "tcp_socket.hpp"

class TcpSocketWinsock : public TcpSocket {
public:
    TcpSocketWinsock();
    TcpSocketWinsock(SOCKET existing_socket, struct sockaddr_in remote_addr);
    virtual ~TcpSocketWinsock() override;
    virtual void SetCallback(OnConnectedCallback on_connect,
                             OnDisconnectCallback on_disconnect,
                             OnDataArrivalCallback on_arrival,
                             OnErrorCallback on_error) override;
    virtual bool Connect(std::string connect_addr, uint16_t port) override;
    virtual bool Send(const uint8_t* buffer, size_t length) override;
    virtual bool Shutdown() override;
private:
    void InitializeWinsock();
    void ThreadWorker();
private:
    SOCKET socket_;
    struct sockaddr_in remote_addr_;
    std::thread thread_;
    OnConnectedCallback on_connect_;
    OnDisconnectCallback on_disconnect_;
    OnDataArrivalCallback on_arrival_;
    OnErrorCallback on_error_;
};


#endif // DRAWBOARD_TCP_SOCKET_WINSOCK_HPP
