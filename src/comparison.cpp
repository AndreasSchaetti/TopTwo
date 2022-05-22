// Comparison of several solutions for finding the two largest integers in a vector of ints
//
#include <algorithm>
#include <chrono>
//#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

struct TResult
{
    int32_t second_largest;
    int32_t largest;
};

auto make_dataset(size_t size, size_t n_permutations) {
    std::vector<int32_t> data(size);
    std::iota(data.begin(), data.end(), 0);
    std::vector<std::vector<int32_t>> dataset;

    std::random_device rd;
    std::mt19937 g(rd());

    for (size_t i; i < n_permutations; ++i)
    {
        std::shuffle(data.begin(), data.end(), g);
        dataset.push_back(data);
    }

    return dataset;
}

TResult two_largest_sort(std::vector<int32_t> const& values) {
    auto vec = std::vector<int32_t>{values};
    std::sort(vec.begin(), vec.end());
    return {vec[vec.size() - 2], vec[vec.size() - 1]};
}

TResult two_largest_nth(std::vector<int32_t> const& values) {
    auto vec = std::vector<int32_t>{values};
    std::nth_element(vec.begin(), vec.begin() + 1, vec.end(),
                     std::greater<int32_t>());
    return {vec[1], vec[0]};
}

TResult two_largest_max_element(std::vector<int32_t> const& values) {
    auto vec = std::vector<int32_t>{values};
    auto const largest_it = std::max_element(vec.cbegin(), vec.cend());

    TResult result;
    result.largest = *largest_it;

    vec.erase(largest_it);
    
    result.second_largest = *std::max_element(vec.cbegin(), vec.cend());
    return result;
}

TResult two_largest_max_element_clever(std::vector<int32_t> const& values) {
    auto vec = std::vector<int32_t>{values};
    auto largest_it = std::max_element(vec.begin(), vec.end());
   
    TResult result;
    result.largest = *largest_it;

    std::iter_swap(largest_it, std::prev(vec.end()));
    
    result.second_largest = *std::max_element(vec.cbegin(), std::prev(vec.cend()));
    return result;
}

struct TwoLargest {
    constexpr static auto two_largest_op =
        [](int32_t value, TResult const& result) -> TResult {
        if (value > result.largest) {
            return {result.largest, value};
        } else if (value > result.second_largest) {
            return {value, result.largest};
        } else {
            return result;
        }
    };

    TResult operator()(int32_t lhs, int32_t rhs) { return {rhs, lhs}; }
    TResult operator()(TResult const& result, int32_t value) {
        return two_largest_op(value, result);
    }
    TResult operator()(int32_t value, TResult const& result) {
        return two_largest_op(value, result);
    }
    TResult operator()(TResult const& lhs, TResult const& rhs) {
        if (lhs.second_largest >= rhs.largest)
        {
            return lhs;
        } else if (rhs.second_largest >= lhs.largest) {
            return rhs;
        }
        else {
            return {std::min(lhs.largest, rhs.largest), std::max(lhs.largest, rhs.largest)};
        }
    }
};

TResult two_largest_reduce(std::vector<int32_t> const& values) {
    auto const two_largest_op =
        [](TResult const& result, int32_t value) -> TResult {
        if (value > result.largest) {
            return {result.largest, value};
        } else if (value > result.second_largest) {
            return {value, result.largest};
        } else {
            return result;
        }
    };

    const TResult result{std::numeric_limits<int32_t>::min(),
                    std::numeric_limits<int32_t>::min()};
    //return std::reduce(values.cbegin(), values.cend(), result, TwoLargest{});
    return std::accumulate(values.cbegin(), values.cend(), result, two_largest_op);
}
/*
TResult two_largest_transform_reduce(std::vector<int32_t> const& values) {
    const TResult result{std::numeric_limits<int32_t>::min(),
                    std::numeric_limits<int32_t>::min()};

    auto const transform_op = [](int32_t val) -> TResult {
        return {std::numeric_limits<int32_t>::min(), val};
    };
    auto const reduce_op =
        [](TResult const& lhs,
           TResult const& rhs) -> TResult {
        auto arr = std::array<int32_t, 4>{std::get<0>(lhs), std::get<1>(lhs),
                                      std::get<0>(rhs), std::get<1>(rhs)};
        std::sort(arr.begin(), arr.end());
        return {arr[2], arr[3]};
    };
    return std::transform_reduce(values.cbegin(), values.cend(), init, reduce_op,
                                 transform_op);
}
*/
int32_t main() {
    const std::vector<size_t> sizes{10, 100, 1'000, 10'000, 100'000, 1'000'000};
    const size_t n_permutations = 1'000;

    for (auto size : sizes)
    {
        auto const dataset = make_dataset(size, n_permutations);

        auto const start_sort = std::chrono::high_resolution_clock::now();
        for (const auto & data : dataset)
        {
            auto volatile res = two_largest_sort(data);
        }
        auto const end_sort = std::chrono::high_resolution_clock::now();

        auto const start_nth_element = std::chrono::high_resolution_clock::now();
        for (const auto & data : dataset)
        {
            auto volatile res = two_largest_nth(data);
        }
        auto const end_nth_element = std::chrono::high_resolution_clock::now();

        auto const start_max_element = std::chrono::high_resolution_clock::now();
        for (const auto & data : dataset)   
        {
            auto const res = two_largest_max_element(data);
        }
        auto const end_max_element = std::chrono::high_resolution_clock::now();

        auto const start_max_element_clever = std::chrono::high_resolution_clock::now();
        for (const auto & data : dataset)   
        {
            auto const res = two_largest_max_element_clever(data);
        }
        auto const end_max_element_clever = std::chrono::high_resolution_clock::now();

        auto const start_reduce = std::chrono::high_resolution_clock::now();
        for (const auto & data : dataset)
        {
            auto const res = two_largest_reduce(data);
        }
        auto const end_reduce = std::chrono::high_resolution_clock::now();
        
        /*
        auto const start_nth_element = std::chrono::high_resolution_clock::now();
        for (const auto & data : dataset)
        {
            auto const res = two_largest_transform_reduce(data);
        }
        auto const end_sort = std::chrono::high_resolution_clock::now();
        */

        auto const print_duration = [] (auto start, auto end)
        {
            return std::chrono::duration<double>(end - start).count();
        };

        std::cout
            << n_permutations << " permutations of size " << size << ": "
            << "sort = " << print_duration(start_sort, end_sort) << " s, "
            << "nth_element = " << print_duration(start_nth_element, end_nth_element) << " s, "
            << "max_element = " << print_duration(start_max_element, end_max_element) << " s, "
            << "max_element_clever = " << print_duration(start_max_element_clever, end_max_element_clever) << " s, "
            << "reduce = " << print_duration(start_reduce, end_reduce) << " s\n";
        /*
        std::format("{} permutations of size {}: sort = {} s, nth_element = {} s, max_element = {} s, max_element_clever = {} s, reduce = {} s\n",
            n_permutations,
            size,
            end_sort - start_sort,
            end_nth_element - start_nth_element,
            end_max_element - start_max_element,
            end_max_element_clever - start_max_element_clever,
            end_reduce - start_reduce);
            */
    }
    return 0;
}