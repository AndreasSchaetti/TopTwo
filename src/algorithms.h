#include <algorithm>
#include <execution>
#include <numeric>
#include <vector>

#include <iostream>

namespace top_two
{
    struct TResult
    {
        int32_t second_largest = std::numeric_limits<int32_t>::min();
        int32_t largest = std::numeric_limits<int32_t>::min();

        bool operator==(const TResult &other) const
        {
            return (second_largest == other.second_largest && largest == other.largest);
        }

        TResult() = default;
        TResult(int32_t second_largest_, int32_t largest_) : second_largest(second_largest_), largest(largest_) {}
        TResult(int32_t val) : second_largest(val), largest(val) { std::cout << "TResult(int32_t val) called with " << val << "\n"; }
    };

    namespace sequential
    {
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

        TResult accumulate(std::vector<int32_t> const &values)
        {
            auto const accumulate_op =
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

            return std::accumulate(values.cbegin(), values.cend(), TResult{}, accumulate_op);
        }

        TResult transform_reduce(std::vector<int32_t> const &values)
        {
            auto const transform_op = [](int32_t value) -> TResult
            {
                return {std::numeric_limits<int32_t>::min(), value};
            };

            auto const reduce_op =
                [](TResult const &lhs,
                   TResult const &rhs) -> TResult
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
            };

            return std::transform_reduce(values.cbegin(), values.cend(), TResult{}, reduce_op,
                                         transform_op);
        }
    }

    namespace parallel
    {
        TResult sort(std::vector<int32_t> const &values)
        {
            auto vec = std::vector<int32_t>{values};
            std::sort(std::execution::par_unseq, vec.begin(), vec.end());
            return {vec[vec.size() - 2], vec[vec.size() - 1]};
        }

        TResult nth_element(std::vector<int32_t> const &values)
        {
            auto vec = std::vector<int32_t>{values};
            std::nth_element(std::execution::par_unseq, vec.begin(), vec.begin() + 1, vec.end(),
                             std::greater<int32_t>());
            return {vec[1], vec[0]};
        }

        TResult max_element(std::vector<int32_t> const &values)
        {
            auto vec = std::vector<int32_t>{values};
            auto const largest_it = std::max_element(std::execution::par_unseq, vec.cbegin(), vec.cend());

            TResult result;
            result.largest = *largest_it;

            vec.erase(largest_it);

            result.second_largest = *std::max_element(std::execution::par_unseq, vec.cbegin(), vec.cend());
            return result;
        }

        TResult max_element_ben_deane(std::vector<int32_t> const &values)
        {
            auto vec = std::vector<int32_t>{values};
            auto largest_it = std::max_element(std::execution::par_unseq, vec.begin(), vec.end());

            TResult result;
            result.largest = *largest_it;

            std::iter_swap(largest_it, std::prev(vec.end()));

            result.second_largest = *std::max_element(std::execution::par_unseq, vec.cbegin(), std::prev(vec.cend()));
            return result;
        }

        struct ReduceOp
        {
            constexpr static auto reduce_op =
                [](int32_t value, TResult const &result) -> TResult
            {
                if (value >= result.largest)
                {
                    // case 'value == result.largest' saves a comparison. it doesn't have to be checked against 'result.second_largest'. 
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

            TResult operator()(int32_t lhs, int32_t rhs) {
                 return {std::min(lhs, rhs), std::max(lhs, rhs)}; }
            TResult operator()(TResult const &result, int32_t value)
            {
                return reduce_op(value, result);
            }
            TResult operator()(int32_t value, TResult const &result)
            {
                return reduce_op(value, result);
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
            return std::reduce(std::execution::par_unseq, values.cbegin(), values.cend(), TResult{}, ReduceOp{});
        }

        TResult transform_reduce(std::vector<int32_t> const &values)
        {
            auto const transform_op = [](int32_t value) -> TResult
            {
                return {std::numeric_limits<int32_t>::min(), value};
            };

            auto const reduce_op =
                [](TResult const &lhs,
                   TResult const &rhs) -> TResult
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
            };

            return std::transform_reduce(std::execution::par_unseq, values.cbegin(), values.cend(), TResult{}, reduce_op,
                                         transform_op);
        }
    }
}