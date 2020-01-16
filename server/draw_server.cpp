#if defined(_WIN32)
    #include <winsock2.h>
#else
    #include <netinet/in.h>
#endif

#include <iostream>
#include <ctime>
#include "common/log.hpp"
#include "socket/tcp_listener.hpp"
#include "socket/tcp_socket.hpp"
#include "common/packet.hpp"
#include "protocol/protocol_generated.h"
#include "draw_server.hpp"

using namespace std::placeholders;

DrawServer::DrawServer() : max_uid_(-1) {
    srand(time(nullptr));
    transmit_worker_.SetTaskQueue(task_queue_);
    transmit_worker_.SetSocketCluster(socket_cluster_);
    listener_.reset(TcpListener::Create(std::bind(&DrawServer::onListenerAccepted, this, _1, _2),
                                        std::bind(&DrawServer::onListenerError, this, _1)));
}

DrawServer::~DrawServer() {
    Stop();
}

void DrawServer::Start(std::string bind_addr, uint16_t port) {
    transmit_worker_.Start();
    listener_->StartListen(std::move(bind_addr), port);
}

void DrawServer::Stop() {
    task_queue_.Clear();
    task_queue_.NotifyExit();
    listener_->EndListen();
    ShutdownSockets();
}

void DrawServer::ShutdownSockets() {
    for (auto& iter : socket_cluster_) {
        TcpSocket* socket = iter.second.get();
        socket->Shutdown();
    }
}

void DrawServer::onListenerAccepted(std::unique_ptr<TcpSocket> socket, struct sockaddr_in) {
    Log::InfoF("[Server] onListenerAccepted\n");
    socket->SetCallback(std::bind(&DrawServer::onClientConnected, this, _1),
                        std::bind(&DrawServer::onClientDisconnected, this, _1),
                        std::bind(&DrawServer::onClientDataArrival, this, _1, _2, _3),
                        std::bind(&DrawServer::onClientError, this, _1, _2));
    uint32_t uid = ++max_uid_;
    socket->SetUserData(reinterpret_cast<void*>(uid));
    TcpSocket* socket_ptr = socket.get();
    socket_cluster_.insert({uid, std::move(socket)});

    // determine user color
    uint32_t color = RandomColor();

    PostServerHello(socket_ptr, uid, color);
    PostFullImage(socket_ptr);
    BroadcastUserEnter(uid, color);
}

void DrawServer::onListenerError(const std::string& msg) {
    Log::ErrorF("[Server] onListenerError: %s\n", msg.c_str());
}

void DrawServer::onClientConnected(TcpSocket* socket) {
    // do nothing
}

void DrawServer::onClientDisconnected(TcpSocket* socket) {
    uint32_t uid = static_cast<uint32_t>(reinterpret_cast<uint64_t>(socket->GetUserData()));
    Log::InfoF("[Server] onClientDisconnected: uid %u left\n", uid);

    socket->Shutdown();
    socket_cluster_.erase(uid);
    BroadcastUserLeave(uid);
}

void DrawServer::onClientDataArrival(TcpSocket* socket, ReadWriteBuffer* buffer, size_t nread) {
    uint32_t uid = static_cast<uint32_t>(reinterpret_cast<uint64_t>(socket->GetUserData()));

    ParseBuffer(buffer, [&](Packet&& packet) {
        const PacketPayload* packet_payload = packet.payload;
        switch (packet.header.packet_type()) {
            case PacketType_StartDraw: {
                auto payload = packet_payload->payload_as<StartDrawPayload>();
                canvas_.BeginDraw(payload->uid(), payload->sequence_id(), payload->color());
                BroadcastPacketExclude(packet, socket);
                break;
            }
            case PacketType_EndDraw: {
                auto payload = packet_payload->payload_as<EndDrawPayload>();
                canvas_.EndDraw(payload->uid(), payload->sequence_id());
                BroadcastPacketExclude(packet, socket);
                break;
            }
            case PacketType_DrawPoints: {
                auto payload = packet_payload->payload_as<DrawPointsPayload>();
                auto points_vector = payload->points();
                if (points_vector->size() == 0) {
                    break;
                }
                std::vector<Point> points(points_vector->size());
                memcpy(points.data(), points_vector->data(), sizeof(Vec2) * points_vector->size());
                canvas_.DrawPoints(payload->uid(), payload->sequence_id(), points);
                BroadcastPacketExclude(packet, socket);
                break;
            }
            case PacketType_DeleteBatch: {
                auto payload = packet_payload->payload_as<DeleteBatchPayload>();
                canvas_.ClearBatch(payload->uid(), payload->sequence_id());
                BroadcastPacketExclude(packet, socket);
                break;
            }
            case PacketType_ServerHello:
            case PacketType_FullImage:
            case PacketType_UserEnter:
            case PacketType_UserLeave:
            default:
                // ignore
                break;
        }
    });
}

void DrawServer::onClientError(TcpSocket* socket, const std::string& message) {
    uint32_t uid = static_cast<uint32_t>(reinterpret_cast<uint64_t>(socket->GetUserData()));
    Log::ErrorF("[Server] onClientError: uid = %u, msg = %s\n", uid, message.c_str());
    socket->Shutdown();
    socket_cluster_.erase(uid);
}

void DrawServer::PostPacket(TcpSocket* socket, PacketHeader* header, const uint8_t* payload, size_t payload_length) {
    std::vector<uint8_t> packet_buffer(sizeof(*header) + payload_length);

    size_t header_size = sizeof(*header);
    memcpy(packet_buffer.data(), header, header_size);
    memcpy(packet_buffer.data() + header_size, payload, payload_length);

    Task t(TaskType::kPrivateSend, std::move(packet_buffer), socket);
    task_queue_.Enqueue(std::move(t));
}

void DrawServer::PostServerHello(TcpSocket *socket, uint32_t uid, uint32_t color) {
    flatbuffers::FlatBufferBuilder builder;
    auto payload = CreateServerHelloPayload(builder, uid, color);
    auto packet_payload = CreatePacketPayload(builder, Payload_ServerHelloPayload, payload.Union());
    builder.Finish(packet_payload);

    PacketHeader header(PacketType_ServerHello, builder.GetSize());

    PostPacket(socket, &header, builder.GetBufferPointer(), builder.GetSize());
}

void DrawServer::PostFullImage(TcpSocket *socket) {
    const PointMap* map = canvas_.GetMap();

    std::vector<flatbuffers::Offset<DrawBatch>> draw_batch_offsets;

    flatbuffers::FlatBufferBuilder builder;

    for (auto& user_pair : *map) {
        for (auto& batch_pair : user_pair.second) {
            const BatchInfo* batch_info = &batch_pair.second;

            std::vector<Vec2> points(batch_info->points.size());
            memcpy(points.data(), batch_info->points.data(), sizeof(Vec2) * batch_info->points.size());
            auto batch = CreateDrawBatchDirect(builder, user_pair.first, batch_pair.first, batch_info->color, &points);
            draw_batch_offsets.push_back(batch);
        }
    }

    auto payload = CreateFullImagePayloadDirect(builder, &draw_batch_offsets);
    auto packet_payload = CreatePacketPayload(builder, Payload_FullImagePayload, payload.Union());
    builder.Finish(packet_payload);

    PacketHeader header(PacketType_FullImage, builder.GetSize());

    PostPacket(socket, &header, builder.GetBufferPointer(), builder.GetSize());
}

void DrawServer::BroadcastPacketExclude(const Packet& packet, TcpSocket* exclude_socket) {
    std::vector<uint8_t> packet_buffer(sizeof(packet.header) + packet.payload_buffer.size());

    size_t header_size = sizeof(packet.header);
    memcpy(packet_buffer.data(), &packet.header, header_size);
    memcpy(packet_buffer.data() + header_size, packet.payload_buffer.data(), packet.payload_buffer.size());

    Task t(TaskType::kBroadcastSendExclude, std::move(packet_buffer), exclude_socket);
    task_queue_.Enqueue(std::move(t));
}

void DrawServer::BroadcastPacket(const Packet& packet) {
    std::vector<uint8_t> packet_buffer(sizeof(packet.header) + packet.payload_buffer.size());

    size_t header_size = sizeof(packet.header);
    memcpy(packet_buffer.data(), &packet.header, header_size);
    memcpy(packet_buffer.data() + header_size, packet.payload_buffer.data(), packet.payload_buffer.size());

    Task t(TaskType::kBroadcastSend, std::move(packet_buffer));
    task_queue_.Enqueue(std::move(t));
}

void DrawServer::BroadcastPacket(PacketHeader* header, const uint8_t* payload, size_t payload_length) {
    std::vector<uint8_t> packet_buffer(sizeof(*header) + payload_length);

    size_t header_size = sizeof(*header);
    memcpy(packet_buffer.data(), header, header_size);
    memcpy(packet_buffer.data() + header_size, payload, payload_length);

    Task t(TaskType::kBroadcastSend, std::move(packet_buffer));
    task_queue_.Enqueue(std::move(t));
}

void DrawServer::BroadcastUserEnter(uint32_t uid, uint32_t color) {
    flatbuffers::FlatBufferBuilder builder;
    auto payload = CreateUserEnterPayload(builder, uid, color);
    auto packet_payload = CreatePacketPayload(builder, Payload_UserEnterPayload, payload.Union());
    builder.Finish(packet_payload);

    PacketHeader header(PacketType_UserEnter, builder.GetSize());

    BroadcastPacket(&header, builder.GetBufferPointer(), builder.GetSize());
}

void DrawServer::BroadcastUserLeave(uint32_t uid) {
    flatbuffers::FlatBufferBuilder builder;
    auto payload = CreateUserEnterPayload(builder, uid);
    auto packet_payload = CreatePacketPayload(builder, Payload_UserLeavePayload, payload.Union());
    builder.Finish(packet_payload);

    PacketHeader header(PacketType_UserLeave, builder.GetSize());

    BroadcastPacket(&header, builder.GetBufferPointer(), builder.GetSize());
}

uint32_t DrawServer::RandomColor() {
    uint8_t r = rand() % 256;
    uint8_t g = rand() % 256;
    uint8_t b = rand() % 256;

    r = ((int)r + 255) / 2;
    g = ((int)g + 255) / 2;
    b = ((int)b + 255) / 2;

    return 0xFF << 24 |
           r    << 16 |
           g    << 8  |
           b;
}

std::vector<uint8_t> DrawServer::BufToVector(const uint8_t *buffer, size_t size) {
    std::vector<uint8_t> vec(size);
    memcpy(vec.data(), buffer, size);
    return vec;
}
