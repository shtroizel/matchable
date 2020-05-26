/*
Copyright (c) 2019-2020, Eric Hyer
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Eric Hyer BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


// demo spread to self and spread of non-matchable types


#include <iostream>

#include "matchable/matchable.h"
#include "matchable/matchable_fwd.h"
#include "test_ok.h"



// forward declare is enough for property of type self
MATCHABLE_FWD(ppl)

PROPERTYx2_MATCHABLE(ppl::Type, friends, int, favorite_number, ppl, Alice, Bob, Charlie)
SET_PROPERTY_VECT(ppl, Alice, friends, ppl::Bob::grab(), ppl::Charlie::grab())
SET_PROPERTY(ppl, Alice, favorite_number, 478)
SET_PROPERTY(ppl, Bob, favorite_number, 275)
SET_PROPERTY(ppl, Charlie, favorite_number, 696)



int main()
{
    test_ok ok;

    int sum{0};

    std::cout << "friends: " << std::endl;
    for (auto f : ppl::Alice::grab().as_friends_vect())
    {
        sum += f.as_favorite_number();
        std::cout << "    " << f.as_favorite_number() << " is Alice's friend " << f
                  << "'s favorite number" << std::endl;
    }

    TEST_EQ(ok, sum, 971);

    return ok();
}
