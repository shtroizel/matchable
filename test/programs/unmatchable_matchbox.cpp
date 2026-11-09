#include <iostream>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Color, red, green, blue)



int main()
{
    test_ok ok;

    matchable::MatchBox<Color::Type, int> outlived;

    {
        UNMATCHABLE(Color, green);

        Color::Flags f;
        f.set(Color::red::grab());
        f.set(Color::blue::grab());
        TEST_EQ(ok, f.is_set(Color::red::grab()), true);
        TEST_EQ(ok, f.is_set(Color::green::grab()), false);
        TEST_EQ(ok, f.is_set(Color::blue::grab()), true);
        TEST_EQ(ok, (int) f.currently_set().size(), 2);

        matchable::MatchBox<Color::Type, int> mb;
        mb.set(Color::red::grab(), 1);
        mb.set(Color::blue::grab(), 3);
        TEST_EQ(ok, mb.at(Color::red::grab()), 1);
        TEST_EQ(ok, mb.at(Color::blue::grab()), 3);
        TEST_EQ(ok, mb.is_set(Color::green::grab()), false);
        TEST_EQ(ok, (int) mb.currently_set().size(), 2);

        outlived = mb;
    }

    outlived.set(Color::green::grab(), 2);
    TEST_EQ(ok, outlived.at(Color::red::grab()), 1);
    TEST_EQ(ok, outlived.at(Color::green::grab()), 2);
    TEST_EQ(ok, outlived.at(Color::blue::grab()), 3);
    TEST_EQ(ok, (int) outlived.currently_set().size(), 3);

    return ok();
}
