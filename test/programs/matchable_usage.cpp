/*
Copyright (c) 2019-2022, shtroizel
All rights reserved without prejudice.

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
#include "matchable/matchable.h"


MATCHABLE(
    // type
    DayOfWeek,

    // variants
    Monday,
    Tuesday,
    Wednessday,
    Thursday,
    Friday,
    Saturday,
    Sunday
)


MATCHABLE(
    Status,

    // escape codes can be used for characters that are invalid for identifiers
    success_bng_,              // Status::success_bng_::grab().as_string() == "success!"
    invalid_spc_input,         // Status::invalid_spc_input::grab().as_string() == "invalid input"
    timed_spc_out,             // Status::from_string("timed out") == Status::timed_spc_out::grab()
    nothing_spc_to_spc_do,
    insufficient_spc_memory,
    index_spc_out_spc_of_spc_bounds,

    // optionally, variants can be prefixed with esc_ as needed
    esc_17                     // Status::esc_17::grab().as_string() == "17"

    // see bottom of matchable/matchable.h for all the escape codes
    // also reference test/programs/escapable.cpp for more on this
);


Status::Type foo(std::string day_string)
{
    // create an instance of the DayOfWeek matchable
    DayOfWeek::Type day_of_week;

    // all matchable types have a hidden "nil" variant
    // at this point day_of_week == DayOfWeek::nil
    assert(day_of_week == DayOfWeek::nil);

    // nil is a valid and usable variant but its syntax is a bit different
    // notice how we don't "grab()" nil like we do for the other variants
    // also, to test for nil we could alternatively do:
    assert(day_of_week.is_nil());

    // lets try to assign the variant from the day_string parameter
    // note the complexity for from_string() is O(log N)
    // if day_string is equal to the output of "as_string()" for any defined variant,
    // then day_of_week will be set to that variant
    // if no variant is found then nil will be assigned instead
    day_of_week = DayOfWeek::from_string(day_string);
    if (day_of_week.is_nil())
        return Status::invalid_spc_input::grab();

    // to show how flags work, we'll create a flags instance and flag our favorite days
    DayOfWeek::Flags favorite_days;
    for (int i = 0; i < 3; ++i)
        favorite_days.set(DayOfWeek::from_index(i)); // note the complexity for from_index is O(1)

    // oops, actually we don't like Wednessday afterall
    favorite_days.unset(DayOfWeek::Wednessday::grab());

    // to get all flagged variants we can use currently_set()
    for (auto day : favorite_days.currently_set())
        if (day == day_of_week)
            std::cout << day << " is one of my favorite days! ";

    // we could also use MATCHABLE_INSTANCE_IN()
    if (!MATCHABLE_INSTANCE_IN(DayOfWeek, day_of_week, Monday, Tuesday))
        return Status::nothing_spc_to_spc_do::grab();

    return Status::success_bng_::grab();
}


int main()
{
    std::cout << "variant iteration by index:" << std::endl;
    for (auto day_of_week : DayOfWeek::variants_by_index())
        std::cout << "    " << day_of_week << std::endl;

    std::cout << "\nvariant iteration by string (alphabetic):" << std::endl;
    for (auto day_of_week : DayOfWeek::variants_by_string())
        std::cout << "    " << day_of_week << std::endl;

    std::cout << "\noutput of foo(\"bad input\") --> " << foo("bad input") << std::endl;
    std::cout << "output of foo(\"Saturday\") --> " << foo("Saturday") << std::endl;
    std::cout << "output of foo(\"Tuesday\") --> " << foo("Tuesday") << std::endl;

    return 0;
}
