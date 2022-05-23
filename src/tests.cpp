// Comparison of several solutions for finding the two largest integers in a vector of ints
//
#include <iostream>
#include <vector>

#include "algorithms.h"

std::ostream& operator<<(std::ostream& os, const top_two::TResult& result)
{
    os << "largest: " << result.largest << ", second largest: " << result.second_largest;
    return os;
}

bool check(const top_two::TResult& expected, const top_two::TResult& actual)
{
    const bool result = (expected == actual);
    if (result)
    {
        std::cout << "OK\n";
    }
    else
    {
        std::cout << "FAILED" << " (expected: " << expected << ", actual: " << actual << "\n";
    }
    return result;
}

template <typename F>
void test(const std::string& algorithm_name, F algorithm_callable)
{
    std::vector<int32_t> const trivial{0, 1, 2, 3, 4};
    top_two::TResult const trivial_expected{3, 4};

    std::vector<int32_t> const trivial_inverted{4, 3, 2, 1, 0};
    top_two::TResult const trivial_inverted_expected{3, 4};

    std::vector<int32_t> const random_odd{6, 2, 8, 4, 3, 9, 1, 2, 4};
    top_two::TResult const random_odd_expected{8, 9};

    std::vector<int32_t> const random_even{6, 2, 8, 4, 3, 9, 1, 2, 4, 7};
    top_two::TResult const random_even_expected{8, 9};

    std::vector<int32_t> const all_zero{0, 0, 0, 0, 0};
    top_two::TResult const all_zero_expected{0, 0};
   
    std::vector<int32_t> const all_one{1, 1, 1, 1, 1};
    top_two::TResult const all_one_expected{1, 1};

    std::cout << algorithm_name << "/trivial: "; check(trivial_expected, algorithm_callable(trivial));
    std::cout << algorithm_name << "/trivial_inverted: "; check(trivial_inverted_expected, algorithm_callable(trivial_inverted));
    std::cout << algorithm_name << "/random_odd: "; check(random_odd_expected, algorithm_callable(random_odd));
    std::cout << algorithm_name << "/random_even: "; check(random_even_expected, algorithm_callable(random_even));
    std::cout << algorithm_name << "/all_zero: "; check(all_zero_expected, algorithm_callable(all_zero));
    std::cout << algorithm_name << "/all_one: "; check(all_one_expected, algorithm_callable(all_one));
}

int32_t main()
{
    std::cout << "sequential\n\n";
    {
        test("sort", top_two::sequential::sort);
        test("nth_element", top_two::sequential::nth_element);
        test("max_element", top_two::sequential::max_element);
        test("max_element_ben_deane", top_two::sequential::max_element_ben_deane);
        test("accumulate", top_two::sequential::accumulate);
        test("transform_reduce", top_two::sequential::transform_reduce);
    }  

    std::cout << "\n\nparallel\n\n";
    {
        test("sort", top_two::parallel::sort);
        test("nth_element", top_two::parallel::nth_element);
        test("max_element", top_two::parallel::max_element);
        test("max_element_ben_deane", top_two::parallel::max_element_ben_deane);
        test("reduce", top_two::parallel::reduce);
        test("transform_reduce", top_two::parallel::transform_reduce);
    }      

    return 0;
}
