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

#include "matchable.h"
#include "test_ok.h"



MATCHABLE(Result, Ok, Err)
MATCHABLE(Error, AlreadyDone, CylindricalCubeOutOfRound)

// Create a new type called MergeResult with all of the variants from types Result and Error.
//
//   * No common spreads exist between Result and Error so none are provided.
//   * Note that even if spreads existed specifying them here is optional. The merged type will have
//     whatever is specified. For an example where the spreads are specified see relationships.cpp
MATCHABLES_MERGE_SPREADS(Result, Error, MergeResult)

MATCHABLE(CustomError, InvalidInput)

// The new "MERGE" type created by MATCHABLES_MERGE_SPREADS() walks like a MATCHABLE and quacks
// like a MATCHABLE, so we can merge it with something else to create another merged matchable.
MATCHABLES_MERGE_SPREADS(CustomError, MergeResult, CustomResult)

MATCHABLE(TimeError, Timeout)

// The order in which the "MATCHABLES" (first two args) are specified determines how the indexing
// works when indexed through the new type. Here we do yet another merge to test this even more.
MATCHABLES_MERGE_SPREADS(TimeError, CustomResult, MegaResult)



int main()
{
    test_ok ok;

    MergeResult::Type merged_result{Error::CylindricalCubeOutOfRound::grab()};

    // indexes
    TEST_EQ(ok, Error::CylindricalCubeOutOfRound::grab().as_index(), 1);
    TEST_EQ(ok, merged_result.as_index(), 3);
    TEST_EQ(ok, merged_result, MergeResult::from_index(3));

    // * match used on a MergedMatchable within a loop
    // * MergedMatchable::from_string()
    do
    {
        MATCH_WITH_FLOW_CONTROL merged_result.match({
            {MergeResult::Type{Result::Err::grab()}, [](FlowControl & lc) { lc.brk(); }},
            {MergeResult::Type{Error::CylindricalCubeOutOfRound::grab()}, [&](FlowControl & lc)
                { merged_result = MergeResult::from_string("Err"); lc.cont(); }},
        }); EVAL_FLOW_CONTROL
        TEST_FAIL(ok);
    }
    while (false);
    TEST_EQ(ok, merged_result, MergeResult::Type{Result::Err::grab()});

    // simple match on a MergedMatchable
    merged_result.match({
        {MergeResult::Type{Result::Ok::grab()}, [&](){ TEST_FAIL(ok); }},
        {MergeResult::Type{Error::AlreadyDone::grab()}, [&](){ TEST_FAIL(ok); }},
        {MergeResult::Type{Error::CylindricalCubeOutOfRound::grab()}, [&](){ TEST_FAIL(ok); }},
    });

    {
        MERGED_UNMATCHABLE(MergeResult, AlreadyDone);

        // AlreadyDone is removed from MergeResult
        if (std::find(MergeResult::variants().begin(), MergeResult::variants().end(),
                MergeResult::Type{Error::AlreadyDone::grab()}) != MergeResult::variants().end())
            TEST_FAIL(ok);

        // However, the Error matchable should still have AlreadyDone!
        if (std::find(Error::variants().begin(), Error::variants().end(), Error::AlreadyDone::grab()) ==
                Error::variants().end())
            TEST_FAIL(ok);

        {
            UNMATCHABLE(Result, Err);

            // MergeResult should still have Err!
            if (std::find(MergeResult::variants().begin(), MergeResult::variants().end(),
                    MergeResult::Type{Result::Err::grab()}) == MergeResult::variants().end())
                TEST_FAIL(ok);
        }

        // MERGED_UNMATCHABLE() can be used on all MATCHABLEs, merged or not!
        // For unmerged matchables, MERGED_UNMATCHABLE() and UNMATCHABLE() have the same effect.
        // UNMATCHABLE() performs faster though, and should be used when possible.
        // For completeness, demonstrate use of MERGED_UNMATCHABLE for normal (unmerged) MATCHABLE type.
        MERGED_UNMATCHABLE(Result, Err);

        // Err removed from Result
        if (std::find(Result::variants().begin(), Result::variants().end(), Result::Err::grab()) !=
                Result::variants().end())
            TEST_FAIL(ok);

        // MergeResult should still have Err!
        if (std::find(MergeResult::variants().begin(), MergeResult::variants().end(),
                MergeResult::Type{Result::Err::grab()}) == MergeResult::variants().end())
            TEST_FAIL(ok);
    }

    // One level of implicit conversion works with assignment construction
    CustomResult::Type custom_result = CustomError::InvalidInput::grab();

    // but, two levels does not...
//     CustomResult::Type another_custom_result = Error::CylindricalCubeOutOfRound::grab(); // compile error

    // so do this instead:
    CustomResult::Type another_custom_result{Error::CylindricalCubeOutOfRound::grab()};

    // 3 levels breaks this as well
//     MegaResult::Type mega_result{Result::Ok::grab()}; // compile error

    // we need to help by reducing a level somehow:
    MegaResult::Type mega_result_0{MergeResult::Type{Result::Ok::grab()}};
    MegaResult::Type mega_result_1{CustomResult::Type{Result::Err::grab()}};

    // To summarize:
    //   * up to one level works with assignment construction
    //   * up to two levels work with construction (no =)
    //   * more than two levels need help
    //
    // Also, its just implicit construction that breaks.
    // Existing instances can be assigned without issues:
    custom_result = Error::CylindricalCubeOutOfRound::grab();
    another_custom_result = Result::Ok::grab();
    mega_result_0 = Result::Err::grab();
    mega_result_1 = Result::Ok::grab();

    // non-merged indexes
    TEST_EQ(ok, Result::Ok::grab().as_index(), 0);
    TEST_EQ(ok, Result::Err::grab().as_index(), 1);
    TEST_EQ(ok, Error::AlreadyDone::grab().as_index(), 0);
    TEST_EQ(ok, Error::CylindricalCubeOutOfRound::grab().as_index(), 1);
    TEST_EQ(ok, CustomError::InvalidInput::grab().as_index(), 0);

    // MergeResult indexes
    TEST_EQ(ok, MergeResult::Type{Result::Ok::grab()}.as_index(), 0);
    TEST_EQ(ok, MergeResult::Type{Result::Err::grab()}.as_index(), 1);
    TEST_EQ(ok, MergeResult::Type{Error::AlreadyDone::grab()}.as_index(), 2);
    TEST_EQ(ok, MergeResult::Type{Error::CylindricalCubeOutOfRound::grab()}.as_index(), 3);

    // CustomResult indexes
    TEST_EQ(ok, CustomResult::Type{CustomError::InvalidInput::grab()}.as_index(), 0);
    TEST_EQ(ok, CustomResult::Type{Result::Ok::grab()}.as_index(), 1);
    TEST_EQ(ok, CustomResult::Type{Result::Err::grab()}.as_index(), 2);
    TEST_EQ(ok, CustomResult::Type{Error::AlreadyDone::grab()}.as_index(), 3);
    TEST_EQ(ok, CustomResult::Type{Error::CylindricalCubeOutOfRound::grab()}.as_index(), 4);

    // MegaResult indexes
    TEST_EQ(ok, MegaResult::Type{TimeError::Timeout::grab()}.as_index(), 0);
    TEST_EQ(ok, MegaResult::Type{CustomError::InvalidInput::grab()}.as_index(), 1);
    TEST_EQ(ok, MegaResult::Type{MergeResult::Type{Result::Ok::grab()}}.as_index(), 2);
    TEST_EQ(ok, MegaResult::Type{CustomResult::Type{Result::Err::grab()}}.as_index(), 3); // switch up help
    TEST_EQ(ok, MegaResult::Type{MergeResult::Type{Error::AlreadyDone::grab()}}.as_index(), 4);
    TEST_EQ(ok, MegaResult::Type{MergeResult::Type{Error::CylindricalCubeOutOfRound::grab()}}.as_index(),5);

    // test MegaResult indexes without needing "help" (chaining)
    TEST_EQ(ok, (mega_result_0 = TimeError::Timeout::grab()).as_index(), 0);
    TEST_EQ(ok, (mega_result_0 = CustomError::InvalidInput::grab()).as_index(), 1);
    TEST_EQ(ok, (mega_result_0 = Result::Ok::grab()).as_index(), 2);
    TEST_EQ(ok, (mega_result_0 = Result::Err::grab()).as_index(), 3);
    TEST_EQ(ok, (mega_result_0 = Error::AlreadyDone::grab()).as_index(), 4);
    TEST_EQ(ok, (mega_result_0 = Error::CylindricalCubeOutOfRound::grab()).as_index(), 5);

    // test MegaResult::from_index()
    for (int i = 0; i < (int) MegaResult::variants().size(); ++i)
    {
        mega_result_0 = MegaResult::from_index(i);
        TEST_EQ(ok, mega_result_0.as_index(), i);
        std::cout << mega_result_0 << " has index: " << mega_result_0.as_index() << std::endl;
    }

    return ok();
}
