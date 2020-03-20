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



MATCHABLE(TimeUnit, Seconds, Minutes, Hours, Days, Weeks)



int main()
{
    {
        // default construction
        TimeUnit::Type const default_constructed;
    }

    // static functions returning TimeUnit::Type
    TimeUnit::Seconds::grab();
    TimeUnit::Minutes::grab();
    TimeUnit::Hours::grab();
    TimeUnit::Days::grab();
    TimeUnit::Weeks::grab();

    // assignment
    TimeUnit::Type const time_unit = TimeUnit::Minutes::grab();

    {
        // copy construction
        TimeUnit::Type copied{time_unit};
    }

    {
        // move construction / assignment
        TimeUnit::Type tu = TimeUnit::Minutes::grab();
        TimeUnit::Type move_constructed{std::move(tu)};
        TimeUnit::Type move_assigned = std::move(move_constructed);
    }

    {
        // same as lt_by_index())
        bool lt = TimeUnit::Seconds::grab() < time_unit; // lt is true
        (void) lt;

        // string compare
        lt = TimeUnit::Seconds::grab().lt_by_string(time_unit); // lt is false

        // enum order compare
        lt = TimeUnit::Seconds::grab().lt_by_index(time_unit); // lt is true
    }

    {
        // equality compare
        bool eq = TimeUnit::Seconds::grab() == time_unit; // eq is false
        (void) eq;
        eq = TimeUnit::Seconds::grab() != time_unit; // eq is true
    }

    {
        // as_string()
        std::string s = time_unit.as_string(); // s is "Minutes"
        s = TimeUnit::nil.as_string(); // s is "nil"
        s = TimeUnit::Type().as_string(); // s is "nil"
    }

    {
        // from_string()
        TimeUnit::Type tu = TimeUnit::from_string("107"); // tu is TimeUnit::nil
        tu = TimeUnit::from_string("nil"); // tu is TimeUnit::nil
        tu = TimeUnit::from_string("Weeks"); // tu is TimeUnit::Weeks::grab()
    }

    {
        // as_index()
        int index = TimeUnit::Seconds::grab().as_index(); // index is 0
        (void) index;
        index = TimeUnit::Days::grab().as_index(); // index is 3
    }

    {
        // from_index()
        TimeUnit::Type tu = TimeUnit::from_index(4); // tu is TimeUnit::Weeks::grab()
        tu = TimeUnit::from_index(107); // tu is TimeUnit::nil
    }

    // is_nil()
    if (time_unit.is_nil())
        exit(EXIT_FAILURE);

    {
        // flags
        TimeUnit::Flags flags;
        bool b = flags.is_set(TimeUnit::Seconds::grab()); // b is false (all are unset by default)
        (void) b;
        flags.set(TimeUnit::Hours::grab());
        b = flags.is_set(TimeUnit::Hours::grab()); // b is true

        // copy flags
        TimeUnit::Flags more_flags{flags};
        b = more_flags.is_set(TimeUnit::Hours::grab()); // b is true

        // clear flags
        more_flags = TimeUnit::Flags{};
        b = more_flags.is_set(TimeUnit::Hours::grab()); // b is false
    }

    {
        // match()
        time_unit.match({
            { TimeUnit::Seconds::grab(), [](){ exit(EXIT_FAILURE); } },
            { TimeUnit::Minutes::grab(), [](){ std::cout << "match!" << std::endl; } },
            { TimeUnit::Hours::grab(), [](){ exit(EXIT_FAILURE); } },
            { TimeUnit::Days::grab(), [](){ exit(EXIT_FAILURE); } },
            { TimeUnit::Weeks::grab(), [](){ exit(EXIT_FAILURE); } },
            { TimeUnit::nil, [](){ exit(EXIT_FAILURE); } },
        }); // tu is TimeUnit::Minutes::grab()
    }

    // traversal, variants(), operator<<()
    TimeUnit::variants().size(); // 5
    std::cout << "Printing some TimeUnit variants..." << std::endl;
    for (auto variant : TimeUnit::variants())
        if (MATCHABLE_INSTANCE_IN(TimeUnit, variant, Hours, Days, Weeks))
            std::cout << "    " << variant << std::endl;

    {
        // remove Days and Weeks
        UNMATCHABLE(TimeUnit, Days, Weeks);
        std::cout << "Traversing TimeUnit variants with Days and Weeks removed..." << std::endl;
        for (auto variant : TimeUnit::variants())
            std::cout << "    " << variant << std::endl;
    }

    std::cout << "Traversing TimeUnit variants with Days and Weeks restored..." << std::endl;
    for (auto variant : TimeUnit::variants())
        std::cout << "    " << variant << std::endl;

    // return success
    [](int magic){ if (magic == 107) return TimeUnit::Weeks::grab(); return TimeUnit::nil; }(107).match({
        { TimeUnit::Weeks::grab(), [](){ exit(EXIT_SUCCESS); } },
    });

    return EXIT_FAILURE;
}
