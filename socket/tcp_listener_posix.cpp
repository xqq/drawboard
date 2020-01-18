//
// @author magicxqq <xqq@xqq.im>
//

#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "tcp_socket_posix.hpp"
#include "tcp_listener_posix.hpp"

TcpListenerPosix::~TcpListenerPosix() {
    if (listen_fd_ || thread_.joinable()) {
        EndListen();
    }
}

bool TcpListenerPosix::StartListen(std::string bind_addr, uint16_t port) {
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd_ == -1) {
        on_error_("Create TCP Socket failed\n");
        return false;
    }

    int enable = 1;
    if (setsockopt(listen_fd_, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable)) == -1) {
        on_error_("setsockopt(TCP_NODELAY) failed\n");
        return false;
    }

    if (setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) {
        on_error_("setsockopt(SO_REUSEADDR) failed\n");
        return false;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_aton(bind_addr.c_str(), &server_addr.sin_addr);

    if (bind(listen_fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        on_error_("Bind port failed\n");
        return false;
    }

    if (listen(listen_fd_, SOMAXCONN) == -1) {
        on_error_("listen() failed\n");
        return false;
    }

    // create listener worker thread
    thread_ = std::thread(&TcpListenerPosix::ThreadWorker, this);

    return true;
}

bool TcpListenerPosix::EndListen() {
    if (listen_fd_) {
        shutdown_flag_ = true;
        close(listen_fd_);
        listen_fd_ = 0;
    }

    if (thread_.joinable()) {
        thread_.detach();
    }

    return true;
}

void TcpListenerPosix::ThreadWorker() {
    sockaddr_in incoming_addr;
    socklen_t incoming_addr_size = sizeof(incoming_addr);

    while (!shutdown_flag_) {
        int client_fd = accept(listen_fd_, reinterpret_cast<struct sockaddr*>(&incoming_addr), &incoming_addr_size);
        if (client_fd < 0) {
            // error raised
            if ((errno == EINVAL || errno == ECONNABORTED) && shutdown_flag_) {
                break;
            } else {
                on_error_("accept() failed");
                break;
            }
        } else {
            // We got a socket fd
            auto socket = std::make_unique<TcpSocketPosix>(client_fd, incoming_addr);
            on_accept_(std::move(socket), incoming_addr);
        }
    }
}
