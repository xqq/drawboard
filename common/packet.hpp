//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_PACKET_HPP
#define DRAWBOARD_PACKET_HPP

#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>
#include <functional>
#include "protocol/protocol_generated.h"

class ReadWriteBuffer;

struct Packet {
    PacketHeader header;
    std::vector<uint8_t> payload_buffer;
    const PacketPayload* payload;
};

using PacketCallback = std::function<void(Packet&& packet)>;

size_t ParseBuffer(ReadWriteBuffer* buffer, PacketCallback callback);


#endif //DRAWBOARD_PACKET_HPP
