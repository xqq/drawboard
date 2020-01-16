//
// @author magicxqq <xqq@xqq.im>
//

#include "socket/read_write_buffer.hpp"
#include "protocol/protocol_generated.h"
#include "packet.hpp"

size_t ParseBuffer(ReadWriteBuffer* buffer, PacketCallback callback) {
    size_t packet_count = 0;
    size_t readable = buffer->GetReadableLength();

    while (readable > sizeof(PacketHeader)) {
        Packet packet;
        buffer->Read(reinterpret_cast<uint8_t*>(&packet.header), sizeof(packet.header));

        readable = buffer->GetReadableLength();
        if (readable < packet.header.payload_length()) {
            break;
        }

        size_t payload_length = packet.header.payload_length();

        packet.payload_buffer.resize(payload_length);
        buffer->Read(packet.payload_buffer.data(), payload_length);
        readable = buffer->GetReadableLength();

        packet.payload = GetPacketPayload(packet.payload_buffer.data());

        callback(std::move(packet));
        packet_count++;
    }

    return packet_count;
}
