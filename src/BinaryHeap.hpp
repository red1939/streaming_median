// Copyright 2017 Bartosz Bielecki

#pragma once

#include <memory>


namespace median {

enum class HeapOrder {
    MinHeap, // Smallest element at root
    MaxHeap // Largest element at root
};

/// Array-based binary heap implementation
/// @tparam T element type kept inside (has to be copyable and comparable)
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
    std::unique_ptr<T[]> data{nullptr};
    size_t size{0};
};

}

// -------------------------------------------------------------------------- //

template<typename T, median::HeapOrder order>
size_t median::BinaryHeap<T, order>::getSize() const
{
    return 0;
}

template<typename T, median::HeapOrder order>
void median::BinaryHeap<T, order>::eraseRoot()
{
    // Noop
}

template<typename T, median::HeapOrder order>
void median::BinaryHeap<T, order>::insert(T element)
{
    (void)element;
}

template<typename T, median::HeapOrder order>
T median::BinaryHeap<T, order>::getRoot() const
{
    return T{};
}
