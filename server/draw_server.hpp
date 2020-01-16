//
// Created by magicxqq on 2020/01/15.
//

#ifndef DRAWBOARD_DRAW_SERVER_HPP
#define DRAWBOARD_DRAW_SERVER_HPP

#include <memory>
#include "common/abstract_canvas.hpp"
#include "common/blocking_queue.hpp"
#include "socket/tcp_listener.hpp"
#include "socket/tcp_socket.hpp"
#include "transmit_worker.hpp"

class DrawServer {
public:
    DrawServer();
    ~DrawServer();
    void Start(std::string bind_addr, uint16_t port);
    void Stop();
private:
    void ShutdownSockets();
private:
    void onListenerAccepted(std::unique_ptr<TcpSocket> socket, struct sockaddr_in);
    void onListenerError(const std::string& msg);
    void onClientConnected(TcpSocket* socket);
    void onClientDisconnected(TcpSocket* socket);
    void onClientDataArrival(TcpSocket* socket, ReadWriteBuffer* buffer, size_t nread);
    void onClientError(TcpSocket* socket, const std::string& message);
private:
    void PostPacket(TcpSocket* socket, PacketHeader* header, const uint8_t* payload, size_t payload_length);
    void PostServerHello(TcpSocket* socket, uint32_t uid, uint32_t color);
    void PostFullImage(TcpSocket* socket);
    void BroadcastPacket(const Packet& packet);
    void BroadcastPacket(PacketHeader* header, const uint8_t* payload, size_t payload_length);
    void BroadcastUserEnter(uint32_t uid, uint32_t color);
    void BroadcastUserLeave(uint32_t uid);
private:
    static uint32_t RandomColor();
    static std::vector<uint8_t> BufToVector(const uint8_t* buffer, size_t size);
private:
    std::unique_ptr<TcpListener> listener_;
    SocketCluster socket_cluster_;
    BlockingQueue<Task> task_queue_;
    TransmitWorker transmit_worker_;
    AbstractCanvas canvas_;
    uint32_t max_uid_;
};

#endif //DRAWBOARD_DRAW_SERVER_HPP
