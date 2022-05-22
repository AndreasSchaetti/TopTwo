#include <algorithm>
#include <numeric>
#include <vector>

namespace top_two
{
    struct TResult
    {
        int32_t second_largest;
        int32_t largest;
    };

    TResult sort(std::vector<int32_t> const &values)
    {
        auto vec = std::vector<int32_t>{values};
        std::sort(vec.begin(), vec.end());
        return {vec[vec.size() - 2], vec[vec.size() - 1]};
    }

    TResult nth_element(std::vector<int32_t> const &values)
    {
        auto vec = std::vector<int32_t>{values};
        std::nth_element(vec.begin(), vec.begin() + 1, vec.end(),
                         std::greater<int32_t>());
        return {vec[1], vec[0]};
    }

    TResult max_element(std::vector<int32_t> const &values)
    {
        auto vec = std::vector<int32_t>{values};
        auto const largest_it = std::max_element(vec.cbegin(), vec.cend());

        TResult result;
        result.largest = *largest_it;

        vec.erase(largest_it);

        result.second_largest = *std::max_element(vec.cbegin(), vec.cend());
        return result;
    }

    TResult max_element_ben_deane(std::vector<int32_t> const &values)
    {
        auto vec = std::vector<int32_t>{values};
        auto largest_it = std::max_element(vec.begin(), vec.end());

        TResult result;
        result.largest = *largest_it;

        std::iter_swap(largest_it, std::prev(vec.end()));

        result.second_largest = *std::max_element(vec.cbegin(), std::prev(vec.cend()));
        return result;
    }
/*
    struct TwoLargest
    {
        constexpr static auto two_largest_op =
            [](int32_t value, TResult const &result) -> TResult
        {
            if (value > result.largest)
            {
                return {result.largest, value};
            }
            else if (value > result.second_largest)
            {
                return {value, result.largest};
            }
            else
            {
                return result;
            }
        };

        TResult operator()(int32_t lhs, int32_t rhs) { return {rhs, lhs}; }
        TResult operator()(TResult const &result, int32_t value)
        {
            return two_largest_op(value, result);
        }
        TResult operator()(int32_t value, TResult const &result)
        {
            return two_largest_op(value, result);
        }
        TResult operator()(TResult const &lhs, TResult const &rhs)
        {
            if (lhs.second_largest >= rhs.largest)
            {
                return lhs;
            }
            else if (rhs.second_largest >= lhs.largest)
            {
                return rhs;
            }
            else
            {
                return {std::min(lhs.largest, rhs.largest), std::max(lhs.largest, rhs.largest)};
            }
        }
    };

    TResult reduce(std::vector<int32_t> const &values)
    {
        auto const two_largest_op =
            [](TResult const &result, int32_t value) -> TResult
        {
            if (value > result.largest)
            {
                return {result.largest, value};
            }
            else if (value > result.second_largest)
            {
                return {value, result.largest};
            }
            else
            {
                return result;
            }
        };

        const TResult result{std::numeric_limits<int32_t>::min(),
                             std::numeric_limits<int32_t>::min()};
        return std::reduce(values.cbegin(), values.cend(), result, TwoLargest{});
    }
*/

    TResult accumulate(std::vector<int32_t> const &values)
    {
        auto const two_largest_op =
            [](TResult const &result, int32_t value) -> TResult
        {
            if (value > result.largest)
            {
                return {result.largest, value};
            }
            else if (value > result.second_largest)
            {
                return {value, result.largest};
            }
            else
            {
                return result;
            }
        };

        const TResult result{std::numeric_limits<int32_t>::min(),
                             std::numeric_limits<int32_t>::min()};
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
}