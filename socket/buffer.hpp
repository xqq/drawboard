//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_BUFFER_HPP
#define DRAWBOARD_BUFFER_HPP

#include <cstddef>
#include <cstdint>
#include "noncopyable.hpp"

class Buffer {
public:
    explicit Buffer(size_t size);
    ~Buffer();
    bool Read(uint8_t* out_buf, size_t length);
    bool Write(const uint8_t* buf, size_t length);
    size_t GetReadableLength();
private:
    size_t size_;
    uint8_t* buffer_;
    uint8_t* read_pointer_;
    uint8_t* write_pointer_;
    uint8_t* end_pointer_;
private:
    DISALLOW_COPY_AND_ASSIGN(Buffer);
};


#endif // DRAWBOARD_BUFFER_HPP
