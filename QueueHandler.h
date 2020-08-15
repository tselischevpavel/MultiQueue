#pragma once

#include <queue>
#include <memory>

#include "IQueueProcessingPolicy.h"
#include "QueueMultiPolicy.h"

template<typename Value>
class QueueHandler{
typedef typename Value::size_type size_type;

public:
    QueueHandler(IQueueProcessingPolicy<Value>* policy) {
        processing_policy = policy;
        queue = std::queue<Value>();
    }
    virtual ~QueueHandler(){
        delete processing_policy;
    }
    QueueHandler(const QueueHandler&) = delete;
    QueueHandler& operator=(const QueueHandler&) = delete;

    size_type size(){
        return processing_policy->size(queue);
    }

    Value dequeue(){
        return processing_policy->dequeue(queue);
    }

    void enqueue(Value&& val){
        processing_policy->enqueue(queue, std::forward<Value>(val));
    }

private:
    std::queue<Value> queue;
    IQueueProcessingPolicy<Value>* processing_policy;
};
