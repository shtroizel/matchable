#include <iostream>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Color, red, green, blue)



int main()
{
    test_ok ok;

    {
        UNMATCHABLE(Color, blue);

        TEST_EQ(ok, (int) Color::variants_by_string().size(), 2);
        TEST_EQ(ok, Color::green::grab().as_by_string_index(), 1);
        TEST_EQ(ok, Color::red::grab().as_by_string_index(), 2);
        for (auto const & c : Color::variants_by_string())
            TEST_EQ(ok, Color::from_by_string_index(c.as_by_string_index()), c);

        TEST_EQ(ok, Color::from_by_string_index(0), Color::nil);

        bool found{false};
        int index = Color::variants_by_string_index_of("red", &found);
        TEST_EQ(ok, found, true);
        TEST_EQ(ok, index, 2);
        TEST_EQ(ok, Color::from_by_string_index(index), Color::red::grab());

        Color::variants_by_string_index_of("blue", &found);
        TEST_EQ(ok, found, false);
        TEST_EQ(ok, Color::from_string("blue"), Color::nil);
    }

    TEST_EQ(ok, (int) Color::variants_by_string().size(), 3);
    TEST_EQ(ok, Color::from_by_string_index(0), Color::blue::grab());
    TEST_EQ(ok, Color::from_by_string_index(2), Color::red::grab());

    return ok();
}
