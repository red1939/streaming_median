// Copyright 2017 Bartosz Bielecki

#pragma once

#include <cassert>

#include "DynamicArray.hpp"


namespace median {

/// Implements streaming median concept using a brute-force solution where a
/// constantly sorted array allows us to quickly find the middle element.
template<typename T>
class SingleArrayMedian {
public:
    /// Appends a new number to the stream
    void append(T value);

    /// Finds median of the numbers in the stream
    /// @return 0 if the stream is empty and regular median otherwise
    float calculateMedian() const;

private:
    void sort();
    void qsort(size_t left, size_t right);

    DynamicArray<T, 2> array{};
};

}

// -------------------------------------------------------------------------- //

template<typename T>
void median::SingleArrayMedian<T>::append(T value)
{
    this->array.push_back(value);
    this->sort();
}

template<typename T>
float median::SingleArrayMedian<T>::calculateMedian() const
{
    auto const size = this->array.getSize();

    if (size == 0) {
        return 0.0f;
    } else if ((size % 2) == 0) {
        auto const left_index = (size / 2) - 1;
        auto const right_index = left_index + 1;
        auto const left = float(this->array[left_index]);
        auto const right = float(this->array[right_index]);

        // Should be more stable and precise than division of sum
        return (left / 2.0f) + (right / 2.0f);
    } else {
        auto const center_index = (size / 2);
        return float(this->array[center_index]);
    }
}

// Yes, that's poor man's sort without: randomization of input (for faster
// handling of sorted sequences); selection of better sorting algorithm for
// shorter sequences; smarter selection of pivot.
template<typename T>
void median::SingleArrayMedian<T>::qsort(
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

    auto left = [&left_index, this]() -> T& { return this->array[left_index]; };
    auto right = [&right_index, this]() -> T& { return this->array[right_index]; };

    size_t const pivot_index{first_index + (elements / 2)};
    T const pivot{this->array[pivot_index]};

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
void median::SingleArrayMedian<T>::sort()
{
    assert(this->array.getSize() > 0);

    size_t const left{0};
    size_t const right{this->array.getSize() - 1};

    this->qsort(left, right);
}
