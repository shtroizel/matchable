/*
Copyright (c) 2019-2022, shtroizel
All rights reserved without prejudice.

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



#include <random>
#include <iostream>

#include "matchable/matchable.h"



MATCHABLE(status, success, invalid_input, nothing_to_do, insufficient_memory, timed_out);


status::Type foo()
{
    static int magic;
    magic++;
    if (magic % 107 == 0) // get variant by grabbing (constant complexity)
        return status::success::grab();

    if (magic % 42 == 0) // get variant from string (log2(n) complexity)
        return status::from_string("invalid_input");

    static std::mt19937 engine{std::random_device()()};

    // get index from variant
    // get std::vector of all variants
    static std::uniform_int_distribution<int> status_distribution{
        status::nothing_to_do::grab().as_index(),
        static_cast<int>(status::variants().size()) - 1
    };

    // get variant from index (constant complexity)
    status::Type ret{status::from_index(status_distribution(engine))};
    assert(!ret.is_nil()); // index out of range would mean bug creating status_distribution

    return ret;
}


int main()
{
    status::Type foo_status;
    assert(foo_status.is_nil());

    while (true)
    {
        std::cout << "." << std::flush;

        foo_status = foo();
        MATCH_WITH_FLOW_CONTROL foo_status.match({
            {status::nil,
                [](auto &){
                    std::cout << status::nil.as_string() << "!" << std::endl;
                }},
            {status::success::grab(),
                [](matchable::FlowControl & fc){ // explicit type instead of auto for FlowControl
                    std::cout << "107" << std::endl;
                    fc.brk();
                }},
            {status::nothing_to_do::grab(),       [](auto & fc){ fc.cont(); }},
            {status::invalid_input::grab(),       [](auto & fc){ fc.cont(); }},
            {status::insufficient_memory::grab(), [](auto & fc){ fc.cont(); }},
            {status::timed_out::grab(),           [](auto & fc){ fc.cont(); }},
        }); EVAL_FLOW_CONTROL

        // only happens if foo_status == status::nil, which is never
        // operator<<() is provided, so as_string() can be omitted...
        std::cout << "match: " << status::nil << "!" << std::endl;
    }

    std::cout << foo_status << std::endl;

    return foo_status.as_index();
}
