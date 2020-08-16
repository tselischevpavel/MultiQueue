/**
 *  IQueueProcessingPolicy interface, base interface for override operations with queue
 */

#pragma once

#include <iostream> // rep
#include <queue>


namespace multi_queue {

template <typename Value>
class IQueueProcessingPolicy {
typedef typename Value::size_type size_type;
public:
    IQueueProcessingPolicy(const unsigned int max_size) : max_size(max_size) {}
    virtual ~IQueueProcessingPolicy(){}
    virtual void enqueue(std::queue<Value>& queue, Value&& val) = 0;
    virtual Value dequeue(std::queue<Value>& queue) = 0;
    virtual size_type size(std::queue<Value>& queue) = 0;

protected:
    bool is_full(const std::queue<Value>& queue) const { return !max_size || (max_size == queue.size()); }
    bool is_empty(const std::queue<Value>& queue) const { return queue.empty(); }
    unsigned int max_size;
};

}
