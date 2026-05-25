#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

#include "bb/List.hpp"

struct test_t {
    int* v;

    test_t() : v{new int{}} { }
    test_t(int v_) : v{new int{v_}} { }
    ~test_t() { delete v; }
};

TEST_CASE("Default construction is an empty list", "[list]") {
    // GIVEN
    bb::List<test_t> l{};

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

// TEST_CASE("", "") {

// }