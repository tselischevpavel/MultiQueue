#pragma once


namespace multi_queue {

class TestExceptions {
/** Test queue exceptions
 */
public:
    TestExceptions(){}
    bool run_test();
    bool run_test_subscribe();
    bool run_test_create_duplicate();
    bool run_test_get_handler();
    void assert_true(const bool);
};

}
