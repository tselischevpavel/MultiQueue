/**
 *  IUnsubscriptable interface, to let IConsumer unsubscribe
 */

#pragma once

#include "IConsumer.h"


namespace multi_queue {

template<typename Value>
class IUnsubscriptable {
public:
    virtual void unsubscribe(IConsumer<Value>* consumer) = 0;
    virtual ~IUnsubscriptable() = default;
};

}
