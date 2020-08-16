# MultiQueue
Library that implements simple queue functionality for single consumer and multiple producers in multi-threading environment.

## Basic usage
All you need just add a new queue, after you can start producing messages into it with producer and take them out with consumer.
For example:
```
auto consumer = MyConsumer<std::string>();
auto producer = MyProducer<int, std::string>();

QueueManager<int, std::string>::Instance().create_queue(100);
QueueManager<int, std::string>::Instance().subscribe(100, &consumer);

producer.produce(100, "1");
std::string val = consumer.consume();
```
Then you done with queue just unsubscribe from it:
```
QueueManager<int, std::string>::Instance().unsubscribe(&consumer);
// or
consumer.unsubscribe();
// or automatically when consumer's destructor called
```

### QueueManager
Provide basic functions to get queue:
```
// create queue and handle it in map by key
std::shared_ptr<QueueHandler<Value>> create_queue(
            const Key& key,
            unsigned int max_size=0,
            const IQueuePolicyFactory<Value>& factory = QueueMultiPolicyFactory<Value>())

// manual delete queue, can be used if no body subscribed to the queue
void delete_queue(const Key& key)

// get queue handler, can be used to produce messages directly into queue
std::shared_ptr<QueueHandler<Value>> get_handler(const Key& key)

// subscribe consumer for start processing messages
void subscribe(const Key& key, IConsumer<Value>* consumer)

// unsubscribe consumer to finish work with queue
void unsubscribe(IConsumer<Value>* consumer)
```

By default queue lifetime ends with unsubscribe of consumer, it can be changed by calling QueueManager::set_manual_remove(), after that queue should be deleted manually with QueueManager::delete_queue(const Key&), it can be switched back with QueueManager::set_auto_remove.

### Producer class
Should implement IProducer<Key, Value> interface with two produce functions:  
`void produce(const Key&, Value&&)` - used QueueManager to choose QueueHandler  
`void produce(std::shared_ptr<QueueHandler<Value>>, Value&&)` - used QueueHandler directly, that can be get from QueueManager

### Consumer class
Should implement IConsumer<Value> interface with consume function:  
`Value consume()` - used to get next Value from the Queue, can raise an exception if Consumer not subscribed

### Queue processing policy

To define new policy:
1) IQueuePolicyFactory<Value> should be implemented with `IQueueProcessingPolicy<Value>* build_policy(unsigned int max_size=0) const` method which returns new policy class;
2) IQueueProcessingPolicy<Value> should be implemented with following methods:
```
void enqueue(std::queue<Value>&, Value&&)
Value dequeue(std::queue<Value>&)
size_type size(std::queue<Value>&)
```
3) create queue using new factory:
```
MyPolicyFactory<std::string> factory = MyPolicyFactory<std::string>();
QueueManager<int, std::string>::Instance().create_queue(100, 10, factory);
```

For example look at [QueueMultiPolicy.h](QueueMultiPolicy.h) or tests.