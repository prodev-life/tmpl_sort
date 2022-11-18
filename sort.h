#include <type_traits>

template <int... I>
struct IntArray
{
};

namespace detail
{

template <typename... Arrays>
struct Join
{
};

template <>
struct Join<>
{
    using type = IntArray<>;
};

template <int... I>
struct Join<IntArray<I...>>
{
    using type = IntArray<I...>;
};

template <int... I1, int... I2>
struct Join<IntArray<I1...>, IntArray<I2...>>
{
    using type = IntArray<I1..., I2...>;
};

template <int... I1, int... I2, typename... Rest>
struct Join<IntArray<I1...>, IntArray<I2...>, Rest...>
{
    using type = typename Join<IntArray<I1..., I2...>, Rest...>::type;
};

template <typename... Arrays>
using join_t = typename Join<Arrays...>::type;

template <bool Accept, int Element>
struct FilterElement
{
};

template <int Element>
struct FilterElement<true, Element>
{
    using type = IntArray<Element>;
};

template <int Element>
struct FilterElement<false, Element>
{
    using type = IntArray<>;
};

template <bool Accept, int Element>
using filter_element_t = typename FilterElement<Accept, Element>::type;

template <template <int> class Pred, typename Array>
struct Filter
{
};

template <template <int> class Pred>
struct Filter<Pred, IntArray<>>
{
    using type = IntArray<>;
};

template <template <int> class Pred, int First, int... Rest>
struct Filter<Pred, IntArray<First, Rest...>>
{
    using type = join_t<
        filter_element_t<Pred<First>::value, First>,
        typename Filter<Pred, IntArray<Rest...>>::type
    >;
};

template <template <int> class Pred, typename Array>
using filter_t = typename Filter<Pred, Array>::type;

template <int X>
struct LessThan
{
    template <int Y>
    struct type
    {
        static constexpr bool value = Y < X;
    };
};

template <int X>
struct Equal
{
    template <int Y>
    struct type
    {
        static constexpr bool value = Y == X;
    };
};

template <int X>
struct GreaterThan
{
    template <int Y>
    struct type
    {
        static constexpr bool value = Y > X;
    };
};

template <typename T>
struct Sort
{
};

template <>
struct Sort<IntArray<>>
{
    using type = IntArray<>;
};

template <int Head, int... Rest>
struct Sort<IntArray<Head, Rest...>>
{
private:
    using less_t = filter_t<LessThan<Head>::template type, IntArray<Head, Rest...>>;
    using equal_t = filter_t<Equal<Head>::template type, IntArray<Head, Rest...>>;
    using greater_t = filter_t<GreaterThan<Head>::template type, IntArray<Head, Rest...>>;

public:
    using type =
        join_t<typename Sort<less_t>::type, equal_t, typename Sort<greater_t>::type>;
};
} // namespace detail

template <typename Array>
using sort_t = typename detail::Sort<Array>::type;

static_assert(std::is_same_v<sort_t<IntArray<3, 1, 2>>, IntArray<1, 2, 3>>);
static_assert(std::is_same_v<sort_t<IntArray<3, 2, 1>>, IntArray<1, 2, 3>>);
static_assert(std::is_same_v<sort_t<IntArray<3, 1, 1>>, IntArray<1, 1, 3>>);
static_assert(std::is_same_v<sort_t<IntArray<1, 1, 1>>, IntArray<1, 1, 1>>);
static_assert(std::is_same_v<sort_t<IntArray<2, 1>>, IntArray<1, 2>>);
static_assert(std::is_same_v<sort_t<IntArray<1, 1>>, IntArray<1, 1>>);
static_assert(std::is_same_v<sort_t<IntArray<1>>, IntArray<1>>);
static_assert(std::is_same_v<sort_t<IntArray<>>, IntArray<>>);