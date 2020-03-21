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

// Create a new type called MergedResult with all of the variants from types Result and Error.
//
//   * No common spreads exist between Result and Error so none are provided.
//   * Note that even if spreads existed specifying them here is optional. The merged type will have
//     whatever is specified. For an example where the spreads are specified see relationships.cpp
MATCHABLES_MERGE_SPREADS(Result, Error, MergedResult)



int main()
{
    test_ok ok;

    // Assign Matchable to MergedMatchable
    MergedResult::Type merged_result = Error::CylindricalCubeOutOfRound::grab();

    // indexes
    TEST_EQ(ok, Error::CylindricalCubeOutOfRound::grab().as_index(), 1);
    TEST_EQ(ok, merged_result.as_index(), 3);
    TEST_EQ(ok, merged_result, MergedResult::from_index(3));

    // * match used on a MergedMatchable within a loop
    // * MergedMatchable::from_string()
    do
    {
        MATCH_WITH_FLOW_CONTROL merged_result.match({
            {MergedResult::Type{Result::Err::grab()}, [](FlowControl & lc) { lc.brk(); }},
            {MergedResult::Type{Error::CylindricalCubeOutOfRound::grab()}, [&](FlowControl & lc)
                { merged_result = MergedResult::from_string("Err"); lc.cont(); }},
        }); EVAL_FLOW_CONTROL
        TEST_FAIL(ok);
    }
    while (false);
    TEST_EQ(ok, merged_result, MergedResult::Type{Result::Err::grab()});

    // simple match on a MergedMatchable
    merged_result.match({
        {MergedResult::Type{Result::Ok::grab()}, [&](){ TEST_FAIL(ok); }},
        {MergedResult::Type{Error::AlreadyDone::grab()}, [&](){ TEST_FAIL(ok); }},
        {MergedResult::Type{Error::CylindricalCubeOutOfRound::grab()}, [&](){ TEST_FAIL(ok); }},
    });

    {
        MERGED_UNMATCHABLE(MergedResult, AlreadyDone);

        // AlreadyDone is removed from MergedResult
        if (std::find(MergedResult::variants().begin(), MergedResult::variants().end(),
                MergedResult::Type{Error::AlreadyDone::grab()}) != MergedResult::variants().end())
            TEST_FAIL(ok);

        // However, the Error matchable should still have AlreadyDone!
        if (std::find(Error::variants().begin(), Error::variants().end(), Error::AlreadyDone::grab()) ==
                Error::variants().end())
            TEST_FAIL(ok);

        {
            UNMATCHABLE(Result, Err);

            // MergedResult should still have Err!
            if (std::find(MergedResult::variants().begin(), MergedResult::variants().end(),
                    MergedResult::Type{Result::Err::grab()}) == MergedResult::variants().end())
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

        // MergedResult should still have Err!
        if (std::find(MergedResult::variants().begin(), MergedResult::variants().end(),
                MergedResult::Type{Result::Err::grab()}) == MergedResult::variants().end())
            TEST_FAIL(ok);
    }

    return ok();
}
