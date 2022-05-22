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

        return duration_in_ms(start, end);
    }
}

int32_t main()
{
    const std::vector<size_t> sizes{10, 100, 1'000, 10'000, 20'000, 40'000, 100'000, 200'000, 400'000, 1'000'000};
    const size_t n_permutations = 1'000;

    std::cout << "Using " << n_permutations << " permutations\n";

    std::ofstream results("results.csv");
    results << "size, sort, nth_element, max_element, max_element_ben_deane, accumulate\n";

    for (auto size : sizes)
    {
        auto const dataset = top_two::make_dataset(size, n_permutations);
        std::cout << "Dataset: " << dataset.size() << " x " << dataset.front().size() << " = " << dataset.size() * dataset.front().size() << " elements\n";

        auto const duration_sort = size > 100'000 ? -1 : calculate_duration(dataset, top_two::sort);
        auto const duration_nth_element = calculate_duration(dataset, top_two::nth_element);
        auto const duration_max_element = calculate_duration(dataset, top_two::max_element);
        auto const duration_max_element_ben_deane = calculate_duration(dataset, top_two::max_element_ben_deane);
        auto const duration_accumulate = calculate_duration(dataset, top_two::accumulate);

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
            << duration_accumulate << "\n";
    }
    return 0;
}
