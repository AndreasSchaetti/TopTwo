#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "algorithms.h"

namespace top_two
{
    constexpr static auto duration_in_ms = [](auto start, auto end) -> double
    {
        return std::chrono::duration<double, std::micro>(end - start).count();
    };

    using TTimingData = std::vector<double>;

    auto make_data(size_t size)
    {
        std::vector<int32_t> data(size);
        std::iota(data.begin(), data.end(), 0);
        return data;
    }

    template<typename F>
    TTimingData generate_data(size_t size, size_t n_permutations, F algorithm)
    {
        std::mt19937 rng(19937);
        auto data = make_data(size);

        TTimingData timing_data(n_permutations);

        for (auto& timing : timing_data)
        {
            std::shuffle(data.begin(), data.end(), rng);
            
            auto const start = std::chrono::high_resolution_clock::now();
            auto volatile res = algorithm(data);
            auto const end = std::chrono::high_resolution_clock::now();

            timing = duration_in_ms(start, end);
        }

        return timing_data;
    }
}

int32_t main()
{
    std::mt19937 rng(19937);

    const size_t n_permutations = 1'000;
    std::cout << "Using " << n_permutations << " permutations\n";

    auto timing_sort                    = top_two::generate_data(100'000,       n_permutations, top_two::sort);
    auto timing_nth_element             = top_two::generate_data(1'000'000,     n_permutations, top_two::nth_element);
    auto timing_max_element             = top_two::generate_data(10'000'000,    n_permutations, top_two::max_element);
    auto timing_max_element_ben_deane   = top_two::generate_data(10'000'000,    n_permutations, top_two::max_element_ben_deane);
    auto timing_accumulate              = top_two::generate_data(10'000'000,   n_permutations, top_two::accumulate);
    auto timing_transform_reduce        = top_two::generate_data(10'000'000,   n_permutations, top_two::transform_reduce);

    std::ofstream timing_data("results/timing_data.csv");
    timing_data << "sort, nth_element, max_element, max_element_ben_deane, accumulate, transform_reduce\n";
    for (size_t i = 0; i < n_permutations; ++i)
    {
        timing_data
            << timing_sort[i] << ","
            << timing_nth_element[i] << ","
            << timing_max_element[i] << ","
            << timing_max_element_ben_deane[i] << ","
            << timing_accumulate[i] << ","
            << timing_transform_reduce[i] << "\n";
    }

    return 0;
}