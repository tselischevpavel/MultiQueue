/**
 *  MultiQueue replace policy factory, build QueueReplacePolicy
 */

#pragma once

#include "IQueuePolicyFactory.h"
#include "QueueReplacePolicy.h"


namespace multi_queue {

template<typename Value>
class QueueReplacePolicyFactory : public IQueuePolicyFactory<Value> {
public:
    QueueReplacePolicyFactory() = default;
    ~QueueReplacePolicyFactory() = default;

    IQueueProcessingPolicy<Value>* build_policy(unsigned int max_size = 0) const {
        return new QueueReplacePolicy<Value>(max_size);
    }
};

}
