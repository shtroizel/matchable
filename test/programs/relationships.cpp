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

SPREADx3_MATCHABLE(
    DayOfWeek,
    DayOfWeekOccurence,
    Month,
    DayOfWeekHoliday,
    Martin_Luther_King_Jr_Day,
    Lincoln_Washington_Day,
    Mothers_Day,
    Memorial_Day,
    Fathers_Day,
    Labor_Day,
    Columbus_Day,
    Thanksgiving_Day
)
SPREADVARIANT_VARIANTS(DayOfWeek, Sunday, DayOfWeekHoliday, Mothers_Day, Fathers_Day)
SPREADVARIANT_VARIANTS(
    DayOfWeek,
    Monday,
    DayOfWeekHoliday,
    Martin_Luther_King_Jr_Day,
    Lincoln_Washington_Day,
    Memorial_Day,
    Labor_Day,
    Columbus_Day
)
SPREADVARIANT_VARIANTS(DayOfWeek, Thursday, DayOfWeekHoliday, Thanksgiving_Day)
SPREADVARIANT_VARIANTS(DayOfWeekOccurence, first, DayOfWeekHoliday, Labor_Day)
SPREADVARIANT_VARIANTS(DayOfWeekOccurence, second, DayOfWeekHoliday, Mothers_Day, Columbus_Day)
SPREADVARIANT_VARIANTS(
    DayOfWeekOccurence,
    third,
    DayOfWeekHoliday,
    Martin_Luther_King_Jr_Day,
    Lincoln_Washington_Day,
    Fathers_Day
)
SPREADVARIANT_VARIANTS(DayOfWeekOccurence, fourth, DayOfWeekHoliday, Thanksgiving_Day)
SPREADVARIANT_VARIANTS(DayOfWeekOccurence, last, DayOfWeekHoliday, Memorial_Day)
SPREADVARIANT_VARIANTS(Month, January, DayOfWeekHoliday, Martin_Luther_King_Jr_Day)
SPREADVARIANT_VARIANTS(Month, Februrary, DayOfWeekHoliday, Lincoln_Washington_Day)
SPREADVARIANT_VARIANTS(Month, May, DayOfWeekHoliday, Mothers_Day, Memorial_Day)
SPREADVARIANT_VARIANTS(Month, June, DayOfWeekHoliday, Fathers_Day)
SPREADVARIANT_VARIANTS(Month, September, DayOfWeekHoliday, Labor_Day)
SPREADVARIANT_VARIANTS(Month, October, DayOfWeekHoliday, Columbus_Day)
SPREADVARIANT_VARIANTS(Month, November, DayOfWeekHoliday, Thanksgiving_Day)

SPREADx2_MATCHABLE(
    DayOfMonth,
    Month,
    DayOfMonthHoliday,
    New_Years_Day,
    Independence_Day,
    Veterans_Day,
    Christmas_Day
)
SPREADVARIANT_VARIANTS(DayOfMonth, _1st, DayOfMonthHoliday, New_Years_Day)
SPREADVARIANT_VARIANTS(DayOfMonth, _4th, DayOfMonthHoliday, Independence_Day)
SPREADVARIANT_VARIANTS(DayOfMonth, _11th, DayOfMonthHoliday, Veterans_Day)
SPREADVARIANT_VARIANTS(DayOfMonth, _25th, DayOfMonthHoliday, Christmas_Day)
SPREADVARIANT_VARIANTS(Month, January, DayOfMonthHoliday, New_Years_Day)
SPREADVARIANT_VARIANTS(Month, July, DayOfMonthHoliday, Independence_Day)
SPREADVARIANT_VARIANTS(Month, November, DayOfMonthHoliday, Veterans_Day)
SPREADVARIANT_VARIANTS(Month, December, DayOfMonthHoliday, Christmas_Day)

MATCHABLES_MERGE_SPREADS(DayOfWeekHoliday, DayOfMonthHoliday, Holiday, Month)


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
    UNMATCHABLE(DayOfWeek, Sunday, Saturday)

    std::cout << "***** print_weekdays() *****" << std::endl;
    for (DayOfWeek::Type d : DayOfWeek::variants())
        std::cout << "    " << d << std::endl;
    std::cout << std::endl;
}


void print_day_of_week_holidays()
{
    std::cout << "***** print_day_of_week_holidays() *****" << std::endl;
    for (DayOfWeekHoliday::Type h : DayOfWeekHoliday::variants())
        std::cout << "    " << h.as_text() << " is celebrated on the " << h.as_DayOfWeekOccurence() << " "
                  << h.as_DayOfWeek() << " in " << h.as_Month() << std::endl;
    std::cout << std::endl;
}


void print_day_of_month_holidays()
{
    std::cout << "***** print_day_of_month_holidays() *****" << std::endl;
    for (DayOfMonthHoliday::Type h : DayOfMonthHoliday::variants())
        std::cout << "    " << h.as_text() << " is celebrated on " << h.as_Month() << " "
                  << h.as_DayOfMonth().as_index() + 1 << std::endl;
    std::cout << std::endl;
}


// demo MATCHABLES_MERGE_SPREADS()
void print_holidays_v0()
{
    std::cout << "***** print_holidays_v0() *****" << std::endl;
    for (Holiday::Type h : Holiday::variants())
        std::cout << "    " << h.as_text() << " is celebrated in " << h.as_Month() << std::endl;
    std::cout << std::endl;
}
void print_holiday(DayOfWeekHoliday::Type h)
{
    std::cout << "    " << h.as_text() << " is celebrated on the " << h.as_DayOfWeekOccurence() << " "
              << h.as_DayOfWeek() << " in " << h.as_Month() << std::endl;
}
void print_holiday(DayOfMonthHoliday::Type h)
{
    std::cout << "    " << h.as_text() << " is celebrated on " << h.as_Month()
              << h.as_DayOfMonth().as_text() << std::endl;
}
void print_holidays_v1()
{
    std::cout << "***** print_holidays_v1() *****" << std::endl;
    auto holidays = Holiday::variants();
    std::sort(holidays.begin(), holidays.end(), [](auto a, auto b) { return a.lt_by_string(b); });
    for (Holiday::Type h : holidays)
    {
        auto matchables = h.as_matchables();
        if (!std::get<0>(matchables).is_nil())
            print_holiday(std::get<0>(matchables));
        else
            print_holiday(std::get<1>(matchables));
    }
    std::cout << std::endl;
}


void print_friday_thirteenth_2007()
{
    std::cout << "***** print_friday_thirteenth_2007() *****" << std::endl;

    DayOfWeek::Type dow = DayOfWeek::from_string("Friday");
    DayOfMonth::Type dom = DayOfMonth::_13th::grab();
    Month::Type month = Month::from_index(6);

    std::cout << dow << ", " << month << " " << dom.as_index() + 1 << ", 2007" << std::endl;
    std::cout << dow << " the" << dom.as_text() << ", " << month << " 2007" << std::endl;
}


int main()
{
    print_weekdays();
    print_day_of_week_holidays();
    print_day_of_month_holidays();
    print_holidays_v0();
    print_holidays_v1();
    print_friday_thirteenth_2007();
    return 0;
}
