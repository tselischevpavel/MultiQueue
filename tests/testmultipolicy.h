#pragma once

class TestMultiPolicy {
/* brief: Test queue multi thread policy
 */
public:
    TestMultiPolicy(){}
    bool run_test();
    bool test_overflow();
    bool test_lock();
    void assert_true(const bool);
};
