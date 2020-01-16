//
// Created by magicxqq on 2020/01/14.
//

#ifndef DRAWBOARD_BLOCKING_QUEUE_HPP
#define DRAWBOARD_BLOCKING_QUEUE_HPP

#include <deque>
#include <mutex>
#include <condition_variable>
#include "noncopyable.hpp"

template <typename T>
class BlockingQueue {
public:
    BlockingQueue() : has_size_limit_(false), max_size_(0), is_exit_(false) {}

    explicit BlockingQueue(size_t max_size) : has_size_limit_(true), max_size_(max_size), is_exit_(false) {}

    void Enqueue(const T& element) {
        std::unique_lock<std::mutex> guard(mutex_);

        if (has_size_limit_) {
            while (deque_.size() + 1 >= max_size_ && !is_exit_) {
                produce_cv_.wait(guard);
            }
        }

        deque_.push_back(element);
        consume_cv_.notify_one();
    }

    void Enqueue(T&& element) {
        std::unique_lock<std::mutex> guard(mutex_);

        if (has_size_limit_) {
            while (deque_.size() + 1 >= max_size_ && !is_exit_) {
                produce_cv_.wait(guard);
            }
        }

        deque_.push_back(std::move(element));
        consume_cv_.notify_one();
    }

    void Poll() {
        std::unique_lock<std::mutex> guard(mutex_);

        while (deque_.size() == 0 && !is_exit_) {
            consume_cv_.wait(guard);
        }
    }

    void WaitUntilEmpty() {
        std::unique_lock<std::mutex> guard(mutex_);

        while (deque_.size() > 0) {
            produce_cv_.wait(guard);
        }
    }

    T Dequeue() {
        std::lock_guard<std::mutex> guard(mutex_);

        T element = std::move(deque_.front());
        deque_.pop_front();

        if (deque_.empty()) {
            produce_cv_.notify_all();
        } else if (has_size_limit_) {
            produce_cv_.notify_one();
        }

        return std::move(element);
    }

    void NotifyExit() {
        std::lock_guard<std::mutex> guard(mutex_);
        is_exit_ = true;
        consume_cv_.notify_all();
        produce_cv_.notify_all();
    }

    bool IsExit() {
        std::lock_guard<std::mutex> guard(mutex_);
        return is_exit_;
    }

    size_t Size() {
        std::lock_guard<std::mutex> guard(mutex_);
        return deque_.size();
    }

    void Clear() {
        std::lock_guard<std::mutex> guard(mutex_);
        deque_.clear();
    }
private:
    std::deque<T> deque_;
    std::mutex mutex_;
    std::condition_variable consume_cv_;
    std::condition_variable produce_cv_;

    bool has_size_limit_;
    size_t max_size_;
    bool is_exit_;
private:
    DISALLOW_COPY_AND_ASSIGN(BlockingQueue);
};

#endif //DRAWBOARD_BLOCKING_QUEUE_HPP
