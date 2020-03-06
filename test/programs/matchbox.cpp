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



// ************ normally save such content to something like TimeUnit.h ************************************
MATCHABLE(TimeUnit, Seconds, Minutes, Hours)

namespace TimeUnit
{
    static MatchBox<var, std::function<double(double const &)>> const as_seconds({
        { nil,             [](double const &)  { return 0.0; } },
        { Seconds::grab(), [](double const & d){ return d; } },
        { Minutes::grab(), [](double const & d){ return d * 60.0; } },
        { Hours::grab(),   [](double const & d){ return d * 60.0 * 60.0; } },
    });

    static MatchBox<var, std::function<double(double const &)>> const as_minutes({
        { nil,             [](double const &)  { return 0.0; } },
        { Seconds::grab(), [](double const & d){ return 0.0 == d ? 0.0 : d / 60.0; } },
        { Minutes::grab(), [](double const & d){ return d;} },
        { Hours::grab(),   [](double const & d){ return d * 60.0; } }
    });

    static MatchBox<var, std::function<double(double const &)>> const as_hours({
        { nil,             [](double const &)  { return 0.0; } },
        { Seconds::grab(), [](double const & d){ return 0.0 == d ? 0.0 : d / (60.0 * 60.0);} },
        { Minutes::grab(), [](double const & d){ return 0.0 == d ? 0.0 : d / 60.0; } },
        { Hours::grab(),   [](double const & d){ return d; } }
    });

    static MatchBox<var, MatchBox<var, std::function<double(double const &)>>> const as_sibling({
        { nil,             MatchBox<var, std::function<double(double const &)>>{} },
        { Seconds::grab(), as_seconds },
        { Minutes::grab(), as_minutes },
        { Hours::grab(),   as_hours },
    });

    inline double convert(var const & in_unit, double const & in_val, var const & out_unit)
    {
        return as_sibling.at(out_unit).at(in_unit)(in_val);
    }
}
// *********************************************************************************************************


int main()
{
    test_ok ok;

    // just print conversions [0..107] for fun...
    for (int i = 0; i < 108; ++i)
    {
        for (auto const & unit_from : TimeUnit::variants())
        {
            std::cout << i << " " << unit_from << " ---> ";
            for (auto const & unit_to : TimeUnit::variants())
                std::cout << TimeUnit::convert(unit_from, i, unit_to) << " " << unit_to << ", ";
            std::cout << std::endl;
        }
    }

    // but actually test 108
    TEST_EQ(ok, TimeUnit::convert(TimeUnit::Minutes::grab(), 1.8, TimeUnit::Hours::grab()), 0.03);
    TEST_EQ(ok, TimeUnit::convert(TimeUnit::Hours::grab(), 0.03, TimeUnit::Minutes::grab()), 1.8);

    TEST_EQ(ok, TimeUnit::convert(TimeUnit::Seconds::grab(), 6480.0, TimeUnit::Hours::grab()), 1.8);
    TEST_EQ(ok, TimeUnit::convert(TimeUnit::Hours::grab(), 1.8, TimeUnit::Seconds::grab()), 6480.0);

    TEST_EQ(ok, TimeUnit::convert(TimeUnit::Seconds::grab(), 388800.0, TimeUnit::Minutes::grab()), 6480.0);
    TEST_EQ(ok, TimeUnit::convert(TimeUnit::Minutes::grab(), 6480.0, TimeUnit::Seconds::grab()), 388800.0);

    return ok();
}
