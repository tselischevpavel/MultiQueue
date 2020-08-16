cmake_minimum_required(VERSION 3.5)

project(multi_queue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(
    multi_queue
    main.cpp
    IConsumer.h
    IProducer.h
    IQueueProcessingPolicy.h
    IQueuePolicyFactory.h
    IUnsubscriptable.h

    QueueManager.h
    QueueHandler.h
    QueueExceptions.h
    QueueMultiPolicy.h
    QueueReplacePolicy.h
    QueueMultiPolicyFactory.h
    QueueReplacePolicyFactory.h

    tests/cases.h
    tests/testsimple.cpp
    tests/testsimple.h
    tests/testexceptions.cpp
    tests/testexceptions.h
    tests/testmultipolicy.cpp
    tests/testmultipolicy.h
    tests/testreplacepolicy.cpp
    tests/testreplacepolicy.h
)
