#include "testreplacepolicy.h"

#include <set>
#include <iostream>
#include <thread>
#include <future>

#include "../IConsumer.h"
#include "../IProducer.h"
#include "../QueueManager.h"
#include "../QueueReplacePolicyFactory.h"

template <typename Value>
class ConsumerTestReplace : public IConsumer<Value>{

public:
    ConsumerTestReplace(){}

    virtual ~ConsumerTestReplace(){}

    Value consume(){
        Value val = this->queue_handler->dequeue();
        std::cout << val << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return val;
    }

    bool has_more() const {
        return this->queue_handler->size() > 0;
    }
};

template <typename Key, typename Value>
class Producer : public IProducer<Key, Value>{

public:
    Producer(){}
    virtual ~Producer(){}
};


void TestReplacePolicy::assert_true(const bool result){
    if( result )
        std::cout << "TestReplacePolicy is OK" << std::endl;
    else
        std::cout << "TestReplacePolicy Failed" << std::endl;
}

bool TestReplacePolicy::run_test(){
    bool result = test_overflow();
    result &= test_lock();
    assert_true(result);
    return result;
}

bool TestReplacePolicy::test_overflow(){
    // start with producer thread, replace old values after 5
    QueueReplacePolicyFactory<std::string> factory = QueueReplacePolicyFactory<std::string>();
    ConsumerTestReplace<std::string> consumer1 = ConsumerTestReplace<std::string>();
    Producer<int, std::string>* producer1 = new Producer<int, std::string>();
    auto qh = QueueManager<int, std::string>::Instance().create_queue(100, 5, factory);
    QueueManager<int, std::string>::Instance().subscribe(100, &consumer1);

    for(int num = 1; num < 11; ++num){
        producer1->produce(qh, "consumed " + std::to_string(num));
    }

    std::vector<std::string> results;
    std::vector<std::string> compare = {
        "consumed 6", "consumed 7", "consumed 8", "consumed 9", "consumed 10"
    };

    while( consumer1.has_more() ){
        results.push_back(consumer1.consume());
    }
    std::cout << "end" << std::endl;
    return results == compare;
}


void produce_for_replace_test(
        std::shared_ptr<QueueHandler<std::string>> qh,
        std::unique_ptr<Producer<int, std::string>> producer,
        int num)
{
    producer->produce(qh, "consumed " + std::to_string(num));
    std::cout << "size " + std::to_string(qh->size()) << std::endl;
}


bool TestReplacePolicy::test_lock(){
    // start with producers thread, consumer starts after size lock
    QueueReplacePolicyFactory<std::string> factory = QueueReplacePolicyFactory<std::string>();
    ConsumerTestReplace<std::string>* consumer1 = new ConsumerTestReplace<std::string>();
    auto qh = QueueManager<int, std::string>::Instance().create_queue(100, 10, factory);
    QueueManager<int, std::string>::Instance().subscribe(100, consumer1);

    std::thread consumer_thread;
    std::thread producers_tread[10];
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    for( int i=0; i<10; ++i ) {
        producers_tread[i] = std::thread(
            produce_for_replace_test, qh,
            std::make_unique<Producer<int, std::string>>(),
            i+1
        );
    }

    for (int i=0; i<10; ++i) {
        producers_tread[i].join();
    }

    std::set<std::string> results;
    std::set<std::string> compare = {
        "consumed 1", "consumed 2", "consumed 3", "consumed 4", "consumed 5",
        "consumed 6", "consumed 7", "consumed 8", "consumed 9", "consumed 10"
    };
    for( int i=0; i<10; ++i ){
        std::string num = consumer1->consume();
        results.insert(num);
    }
    QueueManager<int, std::string>::Instance().unsubscribe(consumer1);
    return results == compare;
}
