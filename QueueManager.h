#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>

#include "IConsumer.h"
#include "QueueHandler.h"
#include "QueueExceptions.h"
#include "IQueuePolicyFactory.h"
#include "QueueMultiPolicyFactory.h"


template<typename Key, typename Value, typename Hash=std::hash<Key>, typename KeyEqual=std::equal_to<Key>>
class QueueManager{

public:
    static QueueManager<Key, Value>& Instance(){
        static QueueManager<Key, Value> instance;
        return instance;
    }

    std::shared_ptr<QueueHandler<Value>> create_queue(
            const Key& key,
            unsigned int max_size=0,
            const IQueuePolicyFactory<Value>& factory = QueueMultiPolicyFactory<Value>())
    {
        const std::lock_guard<std::mutex> lock(mtx);
        auto it = queues.find(key);
        if( it == queues.end() ){
            IQueueProcessingPolicy<Value>* policy = factory.build_policy(max_size);
            it = queues.emplace(std::make_pair(key, std::make_shared<QueueHandler<Value>>(policy))).first;
        }
        else{
            throw QueueExistsException();
        }
        return it->second;
    }

    void subscribe(const Key& key, IConsumer<Key, Value>* consumer){
        const std::lock_guard<std::mutex> lock(mtx);
        auto queue = queues.find(key);
        if( queue == queues.end() ){
            throw QueueNotExistsException();
        }
        auto it = consumers.find(consumer);
        if( it != consumers.end() ){
            throw QueueDuplicateConsumerException();
        }
        auto insert_result = consumers.emplace(std::make_pair(consumer, key));
        consumer->set_queue(queue->second);
    }

    void unsubscribe(IConsumer<Key, Value>* consumer){
        const std::lock_guard<std::mutex> lock(mtx);
        auto it = consumers.find(consumer);
        if( it == consumers.end() ){
            consumer->reset_queue();
            return;
        }
        consumer->reset_queue();
        queues.erase(it->second);
        consumers.erase(it);
    }

    std::shared_ptr<QueueHandler<Value>> get_handler(const Key& key){
        const std::lock_guard<std::mutex> lock(mtx);
        auto queue = queues.find(key);
        if( queue == queues.end() ){
            throw QueueNotExistsException();
        }
        return queue->second;
    }

private:
    QueueManager() {
        queues = std::unordered_map<Key, std::shared_ptr<QueueHandler<Value>>, Hash, KeyEqual>();
        consumers = std::unordered_map<IConsumer<Key, Value>*, Key>();
    }
    ~QueueManager() = default;

    QueueManager(const QueueManager&) = delete;
    QueueManager& operator=(const QueueManager&) = delete;

    std::unordered_map<Key, std::shared_ptr<QueueHandler<Value>>, Hash, KeyEqual> queues;
    std::unordered_map<IConsumer<Key, Value>*, Key> consumers;
    std::mutex mtx;
};
