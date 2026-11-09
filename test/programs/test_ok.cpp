#include <iostream>

#include "test_ok.h"



int main()
{
    test_ok ok;

    std::cout << "the following \"failed\" lines are expected (probes):" << std::endl;

    {
        test_ok probe;
        TEST_EQ(probe, 1.0, 5.0);
        TEST_EQ(ok, probe.ok, false);
    }
    {
        test_ok probe;
        TEST_EQ(probe, -3.0, 3.0);
        TEST_EQ(ok, probe.ok, false);
    }
    {
        test_ok probe;
        TEST_NE(probe, 7.0, 7.0);
        TEST_EQ(ok, probe.ok, false);
    }
    {
        test_ok probe;
        TEST_NE(probe, -2.0, 2.0);
        TEST_EQ(ok, probe.ok, true);
    }
    {
        test_ok probe;
        TEST_EQ(probe, 0.1 + 0.2, 0.3);
        TEST_EQ(ok, probe.ok, true);
    }
    {
        test_ok probe;
        TEST_EQ(probe, 1, 2);
        TEST_EQ(ok, probe.ok, false);
        TEST_EQ(ok, probe(), FAIL_RET_CODE);
    }
    {
        test_ok probe;
        TEST_EQ(probe, 2, 2);
        TEST_EQ(ok, probe(), PASS_RET_CODE);
    }

    return ok();
}
