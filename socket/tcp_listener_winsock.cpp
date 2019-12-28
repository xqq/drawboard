//
// @author magicxqq <xqq@xqq.im>
//

#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include "tcp_socket_winsock.hpp"
#include "tcp_listener_winsock.hpp"


TcpListenerWinsock::TcpListenerWinsock(OnAcceptedCallback on_accept, OnErrorCallback on_error)
    : on_accept_(on_accept), on_error_(on_error), thread_(), shutdown_flag_(false), listen_socket_(0) {

    // Initialize winsock
    WSADATA wsa_data;
    int ret = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (ret != 0) {
        on_error_("WSAStartup failed");
    }
}

TcpListenerWinsock::~TcpListenerWinsock() {
    if (listen_socket_ || thread_.joinable()) {
        EndListen();
    }

    WSACleanup();
}

bool TcpListenerWinsock::StartListen(std::string bind_addr, uint16_t port) {
    listen_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket_ == INVALID_SOCKET) {
        on_error_("Create TCP Socket failed\n");
        return false;
    }

    BOOL enable = 1;
    if (setsockopt(listen_socket_, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&enable), sizeof(enable))) {
        on_error_("setsockopt(TCP_NODELAY) failed\n");
        return false;
    }

    if (setsockopt(listen_socket_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&enable), sizeof(enable))) {
        on_error_("setsockopt(SO_REUSEADDR) failed\n");
        return false;
    }

    std::string port_string = std::to_string(port);
    struct addrinfo *result = nullptr, *ptr = nullptr, hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the bind address and port to be used by the server
    int ret = getaddrinfo(bind_addr.c_str(), port_string.c_str(), &hints, &result);
    if (ret) {
        on_error_("getaddrinfo failed");
        return false;
    }

    ret = bind( listen_socket_, result->ai_addr, (int)result->ai_addrlen);
    if (ret == SOCKET_ERROR) {
        on_error_("bind failed with error");
        freeaddrinfo(result);
        closesocket(listen_socket_);
        listen_socket_ = 0;
        return false;
    }

    freeaddrinfo(result);

    // start listen
    if (listen(listen_socket_, SOMAXCONN) == SOCKET_ERROR) {
        on_error_("Listen failed with error");
        closesocket(listen_socket_);
        listen_socket_ = 0;
        return false;
    }

    // create worker thread
    thread_ = std::thread(&TcpListenerWinsock::ThreadWorker, this);

    return true;
}

bool TcpListenerWinsock::EndListen() {
    if (listen_socket_) {
        shutdown_flag_ = true;
        int ret = shutdown(listen_socket_, SD_BOTH);
        if (ret == SOCKET_ERROR) {
            on_error_("shutdown failed");
            listen_socket_ = 0;
        }
    }

    if (thread_.joinable()) {
        thread_.join();
    }

    if (listen_socket_) {
        closesocket(listen_socket_);
        listen_socket_ = 0;
    }

    return true;
}

void TcpListenerWinsock::ThreadWorker() {
    sockaddr_in incoming_addr;
    socklen_t incoming_addr_size = sizeof(incoming_addr);

    while (!shutdown_flag_) {
        SOCKET client_socket_ = accept(listen_socket_, reinterpret_cast<sockaddr*>(&incoming_addr), &incoming_addr_size);
        if (client_socket_ == INVALID_SOCKET) {
            on_error_("accept failed");
            closesocket(listen_socket_);
            listen_socket_ = 0;
        } else {
            // We got a valid socket
            auto socket = std::make_unique<TcpSocketWinsock>(client_socket_, incoming_addr);
            on_accept_(std::move(socket), incoming_addr);
        }
    }
}
