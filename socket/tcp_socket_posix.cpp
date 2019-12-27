//
// @author magicxqq <xqq@xqq.im>
//

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "tcp_socket_posix.hpp"

TcpSocketPosix::~TcpSocketPosix() {

}

void TcpSocketPosix::SetCallback(OnConnectedCallback on_connect,
                                 OnDisconnectCallback on_disconnect,
                                 OnDataArrivalCallback on_arrival,
                                 OnErrorCallback on_error) {
    on_connect_ = on_connect;
    on_disconnect_ = on_disconnect;
    on_arrival_ = on_arrival;
    on_error_ = on_error;
}

bool TcpSocketPosix::Connect(std::string connect_addr, uint16_t port) {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);

    int enable = 1;
    if (setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable)) == -1) {
        on_error_("setsockopt(TCP_NODELAY) failed");
        return false;
    }

    remote_addr_.sin_family = AF_INET;
    remote_addr_.sin_port = htons(port);
    inet_pton(AF_INET, connect_addr.c_str(), &remote_addr_.sin_addr);

    int ret = connect(fd_, reinterpret_cast<struct sockaddr*>(&remote_addr_), sizeof(remote_addr_));
    if (ret == -1) {
        on_error_("connect() failed");
        return false;
    }

    thread_ = std::thread(&TcpSocketPosix::ThreadWorker, this);

    return true;
}

bool TcpSocketPosix::Send(const uint8_t* buffer, size_t length) {
    int ret = send(fd_, buffer, length, 0);
    if (ret == -1) {
        on_error_("send() returned with error");
        return false;
    }
    return true;
}

bool TcpSocketPosix::Shutdown() {
    int ret = shutdown(fd_, SHUT_RDWR);
    if (ret == -1) {
        on_error_("shutdown() returned with error");
        return false;
    }
    return true;
}

void TcpSocketPosix::ThreadWorker() {

}
