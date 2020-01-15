//
// @author magicxqq <xqq@xqq.im>
//

#include <cassert>
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include "tcp_socket_winsock.hpp"
#include "read_write_buffer.hpp"

TcpSocketWinsock::TcpSocketWinsock() {
    InitializeWinsock();
}

TcpSocketWinsock::TcpSocketWinsock(SOCKET existing_socket, struct sockaddr_in remote_addr)
        : socket_(existing_socket), remote_addr_(remote_addr),
          on_connect_(nullptr), on_disconnect_(nullptr), on_arrival_(nullptr), on_error_(nullptr) {
    InitializeWinsock();
}

void TcpSocketWinsock::InitializeWinsock() {
    WSADATA wsa_data;
    int ret = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (ret != 0) {
        on_error_(this, "WSAStartup failed");
    }
}

TcpSocketWinsock::~TcpSocketWinsock() {
    if (socket_ || thread_.joinable()) {
        Shutdown();
    }

    WSACleanup();
}

void TcpSocketWinsock::SetCallback(OnConnectedCallback on_connect,
                                   OnDisconnectCallback on_disconnect,
                                   OnDataArrivalCallback on_arrival,
                                   OnErrorCallback on_error) {
    on_connect_ = on_connect;
    on_disconnect_ = on_disconnect;
    on_arrival_ = on_arrival;
    on_error_ = on_error;

    if (socket_ && !thread_.joinable()) {
        thread_ = std::thread(&TcpSocketWinsock::ThreadWorker, this);
    }
}

bool TcpSocketWinsock::Connect(std::string connect_addr, uint16_t port) {
    assert(socket_ == 0);

    struct addrinfo *result = nullptr, *ptr = nullptr, hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    std::string port_string = std::to_string(port);

    int ret = getaddrinfo(connect_addr.c_str(), port_string.c_str(), &hints, &result);
    if (ret) {
        on_error_(this, "getaddrinfo() failed");
        return false;
    }

    socket_ = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (socket_ == INVALID_SOCKET) {
        on_error_(this, "socket() creation failed");
        freeaddrinfo(result);
        return false;
    }

    ret = connect(socket_, result->ai_addr, static_cast<int>(result->ai_addrlen));
    if (ret == SOCKET_ERROR) {
        on_error_(this, "connect() failec");
        closesocket(socket_);
        socket_ = 0;
        freeaddrinfo(result);
        return false;
    }

    memcpy(&remote_addr_, result->ai_addr, sizeof(remote_addr_));
    freeaddrinfo(result);

    thread_ = std::thread(&TcpSocketWinsock::ThreadWorker, this);

    return true;
}

bool TcpSocketWinsock::Send(const uint8_t* buffer, size_t length) {
    int ret = send(socket_, reinterpret_cast<const char*>(buffer), static_cast<int>(length), 0);
    if (ret == SOCKET_ERROR) {
        on_error_(this, "send() returned with error");
        return false;
    }
    return true;
}

bool TcpSocketWinsock::Shutdown() {
    if (socket_) {
        int ret = shutdown(socket_, SD_BOTH);
        if (ret == SOCKET_ERROR) {
            on_error_(this, "shutdown() failed");
            closesocket(socket_);
            socket_ = 0;
        }
    }

    if (thread_.joinable()) {
        thread_.join();
    }

    return true;
}

void TcpSocketWinsock::ThreadWorker() {
    Buffer buffer(4096);

    uint8_t buf[512] = {0};

    while (true) {
        int nread = recv(socket_, reinterpret_cast<char*>(buf), sizeof(buf), 0);

        if (nread == 0) {
            // connection closing
            closesocket(socket_);
            socket_ = 0;
            on_disconnect_(this);
            break;
        } else if (nread < 0) {
            closesocket(socket_);
            socket_ = 0;
            on_error_(this, "recv() returned with error");
            break;
        }

        // else: nread bytes received
        buffer.Write(buf, static_cast<size_t>(nread));
        on_arrival_(this, &buffer, (size_t)nread);
        memset(buf, 0, sizeof(buf));
    }
}
