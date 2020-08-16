/**
 *  IQueuePolicyFactory interface, base interface for build policies
 */

#pragma once

#include "IQueueProcessingPolicy.h"


namespace multi_queue {

template <typename Value>
class IQueuePolicyFactory{
public:
    IQueuePolicyFactory() = default;
    virtual ~IQueuePolicyFactory() = default;
    virtual IQueueProcessingPolicy<Value>* build_policy(unsigned int max_size=0) const = 0;
};

}
