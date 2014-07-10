#include <easylogging++.h>
#include <string>
#include <iostream>
#include "sparse_vector.h"
#include "sparse_vector_tests.h"
#include "test.h"


_INITIALIZE_EASYLOGGINGPP 

int main(int argc, char *argn[]) {
        LOG(INFO) << "TESTS//////////////////////////////////////////////////////////////////////////";
        tester base;
        base.add(&sparse_vector_tester1());
        base.add(&sparse_vector_tester2());
        base.make_all(SHOW_PASSED);

        LOG(INFO) << "PASSED: " << base.passed();
        LOG(INFO) << "FAILED: " << base.failed();
        LOG(INFO) << "TOTAL: " << base.total();
        return 0;
}