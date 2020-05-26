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
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Stability, Stable, Unstable)
MATCHABLE(
    SpaceComplexity,
    O_pl_1_pr_,
    O_pl_k_pr_,
    O_pl_log_pl_n_pr__pr_,
    O_pl_n_pr_,
    O_pl_n_pls_k_pr_
)
PROPERTYx2_MATCHABLE(
    Stability::Type,
    stability,
    SpaceComplexity::Type,
    space_complexity,
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

    SortingAlgorithm::BubbleSort::grab().set_stability(Stability::Stable::grab());
    SortingAlgorithm::BucketSort::grab().set_stability(Stability::Stable::grab());
    SortingAlgorithm::CountingSort::grab().set_stability(Stability::Stable::grab());
    SortingAlgorithm::CubeSort::grab().set_stability(Stability::Stable::grab());
    SortingAlgorithm::HeapSort::grab().set_stability(Stability::Unstable::grab());
    SortingAlgorithm::InsertionSort::grab().set_stability(Stability::Stable::grab());
    SortingAlgorithm::MergeSort::grab().set_stability(Stability::Stable::grab());
    SortingAlgorithm::RadixSort::grab().set_stability(Stability::Stable::grab());
    SortingAlgorithm::SelectionSort::grab().set_stability(Stability::Unstable::grab());
    SortingAlgorithm::ShellSort::grab().set_stability(Stability::Unstable::grab());
    SortingAlgorithm::TimSort::grab().set_stability(Stability::Stable::grab());
    SortingAlgorithm::QuickSort::grab().set_stability(Stability::Unstable::grab());

    SortingAlgorithm::BubbleSort::grab().set_space_complexity(SpaceComplexity::O_pl_1_pr_::grab());
    SortingAlgorithm::BucketSort::grab().set_space_complexity(SpaceComplexity::O_pl_n_pr_::grab());
    SortingAlgorithm::CountingSort::grab().set_space_complexity(SpaceComplexity::O_pl_k_pr_::grab());
    SortingAlgorithm::CubeSort::grab().set_space_complexity(SpaceComplexity::O_pl_n_pr_::grab());
    SortingAlgorithm::HeapSort::grab().set_space_complexity(SpaceComplexity::O_pl_1_pr_::grab());
    SortingAlgorithm::InsertionSort::grab().set_space_complexity(SpaceComplexity::O_pl_1_pr_::grab());
    SortingAlgorithm::MergeSort::grab().set_space_complexity(SpaceComplexity::O_pl_n_pr_::grab());
    SortingAlgorithm::RadixSort::grab().set_space_complexity(SpaceComplexity::O_pl_n_pls_k_pr_::grab());
    SortingAlgorithm::SelectionSort::grab().set_space_complexity(SpaceComplexity::O_pl_1_pr_::grab());
    SortingAlgorithm::ShellSort::grab().set_space_complexity(SpaceComplexity::O_pl_1_pr_::grab());
    SortingAlgorithm::TimSort::grab().set_space_complexity(SpaceComplexity::O_pl_n_pr_::grab());
    SortingAlgorithm::QuickSort::grab().set_space_complexity(
        SpaceComplexity::O_pl_log_pl_n_pr__pr_::grab()
    );

    std::vector<SortingAlgorithm::Type> stable_algorithms_by_space_complexity{
        SortingAlgorithm::variants().size()
    };
    auto it = std::copy_if(
        SortingAlgorithm::variants().begin(),
        SortingAlgorithm::variants().end(),
        stable_algorithms_by_space_complexity.begin(),
        [](auto sa) { return sa.as_stability() == Stability::Stable::grab(); }
    );
    stable_algorithms_by_space_complexity.resize(
        std::distance(stable_algorithms_by_space_complexity.begin(), it)
    );
    std::stable_sort(
        stable_algorithms_by_space_complexity.begin(),
        stable_algorithms_by_space_complexity.end(),
        [](auto a, auto b) { return a.as_space_complexity() < b.as_space_complexity(); }
    );

    std::cout << "Stable sorting algorithms: " << std::endl;
    for (auto sa : stable_algorithms_by_space_complexity)
        std::cout << "    " << sa << " has space complexity of: " << sa.as_space_complexity() << std::endl;

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
        TEST_FAIL(ok);

    return ok();
}
