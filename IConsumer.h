#pragma once

#include <memory>

#include "QueueHandler.h"


template <typename Value>

class IConsumer {
    template<typename KeyM, typename ValueM, typename HashM, typename KeyEqualM>
    friend class QueueManager;

public:
    virtual ~IConsumer(){
        unsubscribe();
    }

    virtual Value consume() = 0;

    void unsubscribe(){
        if( queue_handler != nullptr )
            queue_handler->unsubscribe(this);
    }

protected:
    std::shared_ptr<QueueHandler<Value>> queue_handler;

private:
    // can be set/unset only by QueueManager
    virtual void set_queue(std::shared_ptr<QueueHandler<Value>> qh) final {
        queue_handler = qh;
    }

    virtual void reset_queue() final {
        queue_handler.reset();
    }
};
