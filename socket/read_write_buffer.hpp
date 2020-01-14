//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_READ_WRITE_BUFFER_HPP
#define DRAWBOARD_READ_WRITE_BUFFER_HPP

#include <cstddef>
#include <cstdint>
#include "noncopyable.hpp"

class ReadWriteBuffer {
public:
    explicit ReadWriteBuffer(size_t size);
    ~ReadWriteBuffer();
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
    DISALLOW_COPY_AND_ASSIGN(ReadWriteBuffer);
};


#endif //DRAWBOARD_READ_WRITE_BUFFER_HPP
