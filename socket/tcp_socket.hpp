//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_TCP_SOCKET_HPP
#define DRAWBOARD_TCP_SOCKET_HPP

#include <string>
#include <functional>
#include "common/noncopyable.hpp"

struct sockaddr_in;

class ReadWriteBuffer;

class TcpSocket : public Noncopyable {
public:
    using OnConnectedCallback = std::function<void()>;
    using OnDisconnectCallback = std::function<void()>;
    using OnDataArrivalCallback = std::function<void(ReadWriteBuffer* buffer, size_t nread, TcpSocket* socket, void* user_data)>;
    using OnErrorCallback = std::function<void(std::string)>;
public:
    static TcpSocket* Create();
public:
    explicit TcpSocket() : user_data_(nullptr) {}
    virtual ~TcpSocket() {}
    virtual void SetCallback(OnConnectedCallback on_connect,
                             OnDisconnectCallback on_disconnect,
                             OnDataArrivalCallback on_arrival,
                             OnErrorCallback on_error) = 0;
    virtual bool Connect(std::string connect_addr, uint16_t port) = 0;
    virtual bool Send(const uint8_t* buffer, size_t length) = 0;
    virtual bool Shutdown() = 0;

    void SetUserData(void* data) {
        user_data_ = data;
    }

    void* GetUserData() {
        return user_data_;
    }
protected:
    void* user_data_;
};


#endif // DRAWBOARD_TCP_SOCKET_HPP
