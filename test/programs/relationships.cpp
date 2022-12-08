#include <iostream>

#include "matchable/matchable.h"


MATCHABLE(DayOfWeekOccurrence, first, second, third, fourth, last)

MATCHABLE(DayOfWeek, Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday);

PROPERTYx1_MATCHABLE(DayOfWeek::Type, day_of_week, Days, Weekdays, Weekend)
MATCHABLE_VARIANT_PROPERTY_VALUES(
    Days,
    Weekdays,
    day_of_week,
    DayOfWeek::Monday::grab(),
    DayOfWeek::Tuesday::grab(),
    DayOfWeek::Wednesday::grab(),
    DayOfWeek::Thursday::grab(),
    DayOfWeek::Friday::grab()
)
MATCHABLE_VARIANT_PROPERTY_VALUES(Days, Weekend, day_of_week, DayOfWeek::Sunday::grab(), DayOfWeek::Saturday::grab())

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

PROPERTYx4_MATCHABLE(
    DayOfWeek::Type,
    day_of_week,
    DayOfWeekOccurrence::Type,
    occurrence,
    DayOfMonth::Type,
    day_of_month,
    Month::Type,
    month,
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
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Mothers_spc_Day, day_of_week, DayOfWeek::Sunday::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Mothers_spc_Day, occurrence, DayOfWeekOccurrence::second::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Mothers_spc_Day, month, Month::May::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Fathers_spc_Day, day_of_week, DayOfWeek::Sunday::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Fathers_spc_Day, occurrence, DayOfWeekOccurrence::third::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Fathers_spc_Day, month, Month::June::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Martin_spc_Luther_spc_King_spc_Jr_spc_Day, day_of_week, DayOfWeek::Monday::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Martin_spc_Luther_spc_King_spc_Jr_spc_Day,
             occurrence, DayOfWeekOccurrence::third::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Martin_spc_Luther_spc_King_spc_Jr_spc_Day, month, Month::January::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Lincoln_spc_Washington_spc_Day, day_of_week, DayOfWeek::Monday::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Lincoln_spc_Washington_spc_Day, occurrence, DayOfWeekOccurrence::third::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Lincoln_spc_Washington_spc_Day, month, Month::Februrary::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Memorial_spc_Day, day_of_week, DayOfWeek::Monday::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Memorial_spc_Day, occurrence, DayOfWeekOccurrence::last::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Memorial_spc_Day, month, Month::May::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Labor_spc_Day, day_of_week, DayOfWeek::Monday::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Labor_spc_Day, occurrence, DayOfWeekOccurrence::first::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Labor_spc_Day, month, Month::September::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Columbus_spc_Day, day_of_week, DayOfWeek::Monday::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Columbus_spc_Day, occurrence, DayOfWeekOccurrence::second::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Columbus_spc_Day, month, Month::October::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Thanksgiving_spc_Day, day_of_week, DayOfWeek::Thursday::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Thanksgiving_spc_Day, occurrence, DayOfWeekOccurrence::fourth::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Thanksgiving_spc_Day, month, Month::November::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, New_spc_Years_spc_Day, day_of_month, DayOfMonth::esc_1st::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, New_spc_Years_spc_Day, month, Month::January::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Independence_spc_Day, day_of_month, DayOfMonth::esc_4th::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Independence_spc_Day, month, Month::July::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Veterans_spc_Day, day_of_month, DayOfMonth::esc_11th::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Veterans_spc_Day, month, Month::November::grab())

MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Christmas_spc_Day, day_of_month, DayOfMonth::esc_25th::grab())
MATCHABLE_VARIANT_PROPERTY_VALUE(Holiday, Christmas_spc_Day, month, Month::December::grab())



bool is_weekday_v0(DayOfWeek::Type day)
{
    return MATCHABLE_INSTANCE_IN(DayOfWeek, day, Monday, Tuesday, Wednesday, Thursday, Friday);
}


bool is_weekday_v1(DayOfWeek::Type day)
{
    auto const & v = Days::Weekdays().grab().as_day_of_week_vect();
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
        if (h.as_occurrence().is_nil())
            std::cout << "    " << h << " is celebrated on " << h.as_month()
                      << " " << h.as_day_of_month() << std::endl;
        else
            std::cout << "    " << h << " is celebrated on the " << h.as_occurrence()
                      << " " << h.as_day_of_week() << " of " << h.as_month() << std::endl;
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
