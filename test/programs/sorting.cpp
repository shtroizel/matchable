#include <algorithm>
#include <iostream>
#include <vector>

#include "matchable/matchable.h"
#include "test_ok.h"



MATCHABLE(Stability, Stable, Unstable)
MATCHABLE(
    SpaceComplexity,
    O_parl_1_parr_,
    O_parl_k_parr_,
    O_parl_log_parl_n_parr__parr_,
    O_parl_n_parr_,
    O_parl_n_plus_k_parr_
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

    SortingAlgorithm::BubbleSort::grab().set_space_complexity(SpaceComplexity::O_parl_1_parr_::grab());
    SortingAlgorithm::BucketSort::grab().set_space_complexity(SpaceComplexity::O_parl_n_parr_::grab());
    SortingAlgorithm::CountingSort::grab().set_space_complexity(SpaceComplexity::O_parl_k_parr_::grab());
    SortingAlgorithm::CubeSort::grab().set_space_complexity(SpaceComplexity::O_parl_n_parr_::grab());
    SortingAlgorithm::HeapSort::grab().set_space_complexity(SpaceComplexity::O_parl_1_parr_::grab());
    SortingAlgorithm::InsertionSort::grab().set_space_complexity(SpaceComplexity::O_parl_1_parr_::grab());
    SortingAlgorithm::MergeSort::grab().set_space_complexity(SpaceComplexity::O_parl_n_parr_::grab());
    SortingAlgorithm::RadixSort::grab().set_space_complexity(SpaceComplexity::O_parl_n_plus_k_parr_::grab());
    SortingAlgorithm::SelectionSort::grab().set_space_complexity(SpaceComplexity::O_parl_1_parr_::grab());
    SortingAlgorithm::ShellSort::grab().set_space_complexity(SpaceComplexity::O_parl_1_parr_::grab());
    SortingAlgorithm::TimSort::grab().set_space_complexity(SpaceComplexity::O_parl_n_parr_::grab());
    SortingAlgorithm::QuickSort::grab().set_space_complexity(
        SpaceComplexity::O_parl_log_parl_n_parr__parr_::grab()
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
