// Copyright 2016 Bartosz Bielecki

#pragma once

#include "BinaryHeap.hpp"


namespace median {

/// Streaming median implementation that uses two heaps where in one we keep
/// elements less than or equal pivot, and in other greater or equal pivot.
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
    size_t getTotalCount() const;
    bool isInitialSequence() const;
    void initialAppend(T value);
    void regularAppend(T value);
    void normalize();

    BinaryHeap<T, HeapOrder::MaxHeap> left{};
    BinaryHeap<T, HeapOrder::MinHeap> right{};
};

}

// -------------------------------------------------------------------------- //

template<typename T>
void median::TwoHeapsMedian<T>::append(T const value)
{
    // I doubt we need to do additional conditional check here - regular append
    // should be able to work just fine.

    if (this->isInitialSequence()) {
        this->initialAppend(value);
    } else {
        this->regularAppend(value);
    }

    this->normalize();
}

template<typename T>
float median::TwoHeapsMedian<T>::calculateMedian() const
{
    auto const size_left = this->left.getSize();
    auto const size_right = this->right.getSize();

    if (size_left + size_right == 0) {
        return 0.0;
    }

    if (size_left > size_right) {
        return this->left.getRoot();
    } else if (size_right > size_left) {
        return this->right.getRoot();
    } else {
        return float(this->left.getRoot() + this->right.getRoot()) / 2.0f;
    }
}

template<typename T>
size_t median::TwoHeapsMedian<T>::getTotalCount() const
{
    return this->left.getSize() + this->right.getSize();
}

template<typename T>
void median::TwoHeapsMedian<T>::regularAppend(T const value)
{
    auto const float_value = float(value);
    auto const median = calculateMedian();

    if (float_value < median) {
        this->left.insert(value);
    } else if (float_value > median) {
        this->right.insert(value);
    } else {
        if (this->right.getSize() < this->left.getSize()) {
            this->right.insert(value);
        } else {
            this->left.insert(value);
        }
    }

}

template<typename T>
bool median::TwoHeapsMedian<T>::isInitialSequence() const
{
    return this->getTotalCount() < 2;
}

template<typename T>
void median::TwoHeapsMedian<T>::initialAppend(T const value)
{
    auto const sum = this->getTotalCount();

    if (sum == 0) {
        this->left.insert(value);
    } else if (sum == 1) {
        auto const previous_root = this->left.getRoot();
        if (value > previous_root) {
            this->right.insert(value);
        } else {
            this->right.insert(previous_root);
            this->left.eraseRoot();
            this->left.insert(value);
        }
    } else {
        assert(false);
    }
}

template<typename T>
void median::TwoHeapsMedian<T>::normalize()
{
    auto const left_bigger = this->left.getSize() > this->right.getSize() + 1;
    auto const right_bigger = this->right.getSize() > this->left.getSize() + 1;

    // Thank God for polymorphic lambdas
    auto move_between_heaps = [](auto& from, auto& to) {
        auto const value = from.getRoot();
        from.eraseRoot();
        to.insert(value);
    };

    if (left_bigger) {
        move_between_heaps(this->left, this->right);
    } else if (right_bigger) {
        move_between_heaps(this->right, this->left);
    }
}
