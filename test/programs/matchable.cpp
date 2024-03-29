#include <iostream>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(TimeUnit, Seconds, Minutes, Hours, Days, Weeks)

MATCHABLE(Result, Ok, Err)

// matchable without variants?
MATCHABLE(NIL)


enum class Task { Task0, Task1, Task2 };



Result::Type foo(int number)
{
    if (number == 116)
        return Result::Ok::grab();
    return Result::Err::grab();
}


int main()
{
    test_ok ok;

    // default construction
    TimeUnit::Type time_unit;
    TEST_EQ(ok, time_unit, TimeUnit::nil);

    // assignment
    time_unit = TimeUnit::Minutes::grab();
    TEST_EQ(ok, time_unit, TimeUnit::Minutes::grab());

    // copy construction
    TimeUnit::Type time_unit_copied{time_unit};
    TEST_EQ(ok, time_unit_copied, time_unit);
    TEST_EQ(ok, time_unit, TimeUnit::Minutes::grab());

    // move construction
    TimeUnit::Type another_time_unit{std::move(time_unit)};
    TEST_EQ(ok, time_unit, TimeUnit::nil);
    TEST_EQ(ok, another_time_unit, TimeUnit::Minutes::grab());

    // copy assignment
    TimeUnit::Type another_time_unit_copied = time_unit_copied;
    TEST_EQ(ok, another_time_unit_copied, time_unit_copied);
    TEST_EQ(ok, another_time_unit_copied, TimeUnit::Minutes::grab());

    // move assignment
    time_unit = std::move(another_time_unit);
    TEST_EQ(ok, time_unit, TimeUnit::Minutes::grab());
    TEST_EQ(ok, another_time_unit, TimeUnit::nil);

    // compare operators
    another_time_unit = TimeUnit::Seconds::grab();
#ifndef MATCHABLE_OMIT_BY_INDEX
    if (time_unit < another_time_unit) // index compare! (same as lt_by_index())
        TEST_FAIL(ok);

    if (time_unit.lt_by_index(another_time_unit))
        TEST_FAIL(ok);
#endif

    if (another_time_unit.lt_by_string(time_unit))
        TEST_FAIL(ok);

    if (another_time_unit == time_unit)
        TEST_FAIL(ok);

    another_time_unit = time_unit;
    if (another_time_unit != time_unit)
        TEST_FAIL(ok);

    // as_string()
    TEST_EQ(ok, time_unit.as_string(), std::string("Minutes"));
    TEST_EQ(ok, TimeUnit::Type().as_string(), std::string("nil"));

    // from_string()
    TEST_EQ(ok, TimeUnit::from_string("116"), TimeUnit::nil);
    TEST_EQ(ok, TimeUnit::from_string("nil"), TimeUnit::nil);
    TEST_EQ(ok, TimeUnit::from_string("Weeks"), TimeUnit::Weeks::grab());

    // variants_by_string_index_of()
    {
        bool found;
        auto tu = TimeUnit::from_by_string_index(TimeUnit::variants_by_string_index_of("116", &found));
        TEST_EQ(ok, tu, TimeUnit::Days::grab());
        TEST_EQ(ok, found, false);
        int index = TimeUnit::variants_by_string_index_of("Years", &found);
        tu = TimeUnit::from_by_string_index(index);
        TEST_EQ(ok, tu, TimeUnit::nil);
        TEST_EQ(ok, index, (int) TimeUnit::variants().size());
        TEST_EQ(ok, found, false);
        tu = TimeUnit::from_by_string_index(TimeUnit::variants_by_string_index_of("Fortnites", nullptr));
        TEST_EQ(ok, tu, TimeUnit::Hours::grab());
        tu = TimeUnit::from_by_string_index(TimeUnit::variants_by_string_index_of("Minutes", &found));
        TEST_EQ(ok, tu, TimeUnit::Minutes::grab());
        TEST_EQ(ok, found, true);
        tu = TimeUnit::from_by_string_index(TimeUnit::variants_by_string_index_of("Days", &found));
        TEST_EQ(ok, tu, TimeUnit::Days::grab());
        TEST_EQ(ok, found, true);
        tu = TimeUnit::from_by_string_index(TimeUnit::variants_by_string_index_of("Weeks", &found));
        TEST_EQ(ok, tu, TimeUnit::Weeks::grab());
        TEST_EQ(ok, found, true);
    }

#ifndef MATCHABLE_OMIT_BY_INDEX
    // as_index()
    TEST_EQ(ok, TimeUnit::Seconds::grab().as_index(), 0);
    TEST_EQ(ok, TimeUnit::Minutes::grab().as_index(), 1);
    TEST_EQ(ok, TimeUnit::Hours::grab().as_index(), 2);
    TEST_EQ(ok, TimeUnit::Days::grab().as_index(), 3);
    TEST_EQ(ok, TimeUnit::Weeks::grab().as_index(), 4);
    TEST_EQ(ok, NIL::from_string("116").as_index(), -1);

    // from_index()
    TEST_EQ(ok, TimeUnit::from_index(-1), TimeUnit::nil);
    for (auto tu : TimeUnit::variants())
        TEST_EQ(ok, tu, TimeUnit::from_index(tu.as_index()));
    TEST_EQ(ok, TimeUnit::from_index(116), TimeUnit::nil);
#endif

    // as_by_string_index()
    TEST_EQ(ok, TimeUnit::Seconds::grab().as_by_string_index(), 3);
    TEST_EQ(ok, TimeUnit::Minutes::grab().as_by_string_index(), 2);
    TEST_EQ(ok, TimeUnit::Hours::grab().as_by_string_index(), 1);
    TEST_EQ(ok, TimeUnit::Days::grab().as_by_string_index(), 0);
    TEST_EQ(ok, TimeUnit::Weeks::grab().as_by_string_index(), 4);
    TEST_EQ(ok, TimeUnit::nil.as_by_string_index(), -1);

    // is_nil()
    TEST_EQ(ok, time_unit.is_nil(), false);

    // Flags
    TimeUnit::Flags flags;
    for (auto const & tu : TimeUnit::variants())
        TEST_EQ(ok, flags.is_set(tu), false);
    flags.set(TimeUnit::Seconds::grab());
    time_unit = TimeUnit::Seconds::grab();
    TEST_EQ(ok, flags.is_set(time_unit), true);
    TimeUnit::Flags more_flags{flags};
    flags.unset(TimeUnit::Seconds::grab());
    TEST_EQ(ok, more_flags.is_set(time_unit), true);
    TEST_EQ(ok, flags.is_set(time_unit), false);
    TEST_NE(ok, flags, more_flags);
    flags.set(TimeUnit::Hours::grab());
    flags.set(TimeUnit::Seconds::grab());
    more_flags.set(TimeUnit::Hours::grab());
    TEST_EQ(ok, flags, more_flags);
    more_flags.clear();
    TEST_EQ(ok, more_flags, TimeUnit::Flags{});
    more_flags.toggle(TimeUnit::Seconds::grab());
    TEST_EQ(ok, more_flags.is_set(TimeUnit::Seconds::grab()), true);
    more_flags.toggle(TimeUnit::Seconds::grab());
    TEST_EQ(ok, more_flags.is_set(TimeUnit::Seconds::grab()), false);

    // match() by its self - not used within switch or loop...
    int magic{116};
    foo(magic).match({
        {Result::Ok::grab(), [&]() {std::cout << "foo(" << magic << ") is ok!" << std::endl;}},
        {Result::Err::grab(), [&](){std::cout << "foo(" << magic << ") error" << std::endl; TEST_FAIL(ok);}}
    });

    // match() used within a loop
    magic = 99;
    while (magic < 999)
    {
        magic++;
        MATCH_WITH_FLOW_CONTROL foo(magic).match({
            {Result::Ok::grab(),  [](matchable::FlowControl & fc){ fc.brk();}}, // break
            {Result::Err::grab(), [&](matchable::FlowControl & fc)
                                  {
                                      if (magic < 105)
                                      {
                                          fc.cont();
                                          // note need to return here
                                          // since fc is evaluateded after this function returns
                                          return;
                                      }
                                      std::cout << magic << " failed..." << std::endl;
                                  }}
        }); EVAL_FLOW_CONTROL // apply break or continue requested from lambda above
    }
    TEST_EQ(ok, magic, 116);

    // match used within a switch
    Task task = Task::Task0;
    switch (task)
    {
        case Task::Task0: MATCH_WITH_FLOW_CONTROL foo(116).match({
                              {Result::Ok::grab(), [](matchable::FlowControl & fc) { fc.brk(); }},
                          }); EVAL_BREAK_ONLY // not in loop here so continue is invalid - eval break only
                          [[fallthrough]];
        case Task::Task1: TEST_FAIL(ok); break;
        case Task::Task2: TEST_FAIL(ok);
    }

    // match used within a switch that is within a loop
    magic = 99;
    while (magic++ < 999)
    {
        switch (task)
        {
            case Task::Task0: MATCH_WITH_FLOW_CONTROL foo(magic).match({
                                  {Result::Ok::grab(), [](matchable::FlowControl & fc) { fc.brk(); }},
                                  {Result::Err::grab(), [](matchable::FlowControl & fc) { fc.cont(); }},
                              }); EVAL_FLOW_CONTROL
                              [[fallthrough]];
            case Task::Task1: MATCH_WITH_FLOW_CONTROL foo(116).match({
                                  {Result::Ok::grab(), [](matchable::FlowControl & fc) { fc.cont(); }},
                              }); EVAL_FLOW_CONTROL
                              [[fallthrough]];
            case Task::Task2: TEST_FAIL(ok);
        }
    }

    // traversal, variants(), operator<<()
    TEST_EQ(ok, TimeUnit::variants().size(), static_cast<size_t>(5));
    std::cout << "Traversing variants..." << std::endl;
    for (auto const & variant : TimeUnit::variants())
        std::cout << "    " << variant << std::endl;

    // matchable without variants
    NIL::Type n;
    TEST_EQ(ok, n.is_nil(), true);

    return ok();
}
