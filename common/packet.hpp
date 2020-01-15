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

class ReadWriteBuffer;
struct Packet;

using PacketCallback = std::function<void(const Packet* packet, std::vector<uint8_t>&& packet_buffer)>;

size_t ParseBuffer(ReadWriteBuffer* buffer, PacketCallback callback);


#endif //DRAWBOARD_PACKET_HPP
