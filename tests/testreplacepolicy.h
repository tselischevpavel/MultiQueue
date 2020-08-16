#pragma once

class TestReplacePolicy {
/* brief: Test queue replace policy
 */
public:
    TestReplacePolicy(){}
    bool run_test();
    bool test_overflow();
    bool test_lock();
    void assert_true(const bool);
};
