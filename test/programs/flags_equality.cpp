#include <iostream>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Color, red, green, blue)



int main()
{
    test_ok ok;

    Color::Flags a;
    Color::Flags b;
    TEST_EQ(ok, a == b, true);
    TEST_EQ(ok, a != b, false);

    b.set(Color::nil);
    TEST_EQ(ok, a == b, false);
    TEST_EQ(ok, a != b, true);
    TEST_EQ(ok, b == a, false);

    a.toggle(Color::nil);
    TEST_EQ(ok, a == b, true);

    a.set(Color::green::grab());
    b.set(Color::green::grab());
    TEST_EQ(ok, a == b, true);

    b.unset(Color::nil);
    TEST_EQ(ok, a == b, false);
    TEST_EQ(ok, a == Color::Flags({Color::nil, Color::green::grab()}), true);

    a.clear();
    b.clear();
    TEST_EQ(ok, a == b, true);

    return ok();
}
