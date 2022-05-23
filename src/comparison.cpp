// Comparison of several solutions for finding the two largest integers in a vector of ints
//
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
        return std::chrono::duration<double, std::milli>(end - start).count();
    };

    using TDataset = std::vector<std::vector<int32_t>>;

    auto make_dataset(size_t size, size_t n_permutations)
    {
        std::vector<int32_t> data(size);
        std::iota(data.begin(), data.end(), 0);
        TDataset dataset;
        dataset.reserve(n_permutations);

        std::mt19937 rng(19937);

        for (size_t i = 0; i < n_permutations; ++i)
        {
            std::shuffle(data.begin(), data.end(), rng);
            dataset.push_back(data);
        }

        return dataset;
    }

    template <typename F>
    double calculate_duration(TDataset dataset, F algorithm)
    {
        auto const start = std::chrono::high_resolution_clock::now();
        for (const auto &data : dataset)
        {
            auto volatile result = algorithm(data);
        }
        auto const end = std::chrono::high_resolution_clock::now();

        return duration_in_ms(start, end) / dataset.size();
    }
}

int32_t main()
{
    const std::vector<size_t> sizes{
         10
        , 100
        , 1'000 
        , 3'162 
        , 10'000 
        , 31'623 
        , 100'000 
        , 177'828 
        , 316'228
        , 562'341
        , 1'000'000
        //, 1'397'940 
        //, 1'698'970 
        //, 1'875'061 
        //, 10'000'000
    };
    const size_t n_permutations = 1'000;

    std::cout << "Using " << n_permutations << " permutations\n";

    std::ofstream results("results/comparison_of_algorithms_sequential.csv");
    results << "size, sort, nth_element, max_element, max_element_ben_deane, accumulate, transform_reduce\n";

    for (auto size : sizes)
    {
        auto const dataset = top_two::make_dataset(size, n_permutations);
        std::cout << "Dataset: " << dataset.size() << " x " << dataset.front().size() << " = " << dataset.size() * dataset.front().size() << " elements\n";

        auto const duration_sort = size > 100'000 ? -1 : calculate_duration(dataset, top_two::sequential::sort);
        auto const duration_nth_element = calculate_duration(dataset, top_two::sequential::nth_element);
        auto const duration_max_element = calculate_duration(dataset, top_two::sequential::max_element);
        auto const duration_max_element_ben_deane = calculate_duration(dataset, top_two::sequential::max_element_ben_deane);
        auto const duration_accumulate = calculate_duration(dataset, top_two::sequential::accumulate);
        auto const duration_transform_reduce = calculate_duration(dataset, top_two::sequential::transform_reduce);

        results
            << size << ",";
        if (duration_sort == -1)
        {
            results << "NaN,";
        }
        else
        {
            results << duration_sort << ",";
        }
        results
            << duration_nth_element << ","
            << duration_max_element << ","
            << duration_max_element_ben_deane << ","
            << duration_accumulate << ","
            << duration_transform_reduce << "\n";
    }
    return 0;
}
