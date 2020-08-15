#pragma once

#include "IQueuePolicyFactory.h"
#include "QueueMultiPolicy.h"

template<typename Value>
class QueueMultiPolicyFactory : public IQueuePolicyFactory<Value> {
public:
    QueueMultiPolicyFactory() = default;
    ~QueueMultiPolicyFactory() = default;

    IQueueProcessingPolicy<Value>* build_policy(unsigned int max_size = 0) const {
        return new QueueMultiPolicy<Value>(max_size);
    }
};
