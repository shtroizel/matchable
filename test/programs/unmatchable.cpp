#include <iostream>
#include <vector>

#include "matchable/matchable_fwd.h"
#include "matchable/matchable.h"
#include "test_ok.h"



// test forward declation with namespaces
MATCHABLE_FWD(NoNameSpace)
namespace outer_ns
{
    MATCHABLE_FWD(OuterMatchable)
    namespace middle_outer_ns
    {
        MATCHABLE_FWD(MiddleOuterMatchable)
        namespace middle_ns
        {
            MATCHABLE_FWD(MiddleMatchable)
            namespace middle_inner_ns
            {
                MATCHABLE_FWD(MiddleInnerMatchable)
                namespace inner_ns
                {
                    MATCHABLE_FWD(InnerMatchable)
                    namespace core_ns
                    {
                        MATCHABLE_FWD(CoreMatchable)
                    }
                }
            }
        }
    }
}
void f0(NoNameSpace::Type const &) {}
void f1(outer_ns::OuterMatchable::Type const &) {}
void f2(outer_ns::middle_outer_ns::MiddleOuterMatchable::Type const &) {}
void f3(outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::Type const &) {}
void f4(outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::Type const &) {}
void f5(outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::Type const &) {}


MATCHABLE(NoNameSpace, zero, one)
namespace outer_ns
{
    MATCHABLE(OuterMatchable, two, three)
    namespace middle_outer_ns
    {
        MATCHABLE(MiddleOuterMatchable, four, five)
        namespace middle_ns
        {
            MATCHABLE(MiddleMatchable, six, seven)
            namespace middle_inner_ns
            {
                MATCHABLE(MiddleInnerMatchable, eight, nine)
                namespace inner_ns
                {
                    MATCHABLE(InnerMatchable, ten, eleven)
                    namespace core_ns
                    {
                        MATCHABLE(CoreMatchable, twelve, thirteen)
                    }
                }
            }
        }
    }
}


MATCHABLE(
    Digit,

    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine
)


int main()
{
    test_ok ok;

    // Using Unmatchable directly, no macros, no namespaces
    TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 2);
    {
        matchable::Unmatchable<NoNameSpace::Type> unmatchable{
            {NoNameSpace::zero::grab()},
        };
        TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 1);
        TEST_EQ(ok, NoNameSpace::variants()[0], NoNameSpace::one::grab());
        {
            matchable::Unmatchable<NoNameSpace::Type> unmatchable{
                {NoNameSpace::one::grab()},
            };
            TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 0);
        }
        TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 1);
        TEST_EQ(ok, NoNameSpace::variants()[0], NoNameSpace::one::grab());
    }
    TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 2);
    TEST_EQ(ok, NoNameSpace::variants()[0], NoNameSpace::zero::grab());
    TEST_EQ(ok, NoNameSpace::variants()[1], NoNameSpace::one::grab());


    // use macro, no namespaces
    TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 2);
    {
        UNMATCHABLE(NoNameSpace, zero);
        TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 1);
        TEST_EQ(ok, NoNameSpace::variants()[0], NoNameSpace::one::grab());
        {
            UNMATCHABLE(NoNameSpace, one);
            TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 0);
        }
        TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 1);
        TEST_EQ(ok, NoNameSpace::variants()[0], NoNameSpace::one::grab());
    }
    TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 2);
    TEST_EQ(ok, NoNameSpace::variants()[0], NoNameSpace::zero::grab());
    TEST_EQ(ok, NoNameSpace::variants()[1], NoNameSpace::one::grab());


    // use macro, one namespace
    TEST_EQ(ok, outer_ns::OuterMatchable::variants().size(), (size_t) 2);
    {
        NAMESPACEx1_UNMATCHABLE(outer_ns, OuterMatchable, two);
        TEST_EQ(ok, outer_ns::OuterMatchable::variants().size(), (size_t) 1);
        TEST_EQ(ok, outer_ns::OuterMatchable::variants()[0], outer_ns::OuterMatchable::three::grab());
        {
            NAMESPACEx1_UNMATCHABLE(outer_ns, OuterMatchable, three);
            TEST_EQ(ok, outer_ns::OuterMatchable::variants().size(), (size_t) 0);
        }
        TEST_EQ(ok, outer_ns::OuterMatchable::variants().size(), (size_t) 1);
        TEST_EQ(ok, outer_ns::OuterMatchable::variants()[0], outer_ns::OuterMatchable::three::grab());
    }
    TEST_EQ(ok, outer_ns::OuterMatchable::variants().size(), (size_t) 2);
    TEST_EQ(ok, outer_ns::OuterMatchable::variants()[0], outer_ns::OuterMatchable::two::grab());
    TEST_EQ(ok, outer_ns::OuterMatchable::variants()[1], outer_ns::OuterMatchable::three::grab());


    // use macro, two namespaces
    TEST_EQ(ok, outer_ns::middle_outer_ns::MiddleOuterMatchable::variants().size(), (size_t) 2);
    {
        NAMESPACEx2_UNMATCHABLE(outer_ns, middle_outer_ns, MiddleOuterMatchable, five);
        TEST_EQ(ok, outer_ns::middle_outer_ns::MiddleOuterMatchable::variants().size(), (size_t) 1);
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::MiddleOuterMatchable::variants()[0],
            outer_ns::middle_outer_ns::MiddleOuterMatchable::four::grab()
        );
        {
            NAMESPACEx2_UNMATCHABLE(outer_ns, middle_outer_ns, MiddleOuterMatchable, four);
            TEST_EQ(ok, outer_ns::middle_outer_ns::MiddleOuterMatchable::variants().size(), (size_t) 0);
        }
        TEST_EQ(ok, outer_ns::middle_outer_ns::MiddleOuterMatchable::variants().size(), (size_t) 1);
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::MiddleOuterMatchable::variants()[0],
            outer_ns::middle_outer_ns::MiddleOuterMatchable::four::grab()
        );
    }
    TEST_EQ(ok, outer_ns::middle_outer_ns::MiddleOuterMatchable::variants().size(), (size_t) 2);
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::MiddleOuterMatchable::variants()[0],
        outer_ns::middle_outer_ns::MiddleOuterMatchable::four::grab()
    );
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::MiddleOuterMatchable::variants()[1],
        outer_ns::middle_outer_ns::MiddleOuterMatchable::five::grab()
    );


    // use macro, three namespaces
    TEST_EQ(ok, outer_ns::middle_outer_ns::MiddleOuterMatchable::variants().size(), (size_t) 2);
    {
        NAMESPACEx3_UNMATCHABLE(outer_ns, middle_outer_ns, middle_ns, MiddleMatchable, six);
        TEST_EQ(ok, outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::variants().size(), (size_t) 1);
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::variants()[0],
            outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::seven::grab()
        );
        {
            NAMESPACEx3_UNMATCHABLE(outer_ns, middle_outer_ns, middle_ns, MiddleMatchable, seven);
            TEST_EQ(
                ok,
                outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::variants().size(),
                (size_t) 0
            );
        }
        TEST_EQ(ok, outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::variants().size(), (size_t) 1);
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::variants()[0],
            outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::seven::grab()
        );
    }
    TEST_EQ(ok, outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::variants().size(), (size_t) 2);
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::variants()[0],
        outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::six::grab()
    );
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::variants()[1],
        outer_ns::middle_outer_ns::middle_ns::MiddleMatchable::seven::grab()
    );


    // use macro, four namespaces
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::variants().size(),
        (size_t) 2
    );
    {
        NAMESPACEx4_UNMATCHABLE(
            outer_ns,
            middle_outer_ns,
            middle_ns,
            middle_inner_ns,
            MiddleInnerMatchable,
            eight
        );
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::variants().size(),
            (size_t) 1
        );
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::variants()[0],
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::nine::grab()
        );
        {
            NAMESPACEx4_UNMATCHABLE(
                outer_ns,
                middle_outer_ns,
                middle_ns,
                middle_inner_ns,
                MiddleInnerMatchable,
                nine
            );
            TEST_EQ(
                ok,
                outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                        ::MiddleInnerMatchable::variants().size(),
                (size_t) 0
            );
        }
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::variants().size(),
            (size_t) 1
        );
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::variants()[0],
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::nine::grab()
        );
    }
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::variants().size(),
        (size_t) 2
    );
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::variants()[0],
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::eight::grab()
    );
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::variants()[1],
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::MiddleInnerMatchable::nine::grab()
    );


    // use macro, five namespaces
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::variants().size(),
        (size_t) 2
    );
    {
        NAMESPACEx5_UNMATCHABLE(
            outer_ns,
            middle_outer_ns,
            middle_ns,
            middle_inner_ns,
            inner_ns,
            InnerMatchable,
            eleven
        );
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                    ::inner_ns::InnerMatchable::variants().size(),
            (size_t) 1
        );
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::variants()[0],
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::ten::grab()
        );
        {
            NAMESPACEx5_UNMATCHABLE(
                outer_ns,
                middle_outer_ns,
                middle_ns,
                middle_inner_ns,
                inner_ns,
                InnerMatchable,
                ten
            );
            TEST_EQ(
                ok,
                outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                        ::inner_ns::InnerMatchable::variants().size(),
                (size_t) 0
            );
        }
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                    ::inner_ns::InnerMatchable::variants().size(),
            (size_t) 1
        );
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::variants()[0],
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::ten::grab()
        );
    }
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::variants().size(),
        (size_t) 2
    );
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::variants()[0],
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::ten::grab()
    );
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::variants()[1],
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::InnerMatchable::eleven::grab()
    );



    // 6 namespaces!
    // but macro support ends with 5 namespaces...
    // so just use Unmatchable directly
    TEST_EQ(ok, outer_ns::middle_outer_ns::MiddleOuterMatchable::variants().size(), (size_t) 2);
    {
        matchable::Unmatchable<
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns::inner_ns::core_ns::CoreMatchable::Type
        > unmatchable{
            {outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                     ::inner_ns::core_ns::CoreMatchable::twelve::grab()},
        };
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                    ::inner_ns::core_ns::CoreMatchable::variants().size(),
            (size_t) 1
        );
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                    ::inner_ns::core_ns::CoreMatchable::variants()[0],
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                    ::inner_ns::core_ns::CoreMatchable::thirteen::grab()
        );
        {
            matchable::Unmatchable<
                outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                        ::inner_ns::core_ns::CoreMatchable::Type
            > unmatchable{
                {outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                         ::inner_ns::core_ns::CoreMatchable::thirteen::grab()},
            };
            TEST_EQ(
                ok,
                outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                        ::inner_ns::core_ns::CoreMatchable::variants().size(),
                (size_t) 0
            );
        }
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                    ::inner_ns::core_ns::CoreMatchable::variants().size(),
            (size_t) 1
        );
        TEST_EQ(
            ok,
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                    ::inner_ns::core_ns::CoreMatchable::variants()[0],
            outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                    ::inner_ns::core_ns::CoreMatchable::thirteen::grab()
        );
    }
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                ::inner_ns::core_ns::CoreMatchable::variants().size(),
        (size_t) 2
    );
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                ::inner_ns::core_ns::CoreMatchable::variants()[0],
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                ::inner_ns::core_ns::CoreMatchable::twelve::grab()
    );
    TEST_EQ(
        ok,
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                ::inner_ns::core_ns::CoreMatchable::variants()[1],
        outer_ns::middle_outer_ns::middle_ns::middle_inner_ns
                ::inner_ns::core_ns::CoreMatchable::thirteen::grab()
    );


    {
        UNMATCHABLE(Digit, Zero, One, Two);

        int i = 3;
        for (auto d : Digit::variants())
        {
            TEST_EQ(ok, d, Digit::from_index(i));
            TEST_EQ(ok, d.as_index(), i);
            TEST_EQ(ok, d, Digit::from_string(d.as_string()));
            ++i;
        }

        TEST_EQ(ok, Digit::from_string("Zero"), Digit::nil);
        TEST_EQ(ok, Digit::from_string("One"), Digit::nil);
        TEST_EQ(ok, Digit::from_string("Two"), Digit::nil);
        TEST_EQ(ok, Digit::from_index(0), Digit::nil);
        TEST_EQ(ok, Digit::from_index(1), Digit::nil);
        TEST_EQ(ok, Digit::from_index(2), Digit::nil);
    }

    {
        UNMATCHABLE(Digit, Seven, Eight);

        int i = 0;
        for (auto d : Digit::variants())
        {
            TEST_EQ(ok, d, Digit::from_index(i));
            TEST_EQ(ok, d.as_index(), i);
            TEST_EQ(ok, d, Digit::from_string(d.as_string()));
            if (i == 6)
                i = 9;
            else
                ++i;
        }

        TEST_EQ(ok, Digit::from_string("Seven"), Digit::nil);
        TEST_EQ(ok, Digit::from_string("Eight"), Digit::nil);
        TEST_EQ(ok, Digit::from_index(7), Digit::nil);
        TEST_EQ(ok, Digit::from_index(8), Digit::nil);
    }

    TEST_EQ(ok, Digit::from_string("Seven"), Digit::Seven::grab());
    TEST_EQ(ok, Digit::from_string("Eight"), Digit::Eight::grab());
    TEST_EQ(ok, Digit::from_index(7), Digit::Seven::grab());
    TEST_EQ(ok, Digit::from_index(8), Digit::Eight::grab());

    return ok();
}
