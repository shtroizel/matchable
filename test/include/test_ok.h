#pragma once



/*
Copyright (c) 2019-2020, Eric Hyer
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



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
