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

#include "matchable.h"
#include "test_ok.h"


MATCHABLE(
    Number
    , zero
    , one
    , two
    , three
    , four
    , five
    , six
    , seven
    , eight
    , nine
    , ten
    , eleven
    , twelve
    , thirteen
    , fourteen
    , fifteen
    , sixteen
    , seventeen
    , eighteen
    , NineTeen
    , twenty
    , twentyone
    , twentytwo
    , twentythree
    , twentyfour
    , twentyfive
    , twentysix
    , twentyseven
    , twentyeight
    , twentynine
    , thirty
    , thirtyone
    , thirtytwo
    , thirtythree
    , thirtyfour
    , thirtyfive
    , thirtysix
    , thirtyseven
    , thirtyeight
    , thirtynine
    , fourty
    , fourtyone
    , fourtytwo
    , fourtythree
    , fourtyfour
    , fourtyfive
    , fourtysix
    , fourtyseven
    , fourtyeight
    , fourtynine
    , fifty
    , fiftyone
    , fiftytwo
    , fiftythree
    , fiftyfour
    , fiftyfive
    , fiftysix
    , fiftyseven
    , fiftyeight
    , fiftynine
    , sixty
    , sixtyone
    , sixtytwo
    , sixtythree
    , sixtyfour
    , sixtyfive
    , sixtysix
    , sixtyseven
    , sixtyeight
    , sixtynine
    , seventy
    , seventyone
    , seventytwo
    , seventythree
    , seventyfour
    , seventyfive
    , seventysix
    , seventyseven
    , seventyeight
    , seventynine
    , eighty
    , eightyone
    , eightytwo
    , eightythree
    , eightyfour
    , eightyfive
    , eightysix
    , eightyseven
    , eightyeight
    , eightynine
    , ninety
    , ninetyone
    , ninetytwo
    , ninetythree
    , ninetyfour
    , ninetyfive
    , ninetysix
    , ninetyseven
    , ninetyeight
    , ninetynine
    , one_hundred
    , one_hundred_one
    , one_hundred_two
    , one_hundred_three
    , one_hundred_four
    , one_hundred_five
    , one_hundred_six
    , one_hundred_seven
);


int main()
{
    test_ok ok;
    std::cout << "Test traversal of 108 max supported number of variants..." << std::endl;
    Number::Type n;
    for (auto number : Number::variants())
    {
        n = Number::from_string(number.as_string());
        TEST_EQ(ok, n, number);
    }
    return ok();
}
