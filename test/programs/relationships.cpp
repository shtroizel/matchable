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

SPREADx1_MATCHABLE(DayOfWeek::Type, DayOfWeek, Days, Weekdays, Weekend)
SET_SPREAD_VECT(
    Days,
    Weekdays,
    DayOfWeek,
    DayOfWeek::Monday::grab(),
    DayOfWeek::Tuesday::grab(),
    DayOfWeek::Wednesday::grab(),
    DayOfWeek::Thursday::grab(),
    DayOfWeek::Friday::grab()
)
SET_SPREAD_VECT(Days, Weekend, DayOfWeek, DayOfWeek::Sunday::grab(), DayOfWeek::Saturday::grab())

MATCHABLE(
    DayOfMonth,
               esc_1st,  esc_2nd,  esc_3rd,  esc_4th,  esc_5th,  esc_6th,  esc_7th,  esc_8th,  esc_9th,
    esc_10th, esc_11th, esc_12th, esc_13th, esc_14th, esc_15th, esc_16th, esc_17th, esc_18th, esc_19th,
    esc_20th, esc_21st, esc_22nd, esc_23rd, esc_24th, esc_25th, esc_26th, esc_27th, esc_28th, esc_29th,
    esc_30th, esc_31st
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
    DayOfWeek::Type,
    DayOfWeek,
    DayOfWeekOccurence::Type,
    DayOfWeekOccurence,
    DayOfMonth::Type,
    DayOfMonth,
    Month::Type,
    Month,
    Holiday,
    Martin_spc_Luther_spc_King_spc_Jr_spc_Day,
    Lincoln_spc_Washington_spc_Day,
    Mothers_spc_Day,
    Memorial_spc_Day,
    Fathers_spc_Day,
    Labor_spc_Day,
    Columbus_spc_Day,
    Thanksgiving_spc_Day,
    New_spc_Years_spc_Day,
    Independence_spc_Day,
    Veterans_spc_Day,
    Christmas_spc_Day
)
SET_SPREAD(Holiday, Mothers_spc_Day, DayOfWeek, DayOfWeek::Sunday::grab())
SET_SPREAD(Holiday, Mothers_spc_Day, DayOfWeekOccurence, DayOfWeekOccurence::second::grab())
SET_SPREAD(Holiday, Mothers_spc_Day, Month, Month::May::grab())

SET_SPREAD(Holiday, Fathers_spc_Day, DayOfWeek, DayOfWeek::Sunday::grab())
SET_SPREAD(Holiday, Fathers_spc_Day, DayOfWeekOccurence, DayOfWeekOccurence::third::grab())
SET_SPREAD(Holiday, Fathers_spc_Day, Month, Month::June::grab())

SET_SPREAD(Holiday, Martin_spc_Luther_spc_King_spc_Jr_spc_Day, DayOfWeek, DayOfWeek::Monday::grab())
SET_SPREAD(Holiday, Martin_spc_Luther_spc_King_spc_Jr_spc_Day, DayOfWeekOccurence, DayOfWeekOccurence::third::grab())
SET_SPREAD(Holiday, Martin_spc_Luther_spc_King_spc_Jr_spc_Day, Month, Month::January::grab())

SET_SPREAD(Holiday, Lincoln_spc_Washington_spc_Day, DayOfWeek, DayOfWeek::Monday::grab())
SET_SPREAD(Holiday, Lincoln_spc_Washington_spc_Day, DayOfWeekOccurence, DayOfWeekOccurence::third::grab())
SET_SPREAD(Holiday, Lincoln_spc_Washington_spc_Day, Month, Month::Februrary::grab())

SET_SPREAD(Holiday, Memorial_spc_Day, DayOfWeek, DayOfWeek::Monday::grab())
SET_SPREAD(Holiday, Memorial_spc_Day, DayOfWeekOccurence, DayOfWeekOccurence::last::grab())
SET_SPREAD(Holiday, Memorial_spc_Day, Month, Month::May::grab())

SET_SPREAD(Holiday, Labor_spc_Day, DayOfWeek, DayOfWeek::Monday::grab())
SET_SPREAD(Holiday, Labor_spc_Day, DayOfWeekOccurence, DayOfWeekOccurence::first::grab())
SET_SPREAD(Holiday, Labor_spc_Day, Month, Month::September::grab())

SET_SPREAD(Holiday, Columbus_spc_Day, DayOfWeek, DayOfWeek::Monday::grab())
SET_SPREAD(Holiday, Columbus_spc_Day, DayOfWeekOccurence, DayOfWeekOccurence::second::grab())
SET_SPREAD(Holiday, Columbus_spc_Day, Month, Month::October::grab())

SET_SPREAD(Holiday, Thanksgiving_spc_Day, DayOfWeek, DayOfWeek::Thursday::grab())
SET_SPREAD(Holiday, Thanksgiving_spc_Day, DayOfWeekOccurence, DayOfWeekOccurence::fourth::grab())
SET_SPREAD(Holiday, Thanksgiving_spc_Day, Month, Month::November::grab())

SET_SPREAD(Holiday, New_spc_Years_spc_Day, DayOfMonth, DayOfMonth::esc_1st::grab())
SET_SPREAD(Holiday, New_spc_Years_spc_Day, Month, Month::January::grab())

SET_SPREAD(Holiday, Independence_spc_Day, DayOfMonth, DayOfMonth::esc_4th::grab())
SET_SPREAD(Holiday, Independence_spc_Day, Month, Month::July::grab())

SET_SPREAD(Holiday, Veterans_spc_Day, DayOfMonth, DayOfMonth::esc_11th::grab())
SET_SPREAD(Holiday, Veterans_spc_Day, Month, Month::November::grab())

SET_SPREAD(Holiday, Christmas_spc_Day, DayOfMonth, DayOfMonth::esc_25th::grab())
SET_SPREAD(Holiday, Christmas_spc_Day, Month, Month::December::grab())



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
    for (Holiday::Type h : Holiday::variants_by_string())
    {
        if (h.as_DayOfWeekOccurence().is_nil())
            std::cout << "    " << h << " is celebrated on " << h.as_Month()
                      << " " << h.as_DayOfMonth() << std::endl;
        else
            std::cout << "    " << h << " is celebrated on the " << h.as_DayOfWeekOccurence()
                      << " " << h.as_DayOfWeek() << " of " << h.as_Month() << std::endl;
    }
    std::cout << std::endl;
}


void print_friday_july_thirteenth_2007()
{
    std::cout << "***** print_friday_july_thirteenth_2007() *****" << std::endl;

    DayOfWeek::Type dow = DayOfWeek::Friday::grab();
    DayOfMonth::Type dom = DayOfMonth::from_string("13th");
    Month::Type month = Month::from_index(6);

    std::cout << dow << ", " << month << " " << dom.as_index() + 1 << ", 2007" << std::endl;
    std::cout << dow << " the " << dom << ", " << month << " 2007" << std::endl;
}


int main()
{
    print_weekdays();
    print_holidays();
    print_friday_july_thirteenth_2007();
    return 0;
}
