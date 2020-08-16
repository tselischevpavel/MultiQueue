#pragma once

#include "IConsumer.h"

template<typename Value>
class IUnsubscriptable {
public:
    virtual void unsubscribe(IConsumer<Value>* consumer) = 0;
    virtual ~IUnsubscriptable() = default;
};
