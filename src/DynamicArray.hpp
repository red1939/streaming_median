// Copyright 2016 Bartosz Bielecki

#pragma once

#include <cassert>
#include <stdexcept>
#include <memory>
#include <utility>

// For std::copy
#include <algorithm>


namespace median {

/// Dynamically growing array
/// @remarks For simplicity one can only add elements to the array, not remove
/// them.
/// @remarks Delays allocation to moment when first element is added
/// @tparam T type of elements stored (implements copyable concept)
/// @tparam growth_factor used every time when we have not enough space
template<typename T, unsigned int growth_factor>
class DynamicArray {
public:
    void push_back(T element);
    size_t getSize() const;
    T& operator[](size_t index);
    T const& operator[](size_t index) const;

private:
    size_t freeSpace();
    size_t getNewCapacity() const;
    void grow();

    std::unique_ptr<T[]> data{nullptr};
    size_t capacity{0};
    size_t size{0};
};

}

// -------------------------------------------------------------------------- //

template<typename T, unsigned int growth_factor>
void median::DynamicArray<T, growth_factor>::push_back(T element)
{
    assert(size <= capacity);

    if (freeSpace() == 0) {
        this->grow();
    }

    assert(freeSpace() > 0);

    this->data[this->size] = element;
    ++this->size;
}

template<typename T, unsigned int growth_factor>
T& median::DynamicArray<T, growth_factor>::operator[](size_t index)
{
    return const_cast<T&>(
        const_cast<DynamicArray const*>(this)->operator[](index)
    );
}

template<typename T, unsigned int growth_factor>
T const& median::DynamicArray<T, growth_factor>::operator[](size_t index) const
{
    auto const out_of_bounds{index >= this->size};

    if (out_of_bounds) {
        throw std::runtime_error{"Out of bounds access"};
    }

    return data[index];
}

template<typename T, unsigned int growth_factor>
size_t median::DynamicArray<T, growth_factor>::getSize() const
{
    return this->size;
}

template<typename T, unsigned int growth_factor>
size_t median::DynamicArray<T, growth_factor>::freeSpace()
{
    return size - capacity;
}

template<typename T, unsigned int growth_factor>
size_t median::DynamicArray<T, growth_factor>::getNewCapacity() const
{
    auto const new_size{this->size == 0 ? 1 : this->size * growth_factor};

    auto const overflow{new_size <= this->size};
    if (overflow) {
        throw std::runtime_error{"Too many elements"};
    }

    return new_size;
}

template<typename T, unsigned int growth_factor>
void median::DynamicArray<T, growth_factor>::grow()
{
    auto const new_capacity{getNewCapacity()};
    auto new_buffer{std::make_unique<T[]>(new_capacity)};

    // If copy cannot be used, then we can get away with regular loop ... which
    // is of course suboptimal for trivially copyable types
    std::copy(&this->data[0], &this->data[this->size], &new_buffer[0]);

    this->data = std::move(new_buffer);
    this->capacity = new_capacity;
}
