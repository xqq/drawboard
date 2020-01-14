//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_PACKET_HPP
#define DRAWBOARD_PACKET_HPP

#include <cstdint>
#include <cstddef>
#include <memory>
#include <functional>

class ReadWriteBuffer;
struct Packet;

using PacketCallback = std::function<void(const Packet* packet)>;

size_t ParseBuffer(ReadWriteBuffer* buffer, PacketCallback callback);


#endif //DRAWBOARD_PACKET_HPP
