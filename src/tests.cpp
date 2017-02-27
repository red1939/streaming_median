// Copyright 2017 Bartosz Bielecki

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "SingleArrayMedian.hpp"
#include "SortedArray.hpp"


using Array = median::SortedArray<int>;

SCENARIO("table can grow", "[SortedArray]")
{
    Array array;

    constexpr int elements = 1000;

    WHEN("array is empty")
    {
        THEN("size is 0")
        {
            REQUIRE(array.getSize() == 0);
        }
    }

    WHEN("1000 elements are added")
    {
        for (int i = 0; i < elements; ++i) {
            array.push_back(42);
        };

        THEN("size is correctly reported")
        {
            REQUIRE(array.getSize() == elements);
        }
    }
}

SCENARIO("(less than) sorting is correct", "[SortedArray]")
{
    Array array;

    auto check = [&array](size_t index, int value) {
        REQUIRE(array.at(index) == value);
    };

    WHEN("one element is pushed")
    {
        array.push_back(42);

        THEN("it will be present")
        {
            check(0, 42);
        }
    }

    WHEN("two element are pushed")
    {
        array.push_back(3);
        array.push_back(1);

        THEN("they will be ordered")
        {
            check(0, 1);
            check(1, 3);
        }
    }

    WHEN("five elements are pushed")
    {
        array.push_back(5);
        array.push_back(2);
        array.push_back(4);
        array.push_back(3);
        array.push_back(-1);

        THEN("they will be placed correctly")
        {
            check(0, -1);
            check(1, 2);
            check(2, 3);
            check(3, 4);
            check(4, 5);
        }
    }

    WHEN("elements of the same value are pushed")
    {
        array.push_back(42);
        array.push_back(42);
        array.push_back(42);

        THEN("correct number of them will be reported")
        {
            check(0, 42);
            check(1, 42);
            check(2, 42);
        }
    }
}

SCENARIO("out of bounds index is reported", "[SortedArray]")
{
    Array array;

    WHEN("array is empty")
    {
        THEN("no index is correct")
        {
            REQUIRE_THROWS(array.at(0));
            REQUIRE_THROWS(array.at(100));
        }
    }

    WHEN("array is not empty")
    {
        array.push_back(42);

        THEN("going over max index will be reported")
        {
            REQUIRE_THROWS(array.at(1));
            REQUIRE_THROWS(array.at(2));
            REQUIRE_THROWS(array.at(3));
        }
    }
}

template <template<typename> class Median>
void test_empty_sequence()
{
    Median<int> m;

    WHEN("no elements are added") {
        THEN("median is equal 0") {
            REQUIRE(m.calculateMedian() == Approx(0.0f));
        }
    }
}

template <template<typename> class Median>
void test_one_element_sequence()
{
    Median<int> m;
    constexpr int value = 42;

    WHEN("one elements is added") {
        m.append(value);

        THEN("median is equal to this one element") {
            REQUIRE(m.calculateMedian() == Approx(value));
        }
    }
}

template <template<typename> class Median>
void test_two_elements_sequence()
{
    Median<int> m;
    constexpr int value = 4;
    constexpr int value2 = 0;

    WHEN("two, different elements are added") {
        m.append(value);
        m.append(value2);

        THEN("median is equal to average of these two") {
            REQUIRE(m.calculateMedian() == Approx(2.0f));
        }
    }
}

template <template<typename> class Median>
void test_three_elements_sequence()
{
    Median<int> m;
    constexpr int value = -1;
    constexpr int value2 = 15;
    constexpr int value3 = 4;

    WHEN("three, different elements are added") {
        m.append(value);
        m.append(value2);
        m.append(value3);

        THEN("median is equal to the middle element") {
            REQUIRE(m.calculateMedian() == Approx(value3));
        }
    }
}

template <template<typename> class Median>
void test_six_elements_sequence()
{
    Median<int> m;
    constexpr int value = -1;
    constexpr int value2 = 10;
    constexpr int value3 = 0;
    constexpr int value4 = 3;
    constexpr int value5 = 2;
    constexpr int value6 = -100;

    WHEN("six, different elements are added") {
        m.append(value);
        m.append(value2);
        m.append(value3);
        m.append(value4);
        m.append(value5);
        m.append(value6);

        THEN("median is equal to the average of middle elements") {
            REQUIRE(m.calculateMedian() == Approx(1.0f));
        }
    }
}

template <template<typename> class Median>
void test_same_elements_sequence()
{
    Median<int> m;
    constexpr int value = 13;

    WHEN("three, same elements are added") {
        m.append(value);
        m.append(value);
        m.append(value);

        THEN("median is equal to every element value") {
            REQUIRE(m.calculateMedian() == Approx(value));
        }
    }
}

SCENARIO("simple solution works", "[SingleArrayMedian]")
{
    test_empty_sequence<median::SingleArrayMedian>();
    test_one_element_sequence<median::SingleArrayMedian>();
    test_two_elements_sequence<median::SingleArrayMedian>();
    test_three_elements_sequence<median::SingleArrayMedian>();
    test_six_elements_sequence<median::SingleArrayMedian>();
    test_same_elements_sequence<median::SingleArrayMedian>();
}
