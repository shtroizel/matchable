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



#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "matchable.h"
#include "test_ok.h"



MATCHABLE(Stability, Stable, Unstable)
MATCHABLE(
    SpaceComplexity,
    O_1,
    O_k,
    O_n,
    O_n_plus_k,
    O_nk,
    O_log_n,
    O_n_log_n,
    O_n_exp_2,
    O_2_exp_n,
    O_n_factorial
)
SPREADx2_MATCHABLE(
    Stability,
    SpaceComplexity,
    SortingAlgorithm,
    BubbleSort,
    BucketSort,
    CountingSort,
    CubeSort,
    HeapSort,
    InsertionSort,
    MergeSort,
    RadixSort,
    SelectionSort,
    ShellSort,
    TimSort,
    QuickSort
)



int main()
{
    test_ok ok;

    SortingAlgorithm::BubbleSort::grab().set_Stability(Stability::Stable::grab());
    SortingAlgorithm::BucketSort::grab().set_Stability(Stability::Stable::grab());
    SortingAlgorithm::CountingSort::grab().set_Stability(Stability::Stable::grab());
    SortingAlgorithm::CubeSort::grab().set_Stability(Stability::Stable::grab());
    SortingAlgorithm::HeapSort::grab().set_Stability(Stability::Unstable::grab());
    SortingAlgorithm::InsertionSort::grab().set_Stability(Stability::Stable::grab());
    SortingAlgorithm::MergeSort::grab().set_Stability(Stability::Stable::grab());
    SortingAlgorithm::RadixSort::grab().set_Stability(Stability::Stable::grab());
    SortingAlgorithm::SelectionSort::grab().set_Stability(Stability::Unstable::grab());
    SortingAlgorithm::ShellSort::grab().set_Stability(Stability::Unstable::grab());
    SortingAlgorithm::TimSort::grab().set_Stability(Stability::Stable::grab());
    SortingAlgorithm::QuickSort::grab().set_Stability(Stability::Unstable::grab());

    SortingAlgorithm::BubbleSort::grab().set_SpaceComplexity(SpaceComplexity::O_1::grab());
    SortingAlgorithm::BucketSort::grab().set_SpaceComplexity(SpaceComplexity::O_n::grab());
    SortingAlgorithm::CountingSort::grab().set_SpaceComplexity(SpaceComplexity::O_k::grab());
    SortingAlgorithm::CubeSort::grab().set_SpaceComplexity(SpaceComplexity::O_n::grab());
    SortingAlgorithm::HeapSort::grab().set_SpaceComplexity(SpaceComplexity::O_1::grab());
    SortingAlgorithm::InsertionSort::grab().set_SpaceComplexity(SpaceComplexity::O_1::grab());
    SortingAlgorithm::MergeSort::grab().set_SpaceComplexity(SpaceComplexity::O_n::grab());
    SortingAlgorithm::RadixSort::grab().set_SpaceComplexity(SpaceComplexity::O_n_plus_k::grab());
    SortingAlgorithm::SelectionSort::grab().set_SpaceComplexity(SpaceComplexity::O_1::grab());
    SortingAlgorithm::ShellSort::grab().set_SpaceComplexity(SpaceComplexity::O_1::grab());
    SortingAlgorithm::TimSort::grab().set_SpaceComplexity(SpaceComplexity::O_n::grab());
    SortingAlgorithm::QuickSort::grab().set_SpaceComplexity(SpaceComplexity::O_log_n::grab());

    std::vector<SortingAlgorithm::Type> stable_algorithms_by_space_complexity{
        SortingAlgorithm::variants().size()
    };
    auto it = std::copy_if(
        SortingAlgorithm::variants().begin(),
        SortingAlgorithm::variants().end(),
        stable_algorithms_by_space_complexity.begin(),
        [](auto sa) { return sa.as_Stability() == Stability::Stable::grab(); }
    );
    stable_algorithms_by_space_complexity.resize(
        std::distance(stable_algorithms_by_space_complexity.begin(), it)
    );
    std::sort(
        stable_algorithms_by_space_complexity.begin(),
        stable_algorithms_by_space_complexity.end(),
        [](auto a, auto b) { return a.as_SpaceComplexity().lt_enum_order(b.as_SpaceComplexity()); }
    );

    std::cout << "Stable sorting algorithms: " << std::endl;
    for (auto sa : stable_algorithms_by_space_complexity)
        std::cout << "    " << sa << " has space complexity of: " << sa.as_SpaceComplexity() << std::endl;

    std::vector<SortingAlgorithm::Type> truth{
        SortingAlgorithm::BubbleSort::grab(),
        SortingAlgorithm::InsertionSort::grab(),
        SortingAlgorithm::CountingSort::grab(),
        SortingAlgorithm::BucketSort::grab(),
        SortingAlgorithm::CubeSort::grab(),
        SortingAlgorithm::MergeSort::grab(),
        SortingAlgorithm::TimSort::grab(),
        SortingAlgorithm::RadixSort::grab()
    };
    if (stable_algorithms_by_space_complexity != truth)
        FAIL(ok);

    return ok();
}
