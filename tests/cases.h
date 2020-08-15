#pragma once

#include "testsimple.h"
#include "testexceptions.h"
#include "testmultipolicy.h"

namespace tests {

    void run_all(){
        TestSimple().run_test();
        TestExceptions().run_test();
        TestMultiPolicy().run_test();
    }

}
