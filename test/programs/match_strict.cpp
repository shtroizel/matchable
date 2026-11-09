#include <iostream>
#include <string>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Mode, wrap, always)
MATCHABLE(Grown, a)
GROW_MATCHABLE(Grown, b)



int main()
{
    test_ok ok;
    std::string ran;

    Mode::Type::MatchParam const full{
        {Mode::wrap::grab(), [&]() { ran = "wrap"; }},
        {Mode::always::grab(), [&]() { ran = "always"; }},
    };
    Mode::Type::MatchParam full_with_nil{full};
    full_with_nil.set(Mode::nil, [&]() { ran = "nil"; });
    Mode::Type::MatchParam const gap{
        {Mode::wrap::grab(), [&]() { ran = "wrap"; }},
    };
    Mode::Type::MatchParam gap_with_nil{gap};
    gap_with_nil.set(Mode::nil, [&]() { ran = "nil"; });

    ran.clear(); Mode::wrap::grab().match_strict(full);
    TEST_EQ(ok, ran, std::string{"wrap"});
    ran.clear(); Mode::always::grab().match_strict(full);
    TEST_EQ(ok, ran, std::string{"always"});
    ran.clear(); Mode::nil.match_strict(full);
    TEST_EQ(ok, ran, std::string{""});
    ran.clear(); Mode::nil.match_strict(full_with_nil);
    TEST_EQ(ok, ran, std::string{"nil"});

    ran.clear(); Mode::wrap::grab().match_strict(gap_with_nil);
    TEST_EQ(ok, ran, std::string{"nil"});
    ran.clear(); Mode::always::grab().match_strict(gap_with_nil);
    TEST_EQ(ok, ran, std::string{"nil"});
    ran.clear(); Mode::wrap::grab().match_strict(gap);
    TEST_EQ(ok, ran, std::string{""});

    ran.clear(); Mode::wrap::grab().match(gap);
    TEST_EQ(ok, ran, std::string{"wrap"});

    {
        Mode::Type::MatchParamWithFlowControl const fc_full{
            {Mode::wrap::grab(), [](matchable::FlowControl & fc) { fc.cont(); }},
            {Mode::always::grab(), [](matchable::FlowControl & fc) { fc.cont(); }},
        };
        Mode::Type::MatchParamWithFlowControl fc_gap{
            {Mode::wrap::grab(), [](matchable::FlowControl & fc) { fc.cont(); }},
        };
        TEST_EQ(ok, Mode::wrap::grab().match_strict(fc_full).cont_requested(), true);
        TEST_EQ(ok, Mode::wrap::grab().match_strict(fc_gap).cont_requested(), false);
        fc_gap.set(Mode::nil, [](matchable::FlowControl & fc) { fc.brk(); });
        TEST_EQ(ok, Mode::wrap::grab().match_strict(fc_gap).brk_requested(), true);
        TEST_EQ(ok, Mode::wrap::grab().match_strict(fc_gap).cont_requested(), false);
    }

    {
        Grown::Type::MatchParam grown_gap{
            {Grown::a::grab(), [&]() { ran = "a"; }},
        };
        ran.clear(); Grown::a::grab().match_strict(grown_gap);
        TEST_EQ(ok, ran, std::string{""});
        grown_gap.set(Grown::b::grab(), [&]() { ran = "b"; });
        ran.clear(); Grown::a::grab().match_strict(grown_gap);
        TEST_EQ(ok, ran, std::string{"a"});
    }

    return ok();
}
