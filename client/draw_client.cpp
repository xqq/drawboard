//
// Created by magicxqq on 2020/01/14.
//

#include "common/log.hpp"
#include "common/packet.hpp"
#include "protocol/protocol_generated.h"
#include "draw_client.hpp"

using namespace std::placeholders;

DrawClient::DrawClient() : socket_connected_(false), uid_(0), color_(0), sequence_id_(-1) {
    socket_.reset(TcpSocket::Create());
    socket_->SetCallback(std::bind(&DrawClient::onSocketConnected, this, _1),
                         std::bind(&DrawClient::onSocketDisconnected, this, _1),
                         std::bind(&DrawClient::onSocketDataArrival, this, _1, _2, _3),
                         std::bind(&DrawClient::onSocketError, this, _1, _2));
}

DrawClient::~DrawClient() {
    if (socket_connected_) {
        Logout();
    }
}

void DrawClient::Login(std::string host, uint16_t port) {
    socket_->Connect(host, port);
}

void DrawClient::Logout() {
    socket_->Shutdown();
    socket_connected_ = false;
}

uint32_t DrawClient::GetUid() {
    return uid_;
}

uint32_t DrawClient::GetColor() {
    return color_;
}


void DrawClient::InitViewport(int width, int height) {
    canvas_.InitPixelBuffer(width, height);
}

void DrawClient::BeginDraw() {
    if (!socket_connected_) {
        return;
    }
    sequence_id_++;
    canvas_.BeginDraw(uid_, sequence_id_, color_);

    PacketHeader header(PacketType_StartDraw, sizeof(StartDrawPayload));

    flatbuffers::FlatBufferBuilder builder;
    auto payload = CreateStartDrawPayload(builder, uid_, sequence_id_, color_);
    auto packet = CreatePacket(builder, &header, Payload_StartDrawPayload, payload.Union());
    builder.Finish(packet);

    socket_->Send(builder.GetBufferPointer(), builder.GetSize());
}

void DrawClient::EndDraw() {
    if (!socket_connected_) {
        return;
    }
    canvas_.EndDraw(uid_, sequence_id_);

    PacketHeader header(PacketType_EndDraw, sizeof(EndDrawPayload));

    flatbuffers::FlatBufferBuilder builder;
    auto payload = CreateEndDrawPayload(builder, uid_, sequence_id_);
    auto packet = CreatePacket(builder, &header, Payload_EndDrawPayload, payload.Union());
    builder.Finish(packet);

    socket_->Send(builder.GetBufferPointer(), builder.GetSize());
}

void DrawClient::DrawPoint(Point point) {
    if (!socket_connected_) {
        return;
    }
    canvas_.DrawPoint(uid_, sequence_id_, point);

    PacketHeader header(PacketType_DrawPoints, sizeof(DrawPointsPayload));

    flatbuffers::FlatBufferBuilder builder;
    std::vector<Vec2> points = {Vec2((int16_t)point.x, (int16_t)point.y)};
    auto points_vector = builder.CreateVectorOfStructs(points);
    auto payload = CreateDrawPointsPayload(builder, uid_, sequence_id_, points_vector.o);
    auto packet = CreatePacket(builder, &header, Payload_EndDrawPayload, payload.Union());
    builder.Finish(packet);

    socket_->Send(builder.GetBufferPointer(), builder.GetSize());
}

void DrawClient::Render(int width, int height) {
    canvas_.Render(width, height);
}

const char *DrawClient::GetPixelBuffer() {
    return canvas_.GetPixelBuffer();
}

void DrawClient::onSocketConnected(TcpSocket* socket) {
    Log::Info("onSocketConnected");
    socket_connected_ = true;
}

void DrawClient::onSocketDisconnected(TcpSocket* socket) {
    Log::Info("onSocketDisonnected");
    socket_connected_ = false;
}

void DrawClient::onSocketDataArrival(TcpSocket* socket, ReadWriteBuffer* buffer, size_t nread) {
    ParseBuffer(buffer, std::bind(&DrawClient::onPacketCallback, this, _1));
}

void DrawClient::onSocketError(TcpSocket* socket, const std::string& message) {
    Log::ErrorF("onSocketError: %s\n", message.c_str());
    socket_connected_ = false;
}

void DrawClient::onPacketCallback(const Packet* packet) {
    Log::InfoF("onPacketCallback: %s\n", EnumNamesPacketType()[packet->header()->packet_type()]);
    switch (packet->header()->packet_type()) {
        case PacketType_ServerHello:
            uid_ = packet->payload_as<ServerHelloPayload>()->uid();
            color_ = packet->payload_as<ServerHelloPayload>()->color();
            break;
        case PacketType_FullImage: {
            auto payload = packet->payload_as<FullImagePayload>();
            auto batches = payload->batches();

            for (auto draw_batch : *batches) {
                auto points_vector = draw_batch->points();
                if (points_vector->size() == 0) {
                    continue;
                }
                std::vector<Point> points(points_vector->size());
                memcpy(points.data(), points_vector->data(), sizeof(Vec2) * points_vector->size());
                canvas_.BeginDraw(draw_batch->uid(), draw_batch->sequence_id(), draw_batch->color());
                canvas_.DrawPoints(draw_batch->uid(), draw_batch->sequence_id(), points);
                canvas_.EndDraw(draw_batch->uid(), draw_batch->sequence_id());
            }
            break;
        }
        case PacketType_StartDraw: {
            auto payload = packet->payload_as<StartDrawPayload>();
            canvas_.BeginDraw(payload->uid(), payload->sequence_id(), payload->color());
            break;
        }
        case PacketType_EndDraw: {
            auto payload = packet->payload_as<EndDrawPayload>();
            canvas_.EndDraw(payload->uid(), payload->sequence_id());
            break;
        }
        case PacketType_DrawPoints: {
            auto payload = packet->payload_as<DrawPointsPayload>();
            auto points_vector = payload->points();
            if (points_vector->size() == 0) {
                break;
            }
            std::vector<Point> points(points_vector->size());
            memcpy(points.data(), points_vector->data(), sizeof(Vec2) * points_vector->size());
            canvas_.DrawPoints(payload->uid(), payload->sequence_id(), points);
            break;
        }
        case PacketType_DeleteBatch: {
            auto payload = packet->payload_as<DeleteBatchPayload>();
            canvas_.ClearBatch(payload->uid(), payload->sequence_id());
            break;
        }
        case PacketType_UserEnter: {
            auto payload = packet->payload_as<UserEnterPayload>();
            Log::InfoF("User %d entered with color 0x%08x\n", payload->uid(), payload->color());
            break;
        }
        case PacketType_UserLeave: {
            auto payload = packet->payload_as<UserLeavePayload>();
            Log::InfoF("User %d leaved from room\n", payload->uid());
            break;
        }
    }
}
