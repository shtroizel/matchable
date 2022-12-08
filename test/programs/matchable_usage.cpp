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

    // oops, actually we don't like Wednessday after all
    favorite_days.unset(DayOfWeek::Wednessday::grab());

    // to get all flagged variants we can use currently_set()
    for (auto day : favorite_days.currently_set())
        if (day == day_of_week)
            std::cout << day << " is one of my favorite days! ";

    // we can also use is_set() to check for flagged variants
    if (favorite_days.is_set(DayOfWeek::Friday::grab()))
        return Status::esc_17::grab();

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
