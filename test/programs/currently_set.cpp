#include <iostream>
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Color, red, green, blue)



int main()
{
    test_ok ok;

    {
        Color::Flags f;
        f.set(Color::blue::grab());
        TEST_EQ(ok, f.currently_set() == std::vector<Color::Type>({Color::blue::grab()}), true);

        f.set(Color::nil);
        TEST_EQ(ok, f.currently_set() == std::vector<Color::Type>({Color::blue::grab(), Color::nil}), true);

        f.unset(Color::blue::grab());
        TEST_EQ(ok, f.currently_set() == std::vector<Color::Type>({Color::nil}), true);

        f.unset(Color::nil);
        TEST_EQ(ok, f.currently_set().empty(), true);
    }

    {
        matchable::MatchBox<Color::Type, int> mb;
        mb.set(Color::red::grab(), 1);
        TEST_EQ(ok, mb.currently_set() == std::vector<Color::Type>({Color::red::grab()}), true);

        mb.set(Color::nil, 2);
        TEST_EQ(ok, mb.currently_set() == std::vector<Color::Type>({Color::red::grab(), Color::nil}), true);

        mb.unset(Color::nil);
        TEST_EQ(ok, mb.currently_set() == std::vector<Color::Type>({Color::red::grab()}), true);
    }

    return ok();
}
