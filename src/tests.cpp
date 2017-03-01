// Copyright 2017 Bartosz Bielecki

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "BinaryHeap.hpp"
#include "SingleArrayMedian.hpp"
#include "TwoHeapsMedian.hpp"


using Array = median::DynamicArray<int, 2>;
using MinHeap = median::BinaryHeap<int, median::HeapOrder::MinHeap>;
using MaxHeap = median::BinaryHeap<int, median::HeapOrder::MaxHeap>;


SCENARIO("table can grow and shrink", "[DynamicArray]")
{
    Array array;

    constexpr int elements = 1000;
    constexpr int value = 42;

    WHEN("array is empty") {
        THEN("size is 0") {
            REQUIRE(array.getSize() == 0);
        }
    }

    WHEN("1000 elements are added") {
        for (int i = 0; i < elements; ++i) {
            array.push_back(value);
        };

        THEN("size is correctly reported")
        {
            REQUIRE(array.getSize() == elements);
        }

        THEN("no elements are corrupted") {
            for (int i = 0; i < elements; ++i) {
                REQUIRE(array[i] == value);
            };
        }

        AND_WHEN("we shrink the array once") {
            array.shrink();

            THEN("it's size will get smaller by 1") {
                REQUIRE(array.getSize() == (elements - 1));
            }
        }
    }
}

SCENARIO("validation of contract is reported", "[DynamicArray]")
{
    Array array;

    WHEN("array is empty") {
        THEN("no index is correct") {
            REQUIRE_THROWS(array[0]);
            REQUIRE_THROWS(array[100]);
        }

        THEN("we can't shrink the array") {
            REQUIRE_THROWS(array.shrink());
        }
    }

    WHEN("array is not empty") {
        array.push_back(42);

        THEN("going over max index will be reported") {
            REQUIRE_THROWS(array[1]);
            REQUIRE_THROWS(array[2]);
            REQUIRE_THROWS(array[3]);
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

SCENARIO("optimal solution works", "[TwoHeapsMedian]")
{
    test_empty_sequence<median::TwoHeapsMedian>();
    test_one_element_sequence<median::TwoHeapsMedian>();
    test_two_elements_sequence<median::TwoHeapsMedian>();
    test_three_elements_sequence<median::TwoHeapsMedian>();
    test_six_elements_sequence<median::TwoHeapsMedian>();
    test_same_elements_sequence<median::TwoHeapsMedian>();
}

SCENARIO("size remains valid", "[BinaryHeap]")
{
    MinHeap min_heap;

    WHEN("heap is empty") {
        THEN("size is 0") {
            REQUIRE(min_heap.getSize() == 0);
        }
    }

    WHEN("one element is added to heap") {
        min_heap.insert(42);

        THEN("size is 1") {
            REQUIRE(min_heap.getSize() == 1);
        }

        AND_WHEN("it's erased") {
            min_heap.eraseRoot();

            THEN("size is back to 0") {
                REQUIRE(min_heap.getSize() == 0);
            }
        }
    }

    WHEN("ten elements are added to heap") {
        for (int i = 0; i < 10; ++i) {
            min_heap.insert(42);
        }

        THEN("size is 10") {
            REQUIRE(min_heap.getSize() == 10);
        }

        AND_WHEN("3 elements are removed") {
            min_heap.eraseRoot();
            min_heap.eraseRoot();
            min_heap.eraseRoot();

            THEN("size is 7") {
                REQUIRE(min_heap.getSize() == 7);
            }
        }
    }
}

SCENARIO("ordering remains valid for min heap", "[BinaryHeap]")
{
    MinHeap min_heap;

    constexpr auto first = 13;
    constexpr auto second = 44;
    constexpr auto third = -1;
    constexpr auto fourth = 5;
    constexpr auto fifth = 0;
    constexpr auto sixth = 78;

    WHEN("first element is added to heap") {
        min_heap.insert(first);

        THEN("it will become the root") {
            REQUIRE(min_heap.getRoot() == first);
        }

        AND_WHEN("second, larger element is added") {

        min_heap.insert(second);

        THEN("first element is still the root") {
            REQUIRE(min_heap.getRoot() == first);
        }

        AND_WHEN("third, smaller element is added") {

        min_heap.insert(third);

        THEN("third element becomes the root") {
            REQUIRE(min_heap.getRoot() == third);
        }

        AND_WHEN("several larger elements are added") {

        min_heap.insert(fourth);
        min_heap.insert(fifth);
        min_heap.insert(sixth);

        THEN("new levels of binary tree will be filled but root will stay "
             "the same")
        {
            REQUIRE(min_heap.getRoot() == third);
        }

        AND_WHEN("root element is removed") {

        min_heap.eraseRoot();

        THEN("new root will be found") {
            REQUIRE(min_heap.getRoot() == fifth);
        }
    }}}}}

    WHEN("multiple elements of the same value are added to heap") {
        constexpr auto same_value = 33;
        min_heap.insert(same_value);
        min_heap.insert(same_value);
        min_heap.insert(same_value);

        THEN("root will be one of the elements") {
            REQUIRE(min_heap.getRoot() == same_value);
        }

        AND_WHEN("root is erased") {
            min_heap.eraseRoot();

            THEN("root will still have the same value") {
                REQUIRE(min_heap.getRoot() == same_value);
            }
        }
    }
}

SCENARIO("ordering remains valid for max heap", "[BinaryHeap]")
{
    MaxHeap max_heap;

    constexpr auto first = 13;
    constexpr auto second = 44;
    constexpr auto third = -1;
    constexpr auto fourth = 5;
    constexpr auto fifth = 0;
    constexpr auto sixth = 78;

    WHEN("first element is added to heap") {
        max_heap.insert(first);

        THEN("it will become the root") {
            REQUIRE(max_heap.getRoot() == first);
        }

        AND_WHEN("second, larger element is added") {
        max_heap.insert(second);

        THEN("second element will become the new root") {
            REQUIRE(max_heap.getRoot() == second);
        }

        AND_WHEN("third, smaller element is added") {
        max_heap.insert(third);

        THEN("second element is still the root") {
            REQUIRE(max_heap.getRoot() == second);
        }

        AND_WHEN("several larger elements are added") {
        max_heap.insert(fourth);
        max_heap.insert(fifth);
        max_heap.insert(sixth);

        THEN("new levels of binary tree will be filled and the new largest "
             "root is selected")
        {
            REQUIRE(max_heap.getRoot() == sixth);
        }

        AND_WHEN("root element is removed") {
        max_heap.eraseRoot();

        THEN("new root will be found") {
            REQUIRE(max_heap.getRoot() == second);
        }
    }}}}}

    WHEN("multiple elements of the same value are added to heap") {
        constexpr auto same_value = 33;
        max_heap.insert(same_value);
        max_heap.insert(same_value);
        max_heap.insert(same_value);

        THEN("root will be one of the elements") {
            REQUIRE(max_heap.getRoot() == same_value);
        }

        AND_WHEN("root is erased") {
            max_heap.eraseRoot();

            THEN("root will still have the same value") {
                REQUIRE(max_heap.getRoot() == same_value);
            }
        }
    }
}

SCENARIO("invalid operations are handled correctly", "[BinaryHeap]")
{
    MinHeap heap;

    WHEN("we try to find root for empty heap") {
        THEN("heap will throw") {
            REQUIRE_THROWS(heap.getRoot());
        }
    }

    WHEN("we try to remove the root from empty heap") {
        THEN("heap will throw") {
            REQUIRE_THROWS(heap.eraseRoot());
        }
    }
}
