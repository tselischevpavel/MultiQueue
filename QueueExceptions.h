/**
 *  MultiQueue exceptions classes
 */

#pragma once

#include <iostream>
#include <exception>


namespace multi_queue {

class QueueExistsException : public std::exception
{
public:
    const char* what () const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
        return "Queue with same key already exists!";
    }
};

class QueueNotExistsException : public std::exception
{
public:
    const char* what () const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
        return "Queue with key doesn't exist!";
    }
};

class QueueDuplicateConsumerException : public std::exception
{
public:
    const char* what () const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
        return "Consumer already subscribed to another queue!";
    }
};

class QueueRunningException : public std::exception
{
public:
    const char* what () const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
        return "Queue still using by consumer!";
    }
};

}
