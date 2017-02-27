// Copyright 2017 Bartosz Bielecki

#pragma once

#include "SortedArray.hpp"


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
    SortedArray<T> array{};
};

}

// -------------------------------------------------------------------------- //

template<typename T>
void median::SingleArrayMedian<T>::append(T value)
{
    this->array.push_back(value);
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
        auto const left = float(this->array.at(left_index));
        auto const right = float(this->array.at(right_index));

        // Should be more stable and precise than division of sum
        return (left / 2.0f) + (right / 2.0f);
    } else {
        auto const center_index = (size / 2);
        return float(this->array.at(center_index));
    }
}
