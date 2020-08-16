#pragma once


namespace multi_queue {

class TestMultiPolicy {
/** Test queue multi thread policy
 */
public:
    TestMultiPolicy(){}
    bool run_test();
    bool test_overflow();
    bool test_lock();
    void assert_true(const bool);
};

}
