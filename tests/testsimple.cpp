#include "testsimple.h"

#include <iostream>
#include <thread>

#include "../IConsumer.h"
#include "../IProducer.h"
#include "../QueueManager.h"


namespace multi_queue {

template <typename Value>
class ConsumerTestSimple : public IConsumer<Value>{

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
class ProducerTestSimple : public IProducer<Key, Value>{

public:
    ProducerTestSimple(){}

    virtual ~ProducerTestSimple(){}
};

void TestSimple::assert_true(const bool result){
    if( result )
        std::cout << "TestSimple is OK" << std::endl;
    else
        std::cout << "TestSimple Failed" << std::endl;
}

bool TestSimple::run_test(){
    bool result = run_simple();
    result &= run_unsubscribe();
    assert_true(result);
    return result;
}


bool TestSimple::run_simple(){
    bool result = true;

    ConsumerTestSimple<std::string> consumer1 = ConsumerTestSimple<std::string>();
    ProducerTestSimple<int, std::string> producer1 = ProducerTestSimple<int, std::string>();

    std::shared_ptr<QueueHandler<std::string>> qh = QueueManager<int, std::string>::Instance().create_queue(100);
    QueueManager<int, std::string>::Instance().subscribe(100, &consumer1);

    producer1.produce(100, "1");
    producer1.produce(100, "2");
    producer1.produce(100, "3");
    producer1.produce(qh, "4");

    result &= qh->size() == 4;

    consumer1.consume();
    consumer1.consume();
    consumer1.consume();
    consumer1.consume();

    result &= qh->size() == 0;
    return result;
}

bool TestSimple::run_unsubscribe(){
    bool result = true;
    ConsumerTestSimple<std::string>* consumer1 = new ConsumerTestSimple<std::string>();
    QueueManager<int, std::string>::Instance().create_queue(100);
    QueueManager<int, std::string>::Instance().subscribe(100, consumer1);
    delete consumer1; // queue with key was deleted

    QueueManager<int, std::string>::Instance().create_queue(100);
    QueueManager<int, std::string>::Instance().delete_queue(100);
    return result;
}

}
