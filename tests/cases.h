#pragma once

#include <iostream>

#include "testsimple.h"
#include "testexceptions.h"
#include "testmultipolicy.h"
#include "testreplacepolicy.h"


namespace multi_queue {

namespace tests {

    void run_all(){
        unsigned int passed = 0;
        passed += TestSimple().run_test();
        passed += TestExceptions().run_test();
        passed += TestMultiPolicy().run_test();
        passed += TestReplacePolicy().run_test();
        std::cout << "Passed: " << passed << " from 4 Tests" << std::endl;
    }

}

}
