//
// @author magicxqq <xqq@xqq.im>
//

#include "socket/buffer.hpp"
#include "protocol/protocol_generated.h"
#include "packet.hpp"

size_t ParseBuffer(Buffer* buffer, PacketCallback callback) {
    size_t packet_count = 0;
    size_t readable = buffer->GetReadableLength();

    while (readable > sizeof(PacketHeader)) {
        PacketHeader header;
        buffer->Read(reinterpret_cast<uint8_t*>(&header), sizeof(header));

        readable = buffer->GetReadableLength();
        if (readable < header.payload_length()) {
            break;
        }

        size_t packet_size = sizeof(header) + header.payload_length();
        std::unique_ptr<uint8_t[]> packet_buffer(new uint8_t[packet_size]);
        memset(packet_buffer.get(), 0, packet_size);
        memcpy(packet_buffer.get(), &header, sizeof(header));

        buffer->Read(packet_buffer.get() + sizeof(header), header.payload_length());
        readable = buffer->GetReadableLength();

        const Packet* packet = GetPacket(packet_buffer.get());

        callback(packet);
        packet_count++;
    }

    return packet_count;
}
