// Copyright 2017 Bartosz Bielecki

#pragma once

#include "DynamicArray.hpp"

#include <algorithm>
#include <memory>
#include <numeric>


namespace median {

enum class HeapOrder {
    MinHeap, // Smallest element at root
    MaxHeap // Largest element at root
};

/// Array-based binary heap implementation
/// @tparam T element type kept inside (has to be copyable and comparable)
/// @tparam order - total order of heap
/// @todo Check cache-oblivious implementation
template<typename T, HeapOrder order>
class BinaryHeap
{
public:
    BinaryHeap() = default;
    ~BinaryHeap() = default;

    BinaryHeap(BinaryHeap&& other) = default;
    BinaryHeap& operator=(BinaryHeap&& other) = default;

    BinaryHeap(BinaryHeap const& other) = delete;
    BinaryHeap& operator=(BinaryHeap const& other) = delete;

    void insert(T element);

    /// @throws if heap is empty
    T getRoot() const;

    /// @throws if heap is empty
    void eraseRoot();

    size_t getSize() const;

private:
    enum class NextStepDown {
        Stop,
        Left,
        Right
    };

    static bool shouldWalkUp(T const& parent_value, T const* element_value);
    static bool isHigher(T const& a, T const& b);
    static NextStepDown getNextStepDown(
        T const* parent_value, T const* left_child, T const* right_child
    );

    static constexpr size_t no_element = size_t(-1);

    // In case of huge numbers following functions will overflow, but this
    // really doesn't matter as well before that we will run out of available
    // memory.
    size_t getParentIndex(size_t index);
    size_t getLeftIndex(size_t index);
    size_t getRightIndex(size_t index);
    size_t getLastIndex() const;

    // Iterators would definitely be better here, so we wouldn't have to play
    // around with indices and T* all the time.
    T* getValue(size_t index);

    DynamicArray<T, 2> array;
};

}

// -------------------------------------------------------------------------- //

template<typename T, median::HeapOrder order>
void median::BinaryHeap<T, order>::insert(T const element)
{
    this->array.push_back(element);

    size_t parent_idx{no_element};
    T* element_value{};
    T* parent_value{nullptr};

    auto update = [&](size_t const element_idx) {
        assert(element_idx != no_element);

        parent_idx = this->getParentIndex(element_idx);
        element_value = this->getValue(element_idx);
        parent_value = this->getValue(parent_idx);
    };

    update(this->getLastIndex());

    // Heapify-up
    while (shouldWalkUp(*element_value, parent_value)) {
        std::swap(*parent_value, *element_value);
        update(parent_idx);
    }
}

template<typename T, median::HeapOrder order>
T median::BinaryHeap<T, order>::getRoot() const
{
    return this->array[0];
}

template<typename T, median::HeapOrder order>
void median::BinaryHeap<T, order>::eraseRoot()
{
    this->array[0] = this->array[this->getLastIndex()];
    this->array.shrink();

    T* parent_value{};
    size_t left_index{no_element};
    T* left_value{nullptr};
    size_t right_index{no_element};
    T* right_value{nullptr};

    auto update = [&](size_t const child_idx) {
        assert(child_idx != no_element);

        parent_value = this->getValue(child_idx);
        left_index = this->getLeftIndex(child_idx);
        left_value = this->getValue(left_index);
        right_index = this->getRightIndex(child_idx);
        right_value = this->getValue(right_index);
    };

    update(0);

    auto next_step = NextStepDown::Stop;
    // Heapify-down
    while (
        (next_step = getNextStepDown(parent_value, left_value, right_value))
        != NextStepDown::Stop
    ) {
        T* value_to_swap{nullptr};
        auto index_to_follow = no_element;

        if (next_step == NextStepDown::Left) {
            value_to_swap = left_value;
            index_to_follow = left_index;
        } else {
            value_to_swap = right_value;
            index_to_follow = right_index;
        }

        std::swap(*value_to_swap, *parent_value);
        update(index_to_follow);
    }
}

template<typename T, median::HeapOrder order>
size_t median::BinaryHeap<T, order>::getSize() const
{
    return this->array.getSize();
}

template<typename T, median::HeapOrder order>
size_t median::BinaryHeap<T, order>::getParentIndex(size_t const index)
{
    return index == 0 ? no_element : ((index + 1) / 2) - 1;
}

template<typename T, median::HeapOrder order>
size_t median::BinaryHeap<T, order>::getLeftIndex(size_t const index)
{
    auto const result = (2 * index) + 1;
    return result > this->getLastIndex() ? no_element : result;
}

template<typename T, median::HeapOrder order>
size_t median::BinaryHeap<T, order>::getRightIndex(size_t const index)
{
    auto const result = (2 * index) + 2;
    return result > this->getLastIndex() ? no_element : result;
}

template<typename T, median::HeapOrder order>
bool median::BinaryHeap<T, order>::shouldWalkUp(
    T const& child_value, T const* const parent_value
) {
    if (parent_value == nullptr) {
        return false;
    }

    return order == HeapOrder::MinHeap ?
        child_value < *parent_value :
        child_value > *parent_value;
}

template<typename T, median::HeapOrder order>
bool median::BinaryHeap<T, order>::isHigher(T const& a, T const& b)
{
    return order == HeapOrder::MinHeap ?  a < b : a > b;
}

template<typename T, median::HeapOrder order>
size_t median::BinaryHeap<T, order>::getLastIndex() const
{
    return this->array.getSize() - 1;
}

template<typename T, median::HeapOrder order>
typename median::BinaryHeap<T, order>::NextStepDown
median::BinaryHeap<T, order>::getNextStepDown(
    T const* const parent, T const* const left_child, T const* const right_child
) {
    if (!parent) {
        return NextStepDown::Stop;
    }

    auto new_path = parent;

    if (left_child && isHigher(*left_child, *new_path)) {
        new_path = left_child;
    }

    if (right_child && isHigher(*right_child, *new_path)) {
        new_path = right_child;
    }

    if (new_path == left_child) {
        return NextStepDown::Left;
    } else if (new_path == right_child) {
        return NextStepDown::Right;
    } else {
        assert(new_path == parent);
        return NextStepDown::Stop;
    }
}

template<typename T, median::HeapOrder order>
T* median::BinaryHeap<T, order>::getValue(size_t index)
{
    return
        index != no_element && index < this->array.getSize() ?
        &this->array[index] :
        nullptr;
}
