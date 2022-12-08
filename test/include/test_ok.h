#pragma once

#include <cmath>
#include <iostream>


int const FAIL_RET_CODE = -1;
int const PASS_RET_CODE = 0;


struct test_ok
{
    bool ok{true};
    int operator()() const { return ok ? PASS_RET_CODE : FAIL_RET_CODE; }
    template<typename T>
    void test_eq(T const & l, T const & r, std::string const & where)
    {
        if (!(l == r))
        {
            ok = false;
            std::cout << l << " != " << r << "     <---- failed ---->     " << where << std::endl;
        }
    }
    template<typename T>
    void test_ne(T const & l, T const & r, std::string const & where)
    {
        if (l == r)
        {
            ok = false;
            std::cout << l << " == " << r << "     <---- failed ---->     " << where << std::endl;
        }
    }
    void test_fail(std::string const & where)
    {
        ok = false;
        std::cout << "failed ---->     " << where << std::endl;
    }
};


template<>
void test_ok::test_eq<double>(double const & l, double const & r, std::string const & where)
{
    if (fabs(l) - fabs(r) > 0.0001)
    {
        ok = false;
        std::cout << l << " != " << r << "     <---- failed ---->     " << where << std::endl;
    }
}


template<>
void test_ok::test_ne<double>(double const & l, double const & r, std::string const & where)
{
    if (fabs(l) - fabs(r) < 0.0001)
    {
        ok = false;
        std::cout << l << " == " << r << "     <---- failed ---->     " << where << std::endl;
    }
}


#define TEST_EQ(_test_ok, _l, _r) _test_ok.test_eq(_l, _r, __FILE__ ":" + std::to_string(__LINE__))
#define TEST_NE(_test_ok, _l, _r) _test_ok.test_ne(_l, _r, __FILE__ ":" + std::to_string(__LINE__))
#define TEST_FAIL(_test_ok) _test_ok.test_fail(__FILE__ ":" + std::to_string(__LINE__))
