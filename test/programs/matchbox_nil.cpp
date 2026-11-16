#include <iostream>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Color, red, green)



int main()
{
    test_ok ok;

    {
        matchable::MatchBox<Color::Type, int> mb(5);
        TEST_EQ(ok, mb.at(Color::red::grab()), 5);
        TEST_EQ(ok, mb.at(Color::nil), 5);
        TEST_EQ(ok, mb.is_set(Color::nil), false);

        mb.set(Color::nil, 7);
        TEST_EQ(ok, mb.at(Color::nil), 7);
        TEST_EQ(ok, mb.is_set(Color::nil), true);

        mb.unset(Color::nil);
        TEST_EQ(ok, mb.at(Color::nil), 5);
        TEST_EQ(ok, mb.is_set(Color::nil), false);
    }

    {
        matchable::MatchBox<Color::Type, int> const mb(5);
        TEST_EQ(ok, mb.at(Color::nil), 5);
    }

    {
        matchable::MatchBox<Color::Type, int> mb(5);
        TEST_EQ(ok, mb.mut_at(Color::nil), 5);
    }

    {
        matchable::MatchBox<Color::Type, int> mb;
        TEST_EQ(ok, mb.at(Color::nil), 0);
    }

    return ok();
}
