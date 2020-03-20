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



Result::Type foo(int number)
{
    if (number == 107)
        return Result::Ok::grab();
    return Result::Err::grab();
}



int main()
{
    test_ok ok;

    // default construction
    TimeUnit::Type time_unit;
    TEST_EQ(ok, time_unit, TimeUnit::nil);

    // assignment
    time_unit = TimeUnit::Minutes::grab();
    TEST_EQ(ok, time_unit, TimeUnit::Minutes::grab());

    // copy construction
    TimeUnit::Type time_unit_copied{time_unit};
    TEST_EQ(ok, time_unit_copied, time_unit);
    TEST_EQ(ok, time_unit, TimeUnit::Minutes::grab());

    // move construction
    TimeUnit::Type another_time_unit{std::move(time_unit)};
    TEST_EQ(ok, time_unit, TimeUnit::nil);
    TEST_EQ(ok, another_time_unit, TimeUnit::Minutes::grab());

    // copy assignment
    TimeUnit::Type another_time_unit_copied = time_unit_copied;
    TEST_EQ(ok, another_time_unit_copied, time_unit_copied);
    TEST_EQ(ok, another_time_unit_copied, TimeUnit::Minutes::grab());

    // move assignment
    time_unit = std::move(another_time_unit);
    TEST_EQ(ok, time_unit, TimeUnit::Minutes::grab());
    TEST_EQ(ok, another_time_unit, TimeUnit::nil);

    // compare operators
    another_time_unit = TimeUnit::Seconds::grab();
    if (time_unit < another_time_unit) // index compare! (same as lt_by_index())
        TEST_FAIL(ok);

    if (time_unit.lt_by_index(another_time_unit))
        TEST_FAIL(ok);

    if (another_time_unit.lt_by_string(time_unit))
        TEST_FAIL(ok);

    if (another_time_unit == time_unit)
        TEST_FAIL(ok);

    another_time_unit = time_unit;
    if (another_time_unit != time_unit)
        TEST_FAIL(ok);

    // as_string()
    TEST_EQ(ok, time_unit.as_string(), std::string("Minutes"));
    TEST_EQ(ok, TimeUnit::Type().as_string(), std::string("nil"));

    // from_string()
    TEST_EQ(ok, TimeUnit::from_string("107"), TimeUnit::nil);
    TEST_EQ(ok, TimeUnit::from_string("nil"), TimeUnit::nil);
    TEST_EQ(ok, TimeUnit::from_string("Weeks"), TimeUnit::Weeks::grab());

    // as_index()
    TEST_EQ(ok, TimeUnit::Seconds::grab().as_index(), 0);
    TEST_EQ(ok, TimeUnit::Minutes::grab().as_index(), 1);
    TEST_EQ(ok, TimeUnit::Hours::grab().as_index(), 2);
    TEST_EQ(ok, TimeUnit::Days::grab().as_index(), 3);
    TEST_EQ(ok, TimeUnit::Weeks::grab().as_index(), 4);

    // from_index()
    TEST_EQ(ok, TimeUnit::from_index(-1), TimeUnit::nil);
    for (auto tu : TimeUnit::variants())
        TEST_EQ(ok, tu, TimeUnit::from_index(tu.as_index()));
    TEST_EQ(ok, TimeUnit::from_index(107), TimeUnit::nil);

    // is_nil()
    TEST_NE(ok, time_unit, TimeUnit::nil);

    // Flags
    TimeUnit::Flags flags;
    for (auto const & tu : TimeUnit::variants())
        TEST_EQ(ok, flags.is_set(tu), false);
    flags.set(TimeUnit::Seconds::grab());
    time_unit = TimeUnit::Seconds::grab();
    TEST_EQ(ok, flags.is_set(time_unit), true);
    TimeUnit::Flags more_flags{flags};
    flags.unset(TimeUnit::Seconds::grab());
    TEST_EQ(ok, more_flags.is_set(time_unit), true);
    TEST_EQ(ok, flags.is_set(time_unit), false);
    TEST_NE(ok, flags, more_flags);
    flags.set(TimeUnit::Hours::grab());
    flags.set(TimeUnit::Seconds::grab());
    more_flags.set(TimeUnit::Hours::grab());
    TEST_EQ(ok, flags, more_flags);

    // match()
    int input{107};
    foo(input).match({
        {Result::Ok::grab(), [&]() {std::cout << "foo(" << input << ") is ok!" << std::endl;}},
        {Result::Err::grab(), [&](){std::cout << "foo(" << input << ") error" << std::endl; TEST_FAIL(ok);}}
    });

    // traversal, variants(), operator<<()
    TEST_EQ(ok, TimeUnit::variants().size(), static_cast<size_t>(5));
    std::cout << "Traversing variants..." << std::endl;
    for (auto const & variant : TimeUnit::variants())
        std::cout << "    " << variant << std::endl;

    // matchable without variants
    NIL::Type n;
    TEST_EQ(ok, n.is_nil(), true);

    return ok();
}
