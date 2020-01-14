//
// Created by zhengqian on 2020/01/14.
//

#ifndef DRAWBOARD_DRAW_CLIENT_HPP
#define DRAWBOARD_DRAW_CLIENT_HPP

#include <cstdint>
#include <functional>
#include "common/abstract_canvas.hpp"
#include "socket/buffer.hpp"
#include "socket/tcp_socket.hpp"

class DrawClient {
public:
    DrawClient();
    ~DrawClient();
    void Login(std::string host, uint16_t port);
    void Logout();
    uint32_t GetUid();
    uint32_t GetColor();

    void InitViewport(int width, int height);
    void BeginDraw();
    void EndDraw();
    void DrawPoint(Point point);
    void Render(int width, int height);
    const char* GetPixelBuffer();
private:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketDataArrival(Buffer* buffer, size_t nread);
    void onSocketError(std::string message);
    void onPacketCallback(const Packet* packet);
private:
    AbstractCanvas canvas_;
    TcpSocket* socket_;
    bool socket_connected_;
    uint32_t uid_;
    uint32_t color_;
    uint32_t sequence_id_;
};


#endif //DRAWBOARD_DRAW_CLIENT_HPP
