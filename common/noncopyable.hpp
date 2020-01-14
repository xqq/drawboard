//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_NONCOPYABLE_HPP
#define DRAWBOARD_NONCOPYABLE_HPP

class Noncopyable {
public:
    Noncopyable() = default;
    ~Noncopyable() = default;
private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete; \
    TypeName& operator=(const TypeName&) = delete;

#endif // DRAWBOARD_NONCOPYABLE_HPP
