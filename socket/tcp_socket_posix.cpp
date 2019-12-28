//
// @author magicxqq <xqq@xqq.im>
//

#include <cassert>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "buffer.hpp"
#include "tcp_socket_posix.hpp"

TcpSocketPosix::TcpSocketPosix(int existing_fd, struct sockaddr_in remote_addr)
        : fd_(existing_fd), remote_addr_(remote_addr),
          on_connect_(nullptr), on_disconnect_(nullptr), on_arrival_(nullptr), on_error_(nullptr) {
    // do nothing here, defer the thread creation until SetCallback()
}

TcpSocketPosix::~TcpSocketPosix() {
    if (fd_ || thread_.joinable()) {
        Shutdown();
    }
}

void TcpSocketPosix::SetCallback(OnConnectedCallback on_connect,
                                 OnDisconnectCallback on_disconnect,
                                 OnDataArrivalCallback on_arrival,
                                 OnErrorCallback on_error) {
    on_connect_ = on_connect;
    on_disconnect_ = on_disconnect;
    on_arrival_ = on_arrival;
    on_error_ = on_error;

    if (fd_ && !thread_.joinable()) {
        thread_ = std::thread(&TcpSocketPosix::ThreadWorker, this);
    }
}

bool TcpSocketPosix::Connect(std::string connect_addr, uint16_t port) {
    assert(fd_ == 0);

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

    on_connect_();

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
    if (fd_) {
        int ret = shutdown(fd_, SHUT_RDWR);
        if (ret == -1) {
            on_error_("shutdown() returned with error");
            fd_ = 0;
        }
    }

    if (thread_.joinable()) {
        thread_.join();
    }

    return true;
}

void TcpSocketPosix::ThreadWorker() {
    Buffer buffer(4096);

    uint8_t buf[512] = {0};

    while (true) {
        ssize_t nread = recv(fd_, buf, sizeof(buf), 0);

        if (nread == 0) {
            // connection closing
            close(fd_);
            fd_ = 0;
            on_disconnect_();
            break;
        } else if (nread == -1) {
            close(fd_);
            fd_ = 0;
            on_error_("recv() returned with error");
            break;
        }

        buffer.Write(buf, static_cast<size_t>(nread));
        on_arrival_(&buffer, (size_t)nread);
        memset(buf, 0, sizeof(buf));
    }
}
