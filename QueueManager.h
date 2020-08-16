/**
 *  MultiQueue manager class, mediate for work with query handlers
 *  basic usage create/delete queues and manage consumers subscriptions
 */

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

#include "IUnsubscriptable.h"


namespace multi_queue {

template<typename Key, typename Value, typename Hash=std::hash<Key>, typename KeyEqual=std::equal_to<Key>>
class QueueManager : public IUnsubscriptable<Value> {

public:
    static QueueManager<Key, Value, Hash, KeyEqual>& Instance(){
        static QueueManager<Key, Value, Hash, KeyEqual> instance;
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
            it = queues.emplace(std::make_pair(key, std::make_shared<QueueHandler<Value>>(
                                                   policy, this))).first;
        }
        else{
            throw QueueExistsException();
        }
        return it->second;
    }

    void delete_queue(const Key& key) {
        const std::lock_guard<std::mutex> lock(mtx);
        auto it = queues.find(key);
        if( it == queues.end() ){
            throw QueueNotExistsException();
        }
        for( auto consumer_it = consumers.begin(); consumer_it != consumers.end(); ++consumer_it ){
            if( consumer_it->second == key )
                throw QueueRunningException();
        }
        queues.erase(it);
    }

    void subscribe(const Key& key, IConsumer<Value>* consumer){
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

    void unsubscribe(IConsumer<Value>* consumer){
        const std::lock_guard<std::mutex> lock(mtx);
        consumer->reset_queue();
        auto it = consumers.find(consumer);
        if( it == consumers.end() ){
            return;
        }
        consumers.erase(it);
        if( remove_on_unsubscribe ){
            queues.erase(it->second);
        }
    }

    std::shared_ptr<QueueHandler<Value>> get_handler(const Key& key){
        const std::lock_guard<std::mutex> lock(mtx);
        auto queue = queues.find(key);
        if( queue == queues.end() ){
            throw QueueNotExistsException();
        }
        return queue->second;
    }

    void set_manual_remove(){
        remove_on_unsubscribe = false;
    }

    void set_auto_remove(){
        remove_on_unsubscribe = true;
    }

private:
    QueueManager() {
        remove_on_unsubscribe = true;
        queues = std::unordered_map<Key, std::shared_ptr<QueueHandler<Value>>, Hash, KeyEqual>();
        consumers = std::unordered_map<IConsumer<Value>*, Key>();
    }
    ~QueueManager() = default;

    QueueManager(const QueueManager&) = delete;
    QueueManager& operator=(const QueueManager&) = delete;

    bool remove_on_unsubscribe; // leave the queue after unsubscribe by consumer
    std::unordered_map<Key, std::shared_ptr<QueueHandler<Value>>, Hash, KeyEqual> queues;
    std::unordered_map<IConsumer<Value>*, Key> consumers;
    std::mutex mtx;
};

}
