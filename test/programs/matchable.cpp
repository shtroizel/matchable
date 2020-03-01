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


#include <iostream>

#include "matchable.h"
#include "test_ok.h"



MATCHABLE(TimeUnit, Seconds, Minutes, Hours, Days, Weeks)
MATCHABLE(Result, Ok, Err)

// compile time test: possible to create matchable without variants?
MATCHABLE(NIL)


Result::var foo(int number)
{
    if (number == 107)
        return Result::Ok::grab();
    return Result::Err::grab();
}


int main()
{
    test_ok ok;

    // default construction
    TimeUnit::var time_unit;
    TEST_EQ(ok, time_unit, TimeUnit::nil);

    // assignment
    time_unit = TimeUnit::Minutes::grab();
    TEST_EQ(ok, time_unit, TimeUnit::Minutes::grab());

    // copy construction
    TimeUnit::var time_unit_copied{time_unit};
    TEST_EQ(ok, time_unit_copied, time_unit);
    TEST_EQ(ok, time_unit, TimeUnit::Minutes::grab());

    // move construction
    TimeUnit::var another_time_unit{std::move(time_unit)};
    TEST_EQ(ok, time_unit, TimeUnit::nil);
    TEST_EQ(ok, another_time_unit, TimeUnit::Minutes::grab());

    // copy assignment
    TimeUnit::var another_time_unit_copied = time_unit_copied;
    TEST_EQ(ok, another_time_unit_copied, time_unit_copied);
    TEST_EQ(ok, another_time_unit_copied, TimeUnit::Minutes::grab());

    // move assignment
    time_unit = std::move(another_time_unit);
    TEST_EQ(ok, time_unit, TimeUnit::Minutes::grab());
    TEST_EQ(ok, another_time_unit, TimeUnit::nil);

    // compare operators
    another_time_unit = TimeUnit::Seconds::grab();
    if (another_time_unit < time_unit) // string compare!
        FAIL(ok);

    if (another_time_unit == time_unit)
        FAIL(ok);
    
    another_time_unit = time_unit;
    if (another_time_unit != time_unit)
        FAIL(ok);

    // as_string()
    TEST_EQ(ok, time_unit.as_string(), std::string("Minutes"));
    TEST_EQ(ok, TimeUnit::var().as_string(), std::string("nil"));

    // from_string()
    TEST_EQ(ok, TimeUnit::from_string("107"), TimeUnit::nil);
    TEST_EQ(ok, TimeUnit::from_string("nil"), TimeUnit::nil);
    TEST_EQ(ok, TimeUnit::from_string("Weeks"), TimeUnit::Weeks::grab());

    // as_enum()
    switch (time_unit.as_enum())
    {
        case TimeUnit::Enum::Seconds:
        case TimeUnit::Enum::Hours:
        case TimeUnit::Enum::Days:
        case TimeUnit::Enum::Weeks:
        case TimeUnit::Enum::nil: FAIL(ok);
        case TimeUnit::Enum::Minutes:;
    }

    // as_index()
    TEST_EQ(ok, TimeUnit::Seconds::grab().as_index(), 0);
    TEST_EQ(ok, TimeUnit::Minutes::grab().as_index(), 1);
    TEST_EQ(ok, TimeUnit::Hours::grab().as_index(), 2);
    TEST_EQ(ok, TimeUnit::Days::grab().as_index(), 3);
    TEST_EQ(ok, TimeUnit::Weeks::grab().as_index(), 4);

    // is_nil()
    if (time_unit.is_nil())
        FAIL(ok);

    // flag(), unflag(), set_flagged(), is_flagged()
    for (auto const & tu : TimeUnit::variants())
        TEST_EQ(ok, tu.is_flagged(), false);
    TimeUnit::Seconds::grab().flag();
    time_unit = TimeUnit::Seconds::grab();
    TEST_EQ(ok, time_unit.is_flagged(), true);
    time_unit.unflag();
    TEST_EQ(ok, time_unit.is_flagged(), false);
    time_unit.set_flagged(true);
    TEST_EQ(ok, time_unit.is_flagged(), true);
    time_unit.set_flagged(false);
    TEST_EQ(ok, time_unit.is_flagged(), false);

    // flagged_variants()   [static]
    time_unit.flag();
    auto fv = TimeUnit::flagged_variants();
    TEST_EQ(ok, fv.size(), (size_t) 1);
    TEST_EQ(ok, fv[0], time_unit);

    // flagged_variants()
    TimeUnit::Minutes::grab().flag();
    fv = time_unit.flagged_variants();
    TEST_EQ(ok, fv.size(), (size_t) 2);
    TEST_EQ(ok, fv[0], TimeUnit::Seconds::grab());
    TEST_EQ(ok, fv[1], TimeUnit::Minutes::grab());


    // match() ignoring return value
    int input{107};
    foo(input).match({
        {Result::Ok::grab(), [&]() {std::cout << "foo0(" << input << ") is ok!" << std::endl;}},
        {Result::Err::grab(), [&](){std::cout << "foo0(" << input << ") failed" << std::endl; FAIL(ok);}}
    });

    // match() with return value
    input = 42;
    Result::var r = foo(input).match({
        {Result::Ok::grab(), [&](){std::cout << "foo0(" << input << ") is ok!" << std::endl;}},
        {Result::Err::grab(), [&](){std::cout << "foo0(" << input << ") failed" << std::endl;}}
    });
    TEST_EQ(ok, r, Result::Err::grab());

    // traversal, variants(), operator<<()
    TEST_EQ(ok, TimeUnit::variants().size(), static_cast<size_t>(5));
    std::cout << "Traversing variants..." << std::endl;
    for (auto const & variant : TimeUnit::variants())
        std::cout << "    " << variant << std::endl;

    // matchable without variants
    NIL::var n;
    TEST_EQ(ok, n.is_nil(), true);
    
    return ok();
}
