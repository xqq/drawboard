//
// Created by magicxqq on 2020/01/14.
//

#ifndef DRAWBOARD_DRAW_CLIENT_HPP
#define DRAWBOARD_DRAW_CLIENT_HPP

#include <cstdint>
#include <memory>
#include <functional>
#include "common/abstract_canvas.hpp"
#include "common/packet.hpp"
#include "socket/read_write_buffer.hpp"
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
    void UndoLast();
    void Render(int width, int height);
    const char* GetPixelBuffer();
private:
    void SendPacket(PacketHeader* header, const uint8_t* payload, size_t payload_length);
private:
    void onSocketConnected(TcpSocket* socket);
    void onSocketDisconnected(TcpSocket* socket);
    void onSocketDataArrival(TcpSocket* socket, ReadWriteBuffer* buffer, size_t nread);
    void onSocketError(TcpSocket* socket, const std::string& message);
    void onPacketCallback(Packet&& packet);
private:
    AbstractCanvas canvas_;
    std::unique_ptr<TcpSocket> socket_;
    bool socket_connected_;
    uint32_t uid_;
    uint32_t color_;
    uint32_t sequence_id_;
};


#endif //DRAWBOARD_DRAW_CLIENT_HPP
