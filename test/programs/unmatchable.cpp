/*
Copyright (c) 2019-2020, Eric Hyer
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#include <iostream>
#include <vector>

#include "matchable_fwd.h"
#include "matchable.h"
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



int main()
{
    test_ok ok;

    // Using Unmatchable directly, no macros, no namespaces
    TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 2);
    {
        Unmatchable<NoNameSpace::Type> unmatchable{
            {NoNameSpace::zero::grab()},
        };
        TEST_EQ(ok, NoNameSpace::variants().size(), (size_t) 1);
        TEST_EQ(ok, NoNameSpace::variants()[0], NoNameSpace::one::grab());
        {
            Unmatchable<NoNameSpace::Type> unmatchable{
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
        Unmatchable<
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
            Unmatchable<
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

    return ok();
}
