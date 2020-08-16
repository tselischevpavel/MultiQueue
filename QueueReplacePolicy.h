#pragma once

#include <mutex>
#include <condition_variable>

#include "IQueueProcessingPolicy.h"

template <typename Value>
class QueueReplacePolicy : public IQueueProcessingPolicy<Value> {
typedef typename Value::size_type size_type;
public:
    QueueReplacePolicy(const unsigned int max_size) : IQueueProcessingPolicy<Value>(max_size){}
    ~QueueReplacePolicy() = default;

    size_type size(std::queue<Value>& queue){
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }

    void enqueue(std::queue<Value>& queue, Value&& val){
        std::lock_guard<std::mutex> lock(mtx);
        if( this->max_size && queue.size() == this->max_size )
            queue.pop();
        queue.emplace(std::forward<Value>(val));
    }

    Value dequeue(std::queue<Value>& queue){
        // undefined behavior from queue.pop() from empty queue
        // check size not empty, we have single consumer
        std::lock_guard<std::mutex> lock(mtx);
        Value val = queue.front();
        queue.pop();
        return val;
    }

private:
    std::mutex mtx;
};
