#include <catch2/catch_test_macros.hpp>
// don't use brace initialization for GENERATE()
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

#include "bb/Vector.hpp"

TEST_CASE("A default constructed vector is empty", "[vector]") {
    // GIVEN
    bb::Vector<int> v{};

    // THEN
    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());
    REQUIRE(v.capacity() == 0);

    SECTION("A empty vector pushed back adds size 1", "[vector]") {
        // WHEN
        v.push_back(1);

        // THEN
        REQUIRE(v.size() == 1);
        REQUIRE(v[0] == 1);
    }
}

TEST_CASE("A vector given a size, s, fills s default values", "[vector][generator]") {
    SECTION("Generator", "[generator]") {
        // GIVEN
        size_t size = GENERATE(1, 2, 3, 4, 5, 10, 100, 1000);
        bb::Vector<int> v(size);

        // THEN
        REQUIRE(v.size() == size);
        REQUIRE(v[0] == int{});
        REQUIRE(v[size / 2] == int{});
        REQUIRE(v[size - 1] == int{});
    }
}

TEST_CASE("A vector given a capacity, c, fills c sized vector", "[vector][generator]") {
    // GIVEN
    size_t capacity = GENERATE(1, 2, 3, 4, 5, 10, 100, 1000);
    bb::Vector<int> v(capacity);

    // THEN
    REQUIRE(v.size() == v.capacity());
    REQUIRE(v.capacity() == capacity);
    REQUIRE(v.size() == capacity);
}

TEST_CASE("A vector given a initializer list copies the contents", "[vector]") {
    // GIVEN
    bb::Vector<int> v{1, 2, 3};

    // WHEN
    v.push_back(4);

    // THEN
    REQUIRE(v.size() == 4);
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1, 2, 3, 4}));
}

TEST_CASE("A same resized vector will have no effect", "[vector]") {
    // GIVEN
    bb::Vector<int> v(3);

    // THEN
    REQUIRE(v.size() == 3);

    // WHEN
    v.resize(3);

    // THEN
    REQUIRE(v.size() == 3);

    SECTION("A greater size increase size", "[vector]") {
        // WHEN
        v.resize(5);

        // THEN
        REQUIRE(v.size() == 5);
    }

    SECTION("A lesser size decrease size", "[vector]") {
        // WHEN
        v.resize(1);

        // THEN
        REQUIRE(v.size() == 1);
    }
}

TEST_CASE("A reserved vector will have same size", "[vector]") {
    // GIVEN
    bb::Vector<int> v{1, 2, 3, 4, 5};

    // WHEN
    v.reserve(10);

    // THEN
    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() == 10);
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1, 2, 3, 4, 5}));

    SECTION("A lesser capacity and size has no effect", "[vector]") {
        // WHEN
        v.reserve(3);

        // THEN
        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() == 10);
        REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1, 2, 3, 4, 5}));
    }
}

TEST_CASE("Popped back elements decrease size", "[vector]") {
    // GIVEN
    bb::Vector<int> v{1, 2, 3};

    // WHEN
    v.pop_back();
    v.pop_back();

    // THEN
    REQUIRE(v.size() == 1);

    SECTION("When empty then resized elements are default initialised", "[vector]") {
        // GIVEN
        v.pop_back();
        REQUIRE(v.empty());

        // WHEN
        v.resize(2);

        // THEN
        REQUIRE_THAT(v, Catch::Matchers::RangeEquals({int{}, int{}}));
    }

    SECTION("When empty and popped back std::out_of_range exception thrown", "[vector]") {
        // GIVEN
        v.pop_back();
        REQUIRE(v.empty());

        // WHEN and THEN
        REQUIRE_THROWS_AS(v.pop_back(), std::out_of_range);
    }
}