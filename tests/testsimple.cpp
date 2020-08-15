#include "testsimple.h"

#include <iostream>
#include <thread>

#include "../IConsumer.h"
#include "../IProducer.h"
#include "../QueueManager.h"

template <typename Key, typename Value>
class ConsumerTestSimple : public IConsumer<Key, Value>{

public:
    ConsumerTestSimple(){}

    virtual ~ConsumerTestSimple(){}

    Value consume(){
        Value val = this->queue_handler->dequeue();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << val << std::endl;
        return val;
    }
};

template <typename Key, typename Value>
class Producer : public IProducer<Key, Value>{

public:
    Producer(){}

    virtual ~Producer(){}
};

void TestSimple::assert_true(const bool result){
    if( result )
        std::cout << "TestSimple is OK" << std::endl;
    else
        std::cout << "TestSimple Failed" << std::endl;
}

void TestSimple::run_test(){
    bool result = true;

    ConsumerTestSimple<int, std::string>* consumer1 = new ConsumerTestSimple<int, std::string>();
    Producer<int, std::string>* producer1 = new Producer<int, std::string>();

    std::shared_ptr<QueueHandler<std::string>> qh = QueueManager<int, std::string>::Instance().create_queue(100);
    QueueManager<int, std::string>::Instance().subscribe(100, consumer1);

    producer1->produce(100, "1");
    producer1->produce(100, "2");
    producer1->produce(100, "3");
    producer1->produce(qh, "4");

    result &= qh->size() == 4;

    consumer1->consume();
    consumer1->consume();
    consumer1->consume();
    consumer1->consume();

    result &= qh->size() == 0;
    QueueManager<int, std::string>::Instance().unsubscribe(consumer1);

    assert_true(result);
}
