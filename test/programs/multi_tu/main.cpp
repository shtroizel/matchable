#include <iostream>
#include <string>
#include <vector>

#include "tab.h"
#include "test_ok.h"



GROW_MATCHABLE(Tab, alpha)
GROW_MATCHABLE(Tab, zulu)
GROW_MATCHABLE(Keyed, zulu)



int main()
{
    test_ok ok;

    TEST_EQ(ok, (int) Tab::variants().size(), 2);
    TEST_EQ(ok, (int) Tab::variants_by_string().size(), 2);
    TEST_EQ(ok, plain_variant_count(), 2);
    TEST_EQ(ok, Tab::from_string("alpha"), Tab::alpha::grab());
    TEST_EQ(ok, Tab::from_string("zulu"), Tab::zulu::grab());

    TEST_EQ(ok, Tab::alpha::grab().as_by_string_index(), 0);
    TEST_EQ(ok, Tab::zulu::grab().as_by_string_index(), 1);
    TEST_EQ(ok, Tab::from_by_string_index(0), Tab::alpha::grab());
    TEST_EQ(ok, Tab::from_by_string_index(1), Tab::zulu::grab());

#ifndef MATCHABLE_OMIT_BY_INDEX
    int const a = Tab::alpha::grab().as_index();
    int const z = Tab::zulu::grab().as_index();
    TEST_NE(ok, a, z);
    TEST_EQ(ok, a == 0 || a == 1, true);
    TEST_EQ(ok, z == 0 || z == 1, true);
    TEST_EQ(ok, Tab::from_index(a), Tab::alpha::grab());
    TEST_EQ(ok, Tab::from_index(z), Tab::zulu::grab());
#endif

    std::vector<std::string> keyed;
    for (auto const & k : Keyed::variants_by_string())
        keyed.push_back(k.as_string());
    TEST_EQ(ok, keyed == (std::vector<std::string>{"ALPHA", "alpha", "Bravo", "bravo", "Zulu", "zulu"}), true);
    for (int i = 0; i < (int) Keyed::variants_by_string().size(); ++i)
    {
        TEST_EQ(ok, Keyed::variants_by_string()[i].as_by_string_index(), i);
        TEST_EQ(ok, Keyed::from_string(keyed[i]), Keyed::variants_by_string()[i]);
    }

    TEST_EQ(ok, Tab::zulu::grab().as_id(), 26);
    TEST_EQ(ok, Tab::alpha::grab().as_id(), 0);

    Tab::alpha::grab().set_id(1);
    TEST_EQ(ok, Tab::alpha::grab().as_id(), 1);
    TEST_EQ(ok, Tab::zulu::grab().as_id(), 26);
    Tab::zulu::grab().set_id(2);
    TEST_EQ(ok, Tab::alpha::grab().as_id(), 1);
    TEST_EQ(ok, Tab::zulu::grab().as_id(), 2);

    Tab::Flags f;
    f.set(Tab::zulu::grab());
    TEST_EQ(ok, f.is_set(Tab::zulu::grab()), true);
    TEST_EQ(ok, f.is_set(Tab::alpha::grab()), false);
    f.set(Tab::alpha::grab());
    TEST_EQ(ok, (int) f.currently_set().size(), 2);

    return ok();
}
