// Copyright 2017 Bartosz Bielecki

#include <functional>
#include <cassert>
#include <memory>


namespace median {

/// Growing array that keeps its elements sorted
/// @remarks For simplicity one cannot modify the contents of the array in any
/// other way than by adding new elements (i.e. no updates and removals).
/// @remarks Sorting is done using a quicksort
/// @remarks Growth factor is hardcoded to 2
/// @remarks Delays allocation to moment when first element is added
/// @tparam T type of elements stored (implements copyable concept)
/// @tparam Orderer functor implementing less than signature of form:
/// bool operator()(T const& left, T const& right)
template<typename T, typename Orderer>
class SortedArray {
public:
    SortedArray() = default;
    ~SortedArray() = default;

    SortedArray(SortedArray const& other) = delete;
    SortedArray& operator=(SortedArray const& other) = delete;

    SortedArray(SortedArray && other) = default;
    SortedArray& operator=(SortedArray&& other) = default;

    void push_back(T element);

    /// @throws std::runtime_error if index is out of bounds
    T at(size_t index) const;

    size_t getSize() const;

private:
    void sort();

    static constexpr int growth_factor = 2;

    std::unique_ptr<T[]> data;
    size_t capacity{0};
    size_t size{0};
};

template<typename T>
using MinSortedArray = SortedArray<T, std::less<T>>;

template<typename T>
using MaxSortedArray = SortedArray<T, std::greater<T>>;

// -------------------------------------------------------------------------- //

template<typename T, typename Orderer>
void median::SortedArray<T, Orderer>::sort()
{
    // Noop
}

template<typename T, typename Orderer>
T median::SortedArray<T, Orderer>::at(size_t index) const
{
    (void)index;
    return T{};
}

template<typename T, typename Orderer>
void median::SortedArray<T, Orderer>::push_back(T element)
{
    (void)element;
}

template<typename T, typename Orderer>
size_t median::SortedArray<T, Orderer>::getSize() const
{
    return this->size;
}

}
