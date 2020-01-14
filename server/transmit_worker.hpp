//
// Created by magicxqq on 2020/01/14.
//

#ifndef DRAWBOARD_TRANSMIT_WORKER_HPP
#define DRAWBOARD_TRANSMIT_WORKER_HPP

#include <unordered_map>
#include "common/blocking_queue.hpp"
#include "common/threaded_worker.hpp"
#include "socket/tcp_socket.hpp"
#include "task.hpp"

// uid -> TcpSocket*
using SocketCluster = std::unordered_map<uint32_t, std::unique_ptr<TcpSocket>>;

class TransmitWorker : public ThreadedWorker {
public:
    TransmitWorker();

    virtual ~TransmitWorker() override = default;

    void SetTaskQueue(BlockingQueue<Task>& queue) {
        task_queue_ = &queue;
    }

    void SetSocketCluster(SocketCluster& cluster) {
        socket_cluster_ = &cluster;
    }
private:
    virtual void Run() override;
private:
    BlockingQueue<Task>* task_queue_;
    SocketCluster* socket_cluster_;
};

#endif //DRAWBOARD_TRANSMIT_WORKER_HPP
