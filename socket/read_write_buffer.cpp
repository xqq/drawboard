//
// @author magicxqq <xqq@xqq.im>
//

#include <cstdlib>
#include <cstring>
#include "common/log.hpp"
#include "read_write_buffer.hpp"

ReadWriteBuffer::ReadWriteBuffer(size_t size) {
    size_ = size;
    buffer_ = reinterpret_cast<uint8_t*>(malloc(size));
    memset(buffer_, 0, size);

    read_pointer_ = write_pointer_ = buffer_;
    end_pointer_ = buffer_ + size;
}

ReadWriteBuffer::~ReadWriteBuffer() {
    free(buffer_);
    buffer_ = nullptr;
}

bool ReadWriteBuffer::Read(uint8_t* out_buf, size_t length) {
    if (length > GetReadableLength()) {
        return false;
    }

    memcpy(out_buf, read_pointer_, length);
    read_pointer_ += length;
    return true;
}

bool ReadWriteBuffer::Write(const uint8_t* buf, size_t length) {
    size_t remain = end_pointer_ - write_pointer_;

    if (length > size_) {
        Log::ErrorF("ReadWriteBuffer::Write(): space not enough for %zu bytes\n", length);
        return false;
    } else if (length > remain) {  // if space is not enough
        if (length < (read_pointer_ - buffer_) + remain) {
            // move existing data to the front of buffer
            size_t move_length = GetReadableLength();
            memmove(buffer_, read_pointer_, move_length);
            read_pointer_ = buffer_;
            write_pointer_ = read_pointer_ + move_length;
        } else {
            ExpandBuffer(size_ * 2);
        }
    }

    memcpy(write_pointer_, buf, length);
    write_pointer_ += length;
    return true;
}

size_t ReadWriteBuffer::GetReadableLength() {
    ptrdiff_t diff = write_pointer_ - read_pointer_;
    return (size_t)diff;
}

void ReadWriteBuffer::RollbackReadPointer(size_t length) {
    read_pointer_ -= length;
}

void ReadWriteBuffer::ExpandBuffer(size_t size) {
    uint8_t* new_buf = reinterpret_cast<uint8_t*>(malloc(size));
    memset(new_buf, 0, size);

    memcpy(new_buf, buffer_, size_);

    read_pointer_ = new_buf + (read_pointer_ - buffer_);
    write_pointer_ = new_buf + (write_pointer_ - buffer_);
    end_pointer_ = new_buf + size;

    free(buffer_);
    buffer_ = new_buf;
    size_ = size;
}