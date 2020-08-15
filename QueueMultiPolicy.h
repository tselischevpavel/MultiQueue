#pragma once

#include <mutex>
#include <condition_variable>

#include "IQueueProcessingPolicy.h"

template <typename Value>
class QueueMultiPolicy : public IQueueProcessingPolicy<Value> {
typedef typename Value::size_type size_type;
public:
    QueueMultiPolicy(const unsigned int max_size) : IQueueProcessingPolicy<Value>(max_size){}
    ~QueueMultiPolicy() = default;

    size_type size(std::queue<Value>& queue){
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }

    void enqueue(std::queue<Value>& queue, Value&& val){
        std::unique_lock<std::mutex> lock(mtx);
        size_check.wait(lock, [this, &queue](){return !this->max_size || (queue.size() < this->max_size);});
        queue.emplace(std::forward<Value>(val));
        lock.unlock();
        size_check.notify_all();
    }

    Value dequeue(std::queue<Value>& queue){
        std::unique_lock<std::mutex> lock(mtx);
        size_check.wait(lock, [&queue](){return queue.size() > 0;});
        Value val = queue.front();
        queue.pop();
        lock.unlock();
        size_check.notify_all();
        return val;
    }

private:
    std::mutex mtx;
    std::condition_variable size_check;
};
