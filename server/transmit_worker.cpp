//
// Created by magicxqq on 2020/01/14.
//

#include "transmit_worker.hpp"

TransmitWorker::TransmitWorker() :
        task_queue_(nullptr), socket_cluster_(nullptr), ThreadedWorker("Server Transmit Worker Thread") {

}

void TransmitWorker::Run() {
    while (true) {
        task_queue_->Poll();
        if (task_queue_->IsExit()) { // immediate-exit flag
            return;
        }

        Task msg = task_queue_->Dequeue();

        if (msg == TaskType::kPrivateSend) {
            msg.socket->Send(msg.buffer.data(), msg.buffer.size());
        } else if (msg == TaskType::kBroadcastSend) {
            for (auto iter : *socket_cluster_) {
                TcpSocket* socket = iter.second;
                socket->Send(msg.buffer.data(), msg.buffer.size());
            }
        }
    }
}
