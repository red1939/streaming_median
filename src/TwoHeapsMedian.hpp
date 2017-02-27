// Copyright 2016 Bartosz Bielecki

#pragma once

#include "BinaryHeap.hpp"


namespace median {

/// Streaming median implementation that uses two heaps where in one we keep
/// elements less than pivot, and in other greater or equal than pivot.
/// @details As heaps are only partially ordered they do not need to spend much
/// time performing ordering all elements, and by splitting them into two we
/// further decrease the time of insertion/removal. Downside is that we have to
/// constantly keep heaps with the same size so that pivot is the element that
/// is dividing our two sets.
template<typename T>
class TwoHeapsMedian {
public:
    /// Appends a new number to the stream
    void append(T value);

    /// Finds median of the numbers in the stream
    /// @return 0 if the stream is empty and regular median otherwise
    float calculateMedian() const;

private:
    BinaryHeap<T, HeapOrder::MaxHeap> left{};
    BinaryHeap<T, HeapOrder::MinHeap> right{};
};

}

template<typename T>
void median::TwoHeapsMedian<T>::append(T value)
{
    (void)value;
}

template<typename T>
float median::TwoHeapsMedian<T>::calculateMedian() const
{
    return 0.0f;
}
