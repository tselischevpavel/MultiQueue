#pragma once


namespace multi_queue {

class TestSimple {
/** Test simple functionality of queue
 */
public:
    TestSimple(){}
    bool run_test();
    bool run_simple();
    bool run_unsubscribe();
    void assert_true(const bool);
};

}
