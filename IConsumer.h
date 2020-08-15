#pragma once

#include <memory>

#include "QueueHandler.h"


template <typename Key, typename Value>

class IConsumer {
    template<typename KeyM, typename ValueM, typename HashM, typename KeyEqualM>
    friend class QueueManager;

public:
    virtual ~IConsumer() = default;

    virtual Value consume() = 0;

protected:
    std::shared_ptr<QueueHandler<Value>> queue_handler;

private:
    // can be set/unset only by QueueManager
    void set_queue(std::shared_ptr<QueueHandler<Value>> qh){
        queue_handler = qh;
    }

    void reset_queue(){
        queue_handler.reset();
    }
};
