//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_PACKET_HPP
#define DRAWBOARD_PACKET_HPP

#include <cstdint>
#include <cstddef>
#include <memory>

class Buffer;

enum PacketType {
    kFullImage = 1,
    kStartDraw,
    kEndDraw,
    kDrawPoints,
    kUserEnter,
    kUserLeave,
};

struct PacketHeader {
    uint16_t packet_type;
    uint16_t payload_length;
};

struct Packet {
    PacketHeader header;
    std::unique_ptr<uint8_t[]> payload;
};

using PacketCallback =  void(*)(Packet* packet);

size_t ParseBuffer(Buffer* buffer, PacketCallback callback);


#endif //DRAWBOARD_PACKET_HPP
