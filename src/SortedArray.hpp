// Copyright 2017 Bartosz Bielecki

#pragma once

#include <cassert>
#include <stdexcept>
#include <functional>
#include <memory>
#include <utility>

// For std::copy
#include <algorithm>


namespace median {

/// Growing array that keeps its elements sorted
/// @remarks For simplicity one cannot modify the contents of the array in any
/// other way than by adding new elements (i.e. no updates and removals).
/// @remarks Sorting is done using a quicksort
/// @remarks Smaller elements are placed first
/// @remarks Growth factor is hardcoded to 2
/// @remarks Delays allocation to moment when first element is added
/// @tparam T type of elements stored (implements copyable and weak comparable
/// concept)
template<typename T>
class SortedArray {
public:
    SortedArray() = default;
    ~SortedArray() = default;

    SortedArray(SortedArray const& other) = delete;
    SortedArray& operator=(SortedArray const& other) = delete;

    SortedArray(SortedArray&& other) = default;
    SortedArray& operator=(SortedArray&& other) = default;

    void push_back(T element);

    /// @throws std::runtime_error if index is out of bounds
    T at(size_t index) const;

    size_t getSize() const;

private:
    void qsort(size_t left, size_t right);

    size_t freeSpace();
    void sort();
    size_t getNewCapacity() const;
    void grow();

    static constexpr int growth_factor = 2;

    std::unique_ptr<T[]> data;
    size_t capacity{0};
    size_t size{0};
};

}

// -------------------------------------------------------------------------- //

template<typename T>
void median::SortedArray<T>::push_back(T element)
{
    assert(size <= capacity);

    if (freeSpace() == 0) {
        this->grow();
    }

    assert(freeSpace() > 0);

    this->data[this->size] = element;
    ++this->size;

    // Potentially a dirty flag (with some sensible branch predictor) could
    // save us sorting until user calls \c at(), but as we will checking the
    // median every time we add a new value there won't be any gain here.
    this->sort();
}

template<typename T>
T median::SortedArray<T>::at(size_t index) const
{
    auto const out_of_bounds{index >= this->size};

    if (out_of_bounds) {
        throw std::runtime_error{"Out of bounds access"};
    }

    return data[index];
}

template<typename T>
size_t median::SortedArray<T>::getSize() const
{
    return this->size;
}

// Yes, that's poor man's sort without: randomization of input (for faster
// handling of sorted sequences); selection of better sorting algorithm for
// shorter sequences; smarter selection of pivot.
template<typename T>
void median::SortedArray<T>::qsort(
    size_t const first_index,
    size_t const last_index
) {
    // More elegant solution would be to just calculate a difference of indices
    // but that would require safe casting to long long.
    if (first_index > last_index) {
        return;
    }

    auto const elements{last_index - first_index + 1};

    if (elements < 2) {
        return;
    }

    auto left_index{first_index};
    auto right_index{last_index};

    auto left = [&left_index, this]() -> T& { return this->data[left_index]; };
    auto right = [&right_index, this]() -> T& { return this->data[right_index]; };

    size_t const pivot_index{first_index + (elements / 2)};
    T const pivot{this->at(pivot_index)};

    while (left_index <= right_index) {
        for (; left() < pivot; ++left_index) {}
        for (; right() > pivot; --right_index) {}

        if (left_index <= right_index) {
            std::swap(left(), right());
            ++left_index;
            --right_index;
        }
    }

    this->qsort(first_index, right_index);
    this->qsort(left_index, last_index);
}

template<typename T>
size_t median::SortedArray<T>::freeSpace()
{
    return size - capacity;
}

template<typename T>
void median::SortedArray<T>::sort()
{
    assert(this->size > 0);

    size_t const left{0};
    size_t const right{this->size - 1};

    this->qsort(left, right);
}

template<typename T>
size_t median::SortedArray<T>::getNewCapacity() const
{
    auto const new_size{this->size == 0 ? 1 : this->size * 2};

    auto const overflow{new_size <= this->size};
    if (overflow) {
        throw std::runtime_error{"Too many elements"};
    }

    return new_size;
}

template<typename T>
void median::SortedArray<T>::grow()
{
    auto const new_capacity{getNewCapacity()};
    auto new_buffer{std::make_unique<T[]>(new_capacity)};

    // If copy cannot be used, then we can get away with regular loop ... which
    // is of course suboptimal for trivially copyable types
    std::copy(&this->data[0], &this->data[this->size], &new_buffer[0]);

    this->data = std::move(new_buffer);
    this->capacity = new_capacity;
}
