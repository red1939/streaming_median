// Copyright 2017 Bartosz Bielecki

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
    SortedArray heap;
};

}

template<typename T, class Heap>
void median::SingleArrayMedian<T, Heap>::append(T value)
{
    (void)value;
}

template<typename T, class Heap>
float median::SingleArrayMedian<T, Heap>::calculateMedian() const
{
    return 0.0;
}
