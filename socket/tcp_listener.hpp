//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_TCP_LISTENER_HPP
#define DRAWBOARD_TCP_LISTENER_HPP

#include <string>
#include <memory>
#include <cstdint>
#include <functional>
#include "common/noncopyable.hpp"

struct sockaddr_in;

class TcpSocket;

class TcpListener : public Noncopyable {
public:
    using OnAcceptedCallback = std::function<void(std::unique_ptr<TcpSocket>, struct sockaddr_in)>;
    using OnErrorCallback = std::function<void(std::string)>;
public:
    static TcpListener* Create(OnAcceptedCallback on_accept, OnErrorCallback on_error);
public:
    explicit TcpListener() = default;
    virtual ~TcpListener() {}
    virtual bool StartListen(std::string bind_addr, uint16_t port) = 0;
    virtual bool EndListen() = 0;
};


#endif // DRAWBOARD_TCP_LISTENER_HPP
