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
#include <vector>

#include "matchable.h"
#include "test_ok.h"



MATCHABLE(Day, Sun, Mon, Tue, Wed, Thu, Fri, Sat)

SPREADx1_MATCHABLE(Day::Type, day, Days, Weekdays, Weekend)

// Note there are two syntaxes for this. The SET_SPREAD_VECT() macro is used here for Weekdays.
// To demonstrate the other syntax we use the run-time interface within main() to set the Days::Weekend
// variant.
SET_SPREAD_VECT(
    Days,
    Weekdays,
    day,
    Day::Mon::grab(),
    Day::Tue::grab(),
    Day::Wed::grab(),
    Day::Thu::grab(),
    Day::Fri::grab()
)

int main()
{
    test_ok ok;

    // accomplishes the same as VARIANT_SPREADVARIANTVECT() above, but at run-time...
    Days::Weekend::grab().set_day_vect({Day::Sat::grab(), Day::Sun::grab()});

    for (auto days : Days::variants_by_index())
    {
        std::cout << days << ": ";
        for (auto d : days.as_day_vect())
            std::cout << d << " ";
        std::cout << std::endl;
    }

    TEST_EQ(ok, Days::Weekend::grab().as_day_vect().size(), static_cast<size_t>(2));
    TEST_EQ(ok, Days::Weekdays::grab().as_day_vect().size(), static_cast<size_t>(5));

    {
        std::vector<Day::Type> const TRUTH{Day::Sat::grab(), Day::Sun::grab()};
        if (Days::Weekend::grab().as_day_vect() != TRUTH)
            TEST_FAIL(ok);
    }
    {
        std::vector<Day::Type> const TRUTH{
            Day::Mon::grab(),
            Day::Tue::grab(),
            Day::Wed::grab(),
            Day::Thu::grab(),
            Day::Fri::grab(),
        };
        if (Days::Weekdays::grab().as_day_vect() != TRUTH)
            TEST_FAIL(ok);
    }

    return ok();
}
