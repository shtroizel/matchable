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


MATCHABLE(DayOfWeekOccurence, first, second, third, fourth, last)

MATCHABLE(DayOfWeek, Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday);

SPREADVECTOF_MATCHABLE(DayOfWeek, Days, Weekdays, Weekend)
VARIANT_SPREADVARIANTVECT(Days, Weekdays, DayOfWeek, Monday, Tuesday, Wednesday, Thursday, Friday)
VARIANT_SPREADVARIANTVECT(Days, Weekend, DayOfWeek, Sunday, Saturday)

MATCHABLE(
    DayOfMonth,
    _1st, _2nd, _3rd, _4th, _5th, _6th, _7th, _8th, _9th, _10th, _11th,
    _12th, _13th, _14th, _15th, _16th, _17th, _18th, _19th, _20th, _21st,
    _22nd, _23rd, _24th, _25th, _26th, _27th, _28th, _29th, _30th, _31st
)

MATCHABLE(
    Month,
    January,
    Februrary,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
)

SPREADx4_MATCHABLE(
    DayOfWeek,
    DayOfWeekOccurence,
    DayOfMonth,
    Month,
    Holiday,
    Martin_Luther_King_Jr_Day,
    Lincoln_Washington_Day,
    Mothers_Day,
    Memorial_Day,
    Fathers_Day,
    Labor_Day,
    Columbus_Day,
    Thanksgiving_Day,
    New_Years_Day,
    Independence_Day,
    Veterans_Day,
    Christmas_Day
)
VARIANT_SPREADVARIANT(Holiday, Mothers_Day, DayOfWeek, Sunday)
VARIANT_SPREADVARIANT(Holiday, Mothers_Day, DayOfWeekOccurence, second)
VARIANT_SPREADVARIANT(Holiday, Mothers_Day, Month, May)

VARIANT_SPREADVARIANT(Holiday, Fathers_Day, DayOfWeek, Sunday)
VARIANT_SPREADVARIANT(Holiday, Fathers_Day, DayOfWeekOccurence, third)
VARIANT_SPREADVARIANT(Holiday, Fathers_Day, Month, June)

VARIANT_SPREADVARIANT(Holiday, Martin_Luther_King_Jr_Day, DayOfWeek, Monday)
VARIANT_SPREADVARIANT(Holiday, Martin_Luther_King_Jr_Day, DayOfWeekOccurence, third)
VARIANT_SPREADVARIANT(Holiday, Martin_Luther_King_Jr_Day, Month, January)

VARIANT_SPREADVARIANT(Holiday, Lincoln_Washington_Day, DayOfWeek, Monday)
VARIANT_SPREADVARIANT(Holiday, Lincoln_Washington_Day, DayOfWeekOccurence, third)
VARIANT_SPREADVARIANT(Holiday, Lincoln_Washington_Day, Month, Februrary)

VARIANT_SPREADVARIANT(Holiday, Memorial_Day, DayOfWeek, Monday)
VARIANT_SPREADVARIANT(Holiday, Memorial_Day, DayOfWeekOccurence, last)
VARIANT_SPREADVARIANT(Holiday, Memorial_Day, Month, May)

VARIANT_SPREADVARIANT(Holiday, Labor_Day, DayOfWeek, Monday)
VARIANT_SPREADVARIANT(Holiday, Labor_Day, DayOfWeekOccurence, first)
VARIANT_SPREADVARIANT(Holiday, Labor_Day, Month, September)

VARIANT_SPREADVARIANT(Holiday, Columbus_Day, DayOfWeek, Monday)
VARIANT_SPREADVARIANT(Holiday, Columbus_Day, DayOfWeekOccurence, second)
VARIANT_SPREADVARIANT(Holiday, Columbus_Day, Month, October)

VARIANT_SPREADVARIANT(Holiday, Thanksgiving_Day, DayOfWeek, Thursday)
VARIANT_SPREADVARIANT(Holiday, Thanksgiving_Day, DayOfWeekOccurence, fourth)
VARIANT_SPREADVARIANT(Holiday, Thanksgiving_Day, Month, November)

VARIANT_SPREADVARIANT(Holiday, New_Years_Day, DayOfMonth, _1st)
VARIANT_SPREADVARIANT(Holiday, New_Years_Day, Month, January)

VARIANT_SPREADVARIANT(Holiday, Independence_Day, DayOfMonth, _4th)
VARIANT_SPREADVARIANT(Holiday, Independence_Day, Month, July)

VARIANT_SPREADVARIANT(Holiday, Veterans_Day, DayOfMonth, _11th)
VARIANT_SPREADVARIANT(Holiday, Veterans_Day, Month, November)

VARIANT_SPREADVARIANT(Holiday, Christmas_Day, DayOfMonth, _25th)
VARIANT_SPREADVARIANT(Holiday, Christmas_Day, Month, December)



bool is_weekday_v0(DayOfWeek::Type day)
{
    return MATCHABLE_INSTANCE_IN(DayOfWeek, day, Monday, Tuesday, Wednesday, Thursday, Friday);
}


bool is_weekday_v1(DayOfWeek::Type day)
{
    auto const & v = Days::Weekdays().grab().as_DayOfWeek_vect();
    return std::find(v.begin(), v.end(), day) != v.end();
}


void print_weekdays()
{
    // Remove some DayOfWeek variants for the current scope
    UNMATCHABLE(DayOfWeek, Sunday, Saturday);

    std::cout << "***** print_weekdays() *****" << std::endl;
    for (DayOfWeek::Type d : DayOfWeek::variants())
        std::cout << "    " << d << std::endl;
    std::cout << std::endl;
}


void print_holidays()
{
    std::cout << "***** print_holidays() *****" << std::endl;
    auto holidays = Holiday::variants();
    std::sort(holidays.begin(), holidays.end(), [](auto a, auto b) { return a.lt_by_string(b); });
    for (Holiday::Type h : holidays)
    {
        if (h.as_DayOfWeekOccurence().is_nil())
            std::cout << "    " << h.as_text() << " is celebrated on " << h.as_Month()
                    << h.as_DayOfMonth().as_text() << std::endl;
        else
            std::cout << "    " << h.as_text() << " is celebrated on the " << h.as_DayOfWeekOccurence() << " "
                    << h.as_DayOfWeek() << " in " << h.as_Month() << std::endl;
    }
    std::cout << std::endl;
}


void print_friday_july_thirteenth_2007()
{
    std::cout << "***** print_friday_july_thirteenth_2007() *****" << std::endl;

    DayOfWeek::Type dow = DayOfWeek::from_string("Friday");
    DayOfMonth::Type dom = DayOfMonth::_13th::grab();
    Month::Type month = Month::from_index(6);

    std::cout << dow << ", " << month << " " << dom.as_index() + 1 << ", 2007" << std::endl;
    std::cout << dow << " the" << dom.as_text() << ", " << month << " 2007" << std::endl;
}


int main()
{
    print_weekdays();
    print_holidays();
    print_friday_july_thirteenth_2007();
    return 0;
}
