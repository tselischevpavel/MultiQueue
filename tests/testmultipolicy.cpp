#include "testmultipolicy.h"

#include <set>
#include <iostream>
#include <thread>
#include <future>

#include "../IConsumer.h"
#include "../IProducer.h"
#include "../QueueManager.h"

template <typename Value>
class ConsumerTestMulti : public IConsumer<Value>{

public:
    ConsumerTestMulti(){}

    virtual ~ConsumerTestMulti(){}

    Value consume(){
        // TODO nullptr qh
        Value val = this->queue_handler->dequeue();
        std::cout << val << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        return val;
    }
};

template <typename Key, typename Value>
class Producer : public IProducer<Key, Value>{

public:
    Producer(){}
    virtual ~Producer(){}
};


void TestMultiPolicy::assert_true(const bool result){
    if( result )
        std::cout << "TestMultiPolicy is OK" << std::endl;
    else
        std::cout << "TestMultiPolicy Failed" << std::endl;
}

void produce_for_multi_policy_test(
        std::shared_ptr<QueueHandler<std::string>> qh,
        std::unique_ptr<Producer<int, std::string>> producer,
        int num)
{
    producer->produce(qh, "consumed " + std::to_string(num));
    std::cout << "produced " + std::to_string(num) << std::endl;
    std::cout << "size " + std::to_string(qh->size()) << std::endl;
}

void consume_for_multi_policy_test(
        std::promise<bool>&& res,
        ConsumerTestMulti<std::string>* consumer)
{
    std::set<std::string> results;
    std::set<std::string> compare = {
        "consumed 1", "consumed 2", "consumed 3", "consumed 4", "consumed 5",
        "consumed 6", "consumed 7", "consumed 8", "consumed 9", "consumed 10"
    };
    for( int i=0; i<10; ++i ){
        std::string num = consumer->consume();
        results.insert(num);
    }
    res.set_value(results==compare);
}

bool TestMultiPolicy::run_test(){
    bool result = test_overflow();
    result &= test_lock();
    assert_true(result);
    return result;
}

bool TestMultiPolicy::test_overflow(){
    // test policy with waiting for produce/consume from different threads
    std::promise<bool> res;
    auto fut = res.get_future();

    ConsumerTestMulti<std::string>* consumer1 = new ConsumerTestMulti<std::string>();
    std::shared_ptr<QueueHandler<std::string>> qh = QueueManager<int, std::string>::Instance().create_queue(100, 5);
    QueueManager<int, std::string>::Instance().subscribe(100, consumer1);

    std::thread consumer_thread;
    std::thread producers_tread[10];
    consumer_thread = std::thread(consume_for_multi_policy_test, std::move(res), consumer1);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    for( int i=0; i<10; ++i ) {
        producers_tread[i] = std::thread(
            produce_for_multi_policy_test,
            qh, std::make_unique<Producer<int, std::string>>(),
            i+1
        );
    }

    for (int i=0; i<10; ++i) {
        producers_tread[i].join();
    }
    consumer_thread.join();
    QueueManager<int, std::string>::Instance().unsubscribe(consumer1);

    std::cout << "end" << std::endl;
    return fut.get();
}

bool TestMultiPolicy::test_lock(){
    // start with producers thread, consumer starts after size lock
    std::promise<bool> res;
    auto fut = res.get_future();

    ConsumerTestMulti<std::string>* consumer1 = new ConsumerTestMulti<std::string>();
    std::shared_ptr<QueueHandler<std::string>> qh = QueueManager<int, std::string>::Instance().create_queue(100, 3);
    QueueManager<int, std::string>::Instance().subscribe(100, consumer1);

    std::thread consumer_thread;
    std::thread producers_tread[10];
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    for( int i=0; i<10; ++i ) {
        producers_tread[i] = std::thread(
            produce_for_multi_policy_test,qh,
            std::make_unique<Producer<int, std::string>>(),
            i+1
        );
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    consumer_thread = std::thread(consume_for_multi_policy_test, std::move(res), consumer1);

    for (int i=0; i<10; ++i) {
        producers_tread[i].join();
    }
    consumer_thread.join();
    QueueManager<int, std::string>::Instance().unsubscribe(consumer1);

    return fut.get();
}
