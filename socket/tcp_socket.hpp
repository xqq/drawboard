//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_TCP_SOCKET_HPP
#define DRAWBOARD_TCP_SOCKET_HPP

#include <string>
#include <functional>
#include "noncopyable.hpp"

struct sockaddr_in;

class Buffer;

class TcpSocket : public Noncopyable {
public:
    using OnConnectedCallback = std::function<void()>;
    using OnDisconnectCallback = std::function<void()>;
    using OnDataArrivalCallback = std::function<void(Buffer* buffer, size_t nread)>;
    using OnErrorCallback = std::function<void(std::string)>;
public:
    static TcpSocket* Create();
public:
    explicit TcpSocket() = default;
    virtual ~TcpSocket() {}
    virtual void SetCallback(OnConnectedCallback on_connect,
                             OnDisconnectCallback on_disconnect,
                             OnDataArrivalCallback on_arrival,
                             OnErrorCallback on_error) = 0;
    virtual bool Connect(std::string connect_addr, uint16_t port) = 0;
    virtual bool Send(const uint8_t* buffer, size_t length) = 0;
    virtual bool Shutdown() = 0;
};


#endif // DRAWBOARD_TCP_SOCKET_HPP
