#include <iostream>

#include "matchable/matchable.h"
#include "test_ok.h"



PROPERTYx1_MATCHABLE(int, n, Obs, one)



int main()
{
    test_ok ok;
    auto one = Obs::one::grab();

    {
        int once{0};
        int after{0};
        one.add_n_observer("once", [&]() { one.del_n_observer("once"); ++once; });
        one.add_n_observer("z_after", [&]() { ++after; });
        one.set_n(1);
        one.set_n(2);
        TEST_EQ(ok, once, 1);
        TEST_EQ(ok, after, 2);
        one.del_n_observer("z_after");
    }

    {
        int added{0};
        one.add_n_observer("adder", [&]() { one.add_n_observer("z_added", [&]() { ++added; }); });
        one.set_n(3);
        TEST_EQ(ok, added, 0);
        one.set_n(4);
        TEST_EQ(ok, added, 1);
        one.del_n_observer("adder");
        one.del_n_observer("z_added");
    }

    {
        int victim{0};
        one.add_n_observer("a_deleter", [&]() { one.del_n_observer("b_victim"); });
        one.add_n_observer("b_victim", [&]() { ++victim; });
        one.set_n(5);
        TEST_EQ(ok, victim, 0);
        one.del_n_observer("a_deleter");
    }

    {
        int once{0};
        int after{0};
        one.add_n_vect_observer("once", [&]() { one.del_n_vect_observer("once"); ++once; });
        one.add_n_vect_observer("z_after", [&]() { ++after; });
        one.set_n_vect({1});
        one.set_n_vect({2});
        TEST_EQ(ok, once, 1);
        TEST_EQ(ok, after, 2);
    }

    {
        int once{0};
        int after{0};
        Obs::Type nil;
        nil.add_n_observer("once", [&]() { Obs::Type{}.del_n_observer("once"); ++once; });
        nil.add_n_observer("z_after", [&]() { ++after; });
        nil.set_n(1);
        nil.set_n(2);
        TEST_EQ(ok, once, 1);
        TEST_EQ(ok, after, 2);
    }

    {
        int once{0};
        int after{0};
        Obs::Type nil;
        nil.add_n_vect_observer("once", [&]() { Obs::Type{}.del_n_vect_observer("once"); ++once; });
        nil.add_n_vect_observer("z_after", [&]() { ++after; });
        nil.set_n_vect({1});
        nil.set_n_vect({2});
        TEST_EQ(ok, once, 1);
        TEST_EQ(ok, after, 2);
    }

    return ok();
}
