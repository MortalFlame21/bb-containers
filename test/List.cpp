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
    bb::List<int> ld{};

    // WHEN
    ld = ls;

    REQUIRE(ls.begin() != ld.begin());
	REQUIRE_THAT(ld, Catch::Matchers::RangeEquals(ls));
}