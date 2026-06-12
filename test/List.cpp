#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

#include "bb/List.hpp"

TEST_CASE("Default construction is an empty list", "[list]") {
    // GIVEN
    bb::List<int> l{};

    // THEN
    REQUIRE(l.size() == 0);
    REQUIRE(l.empty());
}

TEST_CASE("Explicit size creates size lengthed list of default values", "[list]") {
    // GIVEN
    bb::List<int> l(5);

    // THEN
    REQUIRE(l.size() == 5);
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({0, 0, 0, 0, 0}));

}

TEST_CASE("Explicit size with value creates sized lengthed list of values", "[list]") {
    // GIVEN
    bb::List<int> l(3, 100);

    // THEN
    REQUIRE(l.size() == 3);
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({100, 100, 100}));
}

TEST_CASE("Construction by initialiser list is copied into the List", "[list]") {
    // GIVEN
    bb::List<int> l{1, 2, 3, 4, 5};

    // THEN
    REQUIRE(l.size() == 5);
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1, 2, 3, 4, 5}));
}

TEST_CASE("Copy constructor on list are different objects", "[list]") {
    // GIVEN
    bb::List<int> ls(3, 100);

    // WHEN
    auto ld{ls};

    // THEN
    REQUIRE(ls.begin() != ld.begin());
	REQUIRE_THAT(ld, Catch::Matchers::RangeEquals(ls));
}

TEST_CASE("Copy assignment on copies all elements", "[list]") {
    // GIVEN
    bb::List<int> ls(3, 100);
    bb::List<int> ld{1, 2, 3};

    // WHEN
    ld = ls;

    // THEN
    REQUIRE(ls.begin() != ld.begin());
	REQUIRE_THAT(ld, Catch::Matchers::RangeEquals(ls));
}

TEST_CASE("Move construct on list invalidates source", "[list]") {
    // GIVEN
    bb::List<int> ls(5, 10);
    auto beg{ls.begin()};

    // WHEN
    bb::List<int> ld{std::move(ls)};

    // THEN
    REQUIRE(ld.begin() == beg);
    REQUIRE(ld.begin() != ls.begin());
    REQUIRE(ls.empty());
	REQUIRE_THAT(ld, Catch::Matchers::RangeEquals({10, 10, 10, 10, 10}));
}

TEST_CASE("Move assignment on list invalidates old elements", "[list]") {
    // GIVEN
    bb::List<int> ls{1, 2, 3, 4, 5};
    auto beg_ls{ls.begin()};

    bb::List<int> ld{420, 69, 21};
    auto beg_ld{ld.begin()};

    // WHEN
    ld = std::move(ls);

    // THEN
    REQUIRE(ld.begin() == beg_ls);
    REQUIRE(ld.begin() != beg_ld);
    REQUIRE(ld.begin() != ls.begin());
    REQUIRE(ls.empty());
	REQUIRE_THAT(ld, Catch::Matchers::RangeEquals({1, 2, 3, 4, 5}));
}

TEST_CASE("Clearing a list empties elements", "[list]") {
    // GIVEN
    bb::List<int> l{1, 2, 3, 4, 5};

    // WHEN
    l.clear();

    // THEN
    REQUIRE(l.empty());
    REQUIRE(l.begin() == l.end());
}

TEST_CASE("Inserting element at front of empty list increases size", "[list]") {
    // GIVEN
    bb::List<int> l{};

    // WHEN
    l.push_front(3);

    // THEN
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({3}));

    // WHEN
    l.push_front(2);
    l.push_front(1);

    // THEN
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1, 2, 3}));
}

TEST_CASE("Inserting element at back of list inserts element in correct order", "[list]") {
    // GIVEN
    bb::List<int> l{1, 2, 3};

    // WHEN
    l.push_back(4);

    // THEN
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1, 2, 3, 4}));

    // WHEN
    l.push_back(4);
    l.push_back(5);

    // THEN
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1, 2, 3, 4, 4, 5}));
}

TEST_CASE("Pop element from back removes last element and decreases size", "[list]") {
    // GIVEN
    bb::List<int> l{1, 2, 3};

    // WHEN
    l.pop_back();
    l.pop_back();

    // THEN
    REQUIRE(l.size() == 1);
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1}));

    // WHEN
    l.pop_back();

    // THEN
    REQUIRE(l.empty());
}

TEST_CASE("Pop element from front removes correct element", "[list]") {
    // GIVEN
    bb::List<int> l{1, 2};

    // WHEN
    l.pop_back();

    // THEN
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1}));

    // WHEN
    l.pop_back();

    // WHEN and THEN
    REQUIRE(l.empty());
    REQUIRE_THROWS_AS(l.pop_back(), std::out_of_range);
}

TEST_CASE("Inserting a element between a existing List inserts correctly", "[list]") {
    // GIVEN
    bb::List<int> l{1, 2, 4, 5};

    auto it{l.begin()};
    std::advance(it, 2);

    // WHEN
    l.insert(it, 3);

    // THEN
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1, 2, 3, 4, 5}));
}

TEST_CASE("Inserting at a empty List increases size", "[list]") {
    // GIVEN
    bb::List<int> l{};
    REQUIRE(l.empty());

    // WHEN
    l.insert(l.begin(), 1);

    // THEN
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1}));

    // WHEN
    l.insert(l.begin(), 0);
    l.insert(l.begin(), -1);

    // THEN
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({-1, 0, 1}));
}

TEST_CASE("Inserting elements via count increases size by count", "[list]") {
    // GIVEN
    bb::List l{1, 3};

    auto it{l.begin()};
    std::advance(it, 1);

    // WHEN
    l.insert(it, 3, 2);

    // THEN
    REQUIRE(l.size() == 5);
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1, 2, 2, 2, 3}));
}

TEST_CASE("Erasing at a empty List does nothing", "[list]") {
    // GIVEN
    bb::List<int> l{};

    // WHEN
    l.erase(l.begin());

    // THEN
    REQUIRE(l.empty());
}

TEST_CASE("Erasing element from a List removes item", "[list]") {
    // GIVEN
    bb::List<int> l{1, 2, 3, 4, 5};

    // WHEN
    l.erase(l.begin());
    l.erase(l.begin());

    // THEN
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({3, 4, 5}));
}

TEST_CASE("Erasing element from middle of list removes item correctly", "[list]") {
    // GIVEN
    bb::List<int> l{1, 2, 3, 4, 5};

    auto it{l.begin()};
    std::advance(it, 2);

    // WHEN
    l.erase(it);

    // THEN
    REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1, 2, 4, 5}));
}

TEST_CASE("Erasing elements within a range removes item correctly", "[list]") {
    SECTION("Range erase reduces list from [first, last)", "[list]") {
        // GIVEN
        bb::List<int> l{1, 2, 3, 4, 5};

        auto end{l.begin()};
        std::advance(end, 2);

        // WHEN
        l.erase(l.begin(), end);

        // THEN
        REQUIRE_THAT(l, Catch::Matchers::RangeEquals({3, 4, 5}));
    }

    SECTION("Range erase from middle to and reduces list size", "[list]") {
        // GIVEN
        bb::List<int> l{1, 2, 3, 4, 5};

        auto mid{l.begin()};
        std::advance(mid, 2);

        // WHEN
        l.erase(mid, l.end());

        // THEN
        REQUIRE_THAT(l, Catch::Matchers::RangeEquals({1, 2}));
    }
}

TEST_CASE("List splice tranfers all elements from other to *this", "[list]") {
    // GIVEN
    bb::List<int> l1{1, 2, 3, 4, 5};
    bb::List<int> l2{6, 7, 8, 9, 10};

    // WHEN
    l1.splice(l1.end(), l2);

    // THEN
    REQUIRE_THAT(l1, Catch::Matchers::RangeEquals({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    REQUIRE(l2.empty());
}