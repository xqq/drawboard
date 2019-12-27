//
// @author magicxqq <xqq@xqq.im>
//

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "buffer.hpp"

Buffer::Buffer(size_t size) {
    size_ = size;
    buffer_ = reinterpret_cast<uint8_t*>(malloc(size));
    memset(buffer_, 0, size);

    read_pointer_ = write_pointer_ = buffer_;
    end_pointer_ = buffer_ + size;
}

Buffer::~Buffer() {
    free(buffer_);
}

bool Buffer::Read(uint8_t* out_buf, size_t length) {
    if (length > GetReadableLength()) {
        return false;
    }

    memcpy(out_buf, read_pointer_, length);
    read_pointer_ += length;
    return true;
}

bool Buffer::Write(const uint8_t* buf, size_t length) {
    size_t remain = end_pointer_ - write_pointer_;

    if (length > size_) {
        fprintf(stderr, "Buffer::Write(): space not enough for %zu bytes", length);
        return false;
    } else if (length > remain) {  // if space is not enough
        // move existing data to the front of buffer
        size_t move_length = GetReadableLength();
        memmove(buffer_, read_pointer_, move_length);
        read_pointer_ = buffer_;
        write_pointer_ = read_pointer_ + move_length;
    }

    memcpy(write_pointer_, buf, length);
    write_pointer_ += length;
    return true;
}

size_t Buffer::GetReadableLength() {
    ptrdiff_t diff = write_pointer_ - read_pointer_;
    return (size_t)diff;
}
