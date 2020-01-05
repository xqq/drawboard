//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_PROTOCOL_HPP
#define DRAWBOARD_PROTOCOL_HPP

#include <cstdint>

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
} __attribute__((packed));

using PacketCallback =  void(*)(Packet* packet);

size_t ParseBuffer(Buffer* buffer, PacketCallback callback);


#endif // DRAWBOARD_PROTOCOL_HPP
