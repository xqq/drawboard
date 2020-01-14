//
// Created by magicxqq on 2020/01/14.
//

#ifndef DRAWBOARD_THREADED_WORKER_HPP
#define DRAWBOARD_THREADED_WORKER_HPP

#include <thread>
#include <string>
#include "noncopyable.hpp"

class ThreadedWorker {
public:
    void Start();
protected:
    ThreadedWorker() = default;

    explicit ThreadedWorker(const std::string& thread_name) : thread_name_(thread_name) {}

    virtual ~ThreadedWorker();

    virtual void Run() = 0;
private:
    std::thread thread_;
    std::string thread_name_;
private:
    DISALLOW_COPY_AND_ASSIGN(ThreadedWorker);
};


#endif //DRAWBOARD_THREADED_WORKER_HPP
