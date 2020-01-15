//
// Created by magicxqq on 2020/01/14.
//

#ifndef DRAWBOARD_TASK_HPP
#define DRAWBOARD_TASK_HPP

#include <cstdint>
#include <vector>

enum class TaskType : int {
    kPrivateSend = 0,
    kBroadcastSend,
    kPVoid
};

struct Task {
public:
    TaskType type;

    std::vector<uint8_t> buffer;

    union {
        TcpSocket* socket;
        void* pvoid;
    };
public:
    Task(TaskType t, std::vector<uint8_t>&& v) : type(t), buffer(std::move(v)), pvoid(nullptr) {}

    Task(TaskType t, std::vector<uint8_t>&& v, TcpSocket* s) : type(t), buffer(std::move(v)), socket(s) {}

    Task(TaskType t, std::vector<uint8_t>&& v, void* p) : type(t), buffer(std::move(v)), pvoid(p) {}

    friend bool operator==(const Task& lhs, TaskType rhs) {
        return lhs.type == rhs;
    }

    friend bool operator!=(const Task& lhs, TaskType rhs) {
        return !(lhs == rhs);
    }
};

#endif //DRAWBOARD_TASK_HPP
