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
        PacketHeader header;
        buffer->Read(reinterpret_cast<uint8_t*>(&header), sizeof(header));

        readable = buffer->GetReadableLength();
        if (readable < header.payload_length()) {
            break;
        }

        size_t packet_size = sizeof(header) + header.payload_length();
        std::vector<uint8_t> packet_buffer(packet_size);
        memcpy(packet_buffer.data(), &header, sizeof(header));

        buffer->Read(packet_buffer.data() + sizeof(header), header.payload_length());
        readable = buffer->GetReadableLength();

        const Packet* packet = GetPacket(packet_buffer.data());

        callback(packet, std::move(packet_buffer));
        packet_count++;
    }

    return packet_count;
}
