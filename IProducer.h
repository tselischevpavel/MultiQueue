#pragma once

#include <memory>

#include "QueueHandler.h"
#include "QueueManager.h"

template <typename Key, typename Value>
class IProducer {
public:
    //produce with choosing Queue by key
    virtual void produce(const Key& key, Value&& value){
        QueueManager<Key, Value>::Instance().get_handler(key)->enqueue(std::forward<Value>(value));
    }

    //direct produce to the Queue
    virtual void produce(std::shared_ptr<QueueHandler<Value>> qh, Value&& value){
        qh->enqueue(std::forward<Value>(value));
    }

    virtual ~IProducer() = 0;
};

template <typename Key, typename Value>
IProducer<Key, Value>::~IProducer(){};
