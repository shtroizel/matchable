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
    // default construction
    TimeUnit::Type const default_constructed;

    // static functions returning TimeUnit::Type
    {
        TimeUnit::Seconds::grab();
        TimeUnit::Minutes::grab();
        TimeUnit::Hours::grab();
        TimeUnit::Days::grab();
        TimeUnit::Weeks::grab();
    }

    // assignment
    TimeUnit::Type const time_unit = TimeUnit::Minutes::grab();

    // copy construction / assignment
    {
        TimeUnit::Type copied{time_unit};
        TimeUnit::Type assigned = time_unit;
    }

    // move construction / assignment
    {
        TimeUnit::Type tu = TimeUnit::Minutes::grab();
        TimeUnit::Type move_constructed{std::move(tu)};
        TimeUnit::Type move_assigned = std::move(move_constructed);
    }

    {
        // string compare! (same as lt_alphabetic())
        bool lt = TimeUnit::Seconds::grab() < time_unit; // lt is false

        // explicit string compare
        lt = TimeUnit::Seconds::grab().lt_alphabetic(time_unit); // lt is false

        // enum order compare
        lt = TimeUnit::Seconds::grab().lt_enum_order(time_unit); // lt is true
    }

    {
        // equality compare
        bool eq = TimeUnit::Seconds::grab() == time_unit; // eq is false
        eq = TimeUnit::Seconds::grab() != time_unit; // eq is true
    }

    {
        // as_string()
        std::string s = time_unit.as_string(); // s is "Minutes"
        s = TimeUnit::nil.as_string(); // s is "nil"
        s = TimeUnit::Type().as_string(); // s is "nil"
    }

    // from_string()
    {
        TimeUnit::Type tu = TimeUnit::from_string("107"); // tu is TimeUnit::nil
        tu = TimeUnit::from_string("nil"); // tu is TimeUnit::nil
        tu = TimeUnit::from_string("Weeks"); // tu is TimeUnit::Weeks::grab()
    }

    // as_enum()
    switch (time_unit.as_enum())
    {
        case TimeUnit::Enum::Seconds:
        case TimeUnit::Enum::Hours:
        case TimeUnit::Enum::Days:
        case TimeUnit::Enum::Weeks:
        case TimeUnit::Enum::nil: return -1;
        case TimeUnit::Enum::Minutes:;
        // no default, compile warning as error better than runtime default
    }

    {
        // as_index()
        int index = TimeUnit::Seconds::grab().as_index(); // index is 0
        index = TimeUnit::Days::grab().as_index(); // index is 3
    }

    {
        // from_index()
        TimeUnit::Type tu = TimeUnit::from_index(4); // tu is TimeUnit::Weeks::grab()
        tu = TimeUnit::from_index(107); // tu is TimeUnit::nil
    }

    // is_nil();
    time_unit.is_nil(); // false

    {
        // flags
        TimeUnit::Flags flags;
        bool b = flags.is_set(TimeUnit::Seconds::grab()); // b is false (all are unset by default)
        flags.set(TimeUnit::Hours::grab());
        b = flags.is_set(TimeUnit::Hours::grab()); // b is true

        // copy flags
        TimeUnit::Flags more_flags{flags};
        b = more_flags.is_set(TimeUnit::Hours::grab()); // b is true

        // clear flags
        more_flags = TimeUnit::Flags{};
        b = more_flags.is_set(TimeUnit::Hours::grab()); // b is false
    }

    // match()
    {
        TimeUnit::Type const tu = [](){ return TimeUnit::Minutes::grab(); }().match({
            { TimeUnit::Seconds::grab(), [](){ exit(-1); } },
            { TimeUnit::Minutes::grab(), [](){ std::cout << "match!" << std::endl; } },
            { TimeUnit::Hours::grab(), [](){ exit(-1); } },
            { TimeUnit::Days::grab(), [](){ exit(-1); } },
            { TimeUnit::Weeks::grab(), [](){ exit(-1); } },
            { TimeUnit::nil, [](){ exit(-1); } },
        }); // tu is TimeUnit::Minutes::grab()
    }

    // traversal, variants(), operator<<()
    TimeUnit::variants().size(); // 5
    std::cout << "Traversing TimeUnit variants..." << std::endl;
    for (auto variant : TimeUnit::variants())
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
        { TimeUnit::Weeks::grab(), [](){ exit(0); } },
    });

    return -1;
}
