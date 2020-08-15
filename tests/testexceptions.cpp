#include "testexceptions.h"

#include "../IConsumer.h"
#include "../IProducer.h"
#include "../QueueManager.h"
#include "../QueueExceptions.h"

template <typename Key, typename Value>
class Consumer : public IConsumer<Key, Value>{

public:
    Consumer(){}

    virtual ~Consumer(){}

    Value consume(){
        Value val = this->queue_handler->dequeue();
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

void TestExceptions::assert_true(const bool result){
    if( result )
        std::cout << "TestExceptions is OK" << std::endl;
    else
        std::cout << "TestExceptions Failed" << std::endl;
}

void TestExceptions::run_test()
{
    bool result = true;
    if( !run_test_subscribe() ){
        std::cout << "run_test_subscribe - failed" << std::endl;
        result = false;
    }
    if( !run_test_create_duplicate() ){
        std::cout << "run_test_create_duplicate - failed" << std::endl;
        result = false;
    }
    if( !run_test_get_handler() ){
        std::cout << "run_test_get_handler - failed" << std::endl;
    result = false;
    }
    assert_true(result);
}

bool TestExceptions::run_test_subscribe()
{
    bool result = true;
    Consumer<int, std::string>* consumer1 = new Consumer<int, std::string>();
    //case 1
    try {
        result = false;
        QueueManager<int, std::string>::Instance().subscribe(100, consumer1);
    } catch (const QueueNotExistsException&) {
        result = true;
    }
    if( !result )
        return false;
    //case 2
    try {
        result = false;
        QueueManager<int, std::string>::Instance().create_queue(100);
        QueueManager<int, std::string>::Instance().create_queue(200);
        QueueManager<int, std::string>::Instance().subscribe(100, consumer1);
        QueueManager<int, std::string>::Instance().subscribe(200, consumer1);
    } catch (const QueueDuplicateConsumerException&) {
        QueueManager<int, std::string>::Instance().unsubscribe(consumer1);
        QueueManager<int, std::string>::Instance().subscribe(200, consumer1);
        QueueManager<int, std::string>::Instance().unsubscribe(consumer1);
        result = true;
    }
    return result;
}

bool TestExceptions::run_test_create_duplicate()
{
    Consumer<int, std::string>* consumer1 = new Consumer<int, std::string>();
    try {
        QueueManager<int, std::string>::Instance().create_queue(100);
        QueueManager<int, std::string>::Instance().create_queue(100);
    } catch (const QueueExistsException&) {
        // removing queue
        QueueManager<int, std::string>::Instance().subscribe(100, consumer1);
        QueueManager<int, std::string>::Instance().unsubscribe(consumer1);
        return true;
    }
    return false;
}

bool TestExceptions::run_test_get_handler()
{
    try {
        QueueManager<int, std::string>::Instance().get_handler(200);
    } catch (const QueueNotExistsException&) {
        return true;
    }
    return false;
}
