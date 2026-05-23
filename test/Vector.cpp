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

TEST_CASE("Push back can increase capacity can increase if require reserve", "[vector]") {
    // GIVEN
    bb::Vector<int> v(3);

    // THEN
    CHECK(v.capacity() == 3);

    // WHEN
    v.push_back({});

    // THEN
    CHECK(v.size() == 4);
    CHECK(v.capacity() >= 4);
}

TEST_CASE("Vector::clear clears contents successfully", "[vector]") {
    // GIVEN
    bb::Vector<int> v(3);

    // WHEN
    v.clear();

    // THEN
    CHECK(v.empty());

    SECTION("Push element element on a cleared vector to increase size", "[vectors]") {
        // WHEN
        v.push_back(1);

        // THEN
        CHECK(v.size() == 1);
        CHECK(v.capacity() >= 1);
    }
}

TEST_CASE("Copy constructor on source to destination are different objects", "[vector]") {
	// GIVEN
	bb::Vector<int> vs{1, 2, 3};

	// WHEN
	auto vd{vs};

	// THEN
	REQUIRE(vs.begin() != vd.begin());
	REQUIRE_THAT(vd, Catch::Matchers::RangeEquals(vs));
}

TEST_CASE("Copy assignment on same objects ignores copy", "[vector]") {
	// GIVEN
	bb::Vector<int> v{1, 2, 3};
	auto begin{v.begin()};

	// WHEN
	v = v;

	// THEN
	REQUIRE(begin == v.begin());
	REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1, 2, 3}));
}

TEST_CASE("Copy assignment accurately copies all elements", "[vector]") {
	// GIVEN
	bb::Vector<int> vs{1, 2, 3};
	bb::Vector<int> vd{};
	auto begin{vd.begin()};

	// WHEN
	vd = vs;

	// THEN
	REQUIRE(begin != vd.begin());
	REQUIRE(vs.begin() != vd.begin());
	REQUIRE_THAT(vd, Catch::Matchers::RangeEquals({1, 2, 3}));
}

TEST_CASE("Move constructor has one invalidated pointer to moved", "[vector]") {
	// GIVEN
	bb::Vector<int> vs{1, 2, 3};
	auto begin{vs.begin()};

	// WHEN
	auto vd{std::move(vs)};

	// THEN
	REQUIRE(vs.begin() != vd.begin());
	REQUIRE(vs.begin() == nullptr);
	REQUIRE(begin == vd.begin());
	REQUIRE_THAT(vd, Catch::Matchers::RangeEquals({1, 2, 3}));
}

TEST_CASE("Move assignment invalidates the to be moved source", "[vector]") {
	// GIVEN
	bb::Vector<int> vs{1, 2, 3};
	bb::Vector<int> vd{};
	auto begin{vs.begin()};

	// WHEN
	vd = std::move(vs);

	// THEN
	REQUIRE(vs.begin() != vd.begin());
	REQUIRE(vs.begin() == nullptr);
	REQUIRE(begin == vd.begin());
	REQUIRE_THAT(vd, Catch::Matchers::RangeEquals({1, 2, 3}));
}

TEST_CASE("Erase using iterators removes elements in correct position", "[vector]") {
    // GIVEN
	bb::Vector<int> v{1, 2, 3};

    // WHEN
    v.erase(v.begin() + 1);

    // THEN
	REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1, 3}));

    // WHEN
    v.erase(v.end() - 1);

    // THEN
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1}));
}

TEST_CASE("Insert in middle pushes before elements to the end", "[vector]") {
    // GIVEN
	bb::Vector<int> v{1, 2, 3};

    // WHEN
    v.insert(v.begin() + 1, 2);

    // THEN
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1, 2, 2, 3}));

    // WHEN
    v.insert(v.begin() + 3, 3);

    // THEN
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1, 2, 2, 3, 3}));
}

TEST_CASE("Insert before other elements pushes elements correctly", "[vector]") {
    // GIVEN
	bb::Vector<int> v{1, 2, 3};

    // WHEN
    v.insert(v.begin(), 0);
    v.insert(v.begin() + v.size() - 1, 4);
    v.insert(v.begin() + v.size() - 1, 5);

    // THEN
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals({0, 1, 2, 4, 5, 3}));
}

TEST_CASE("Insert at end inserts elements correctly", "[vector]") {
    // GIVEN
	bb::Vector<int> v{1, 2, 3};

    // WHEN
    v.insert(v.end(), 4);
    v.insert(v.end(), 5);

    // THEN
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1, 2, 3, 4, 5}));
}

TEST_CASE("Insert with no elements at begining increases Vector size.", "[vector]") {
    // GIVEN
    bb::Vector<int> v{};

    // WHEN
    v.insert(v.begin(), 1);
    v.insert(v.begin(), 2);
    v.insert(v.begin(), 3);

    // THEN
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals({3, 2, 1}));
}

TEST_CASE("Inserted elements with a count are correctly in order", "[vector]") {
    // GIVEN
    bb::Vector<int> v{1, 1, 1};

    // WHEN
    v.insert(v.begin() + 1, 2, 3);

    // THEN
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1, 3, 3, 1, 1}));
}

TEST_CASE("Insert of extra 2 elements at the end increases Vector", "[vector]") {
    // GIVEN
    bb::Vector<int> v{1};

    // WHEN
    v.insert(v.end(), 4, 2);

    // THEN
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals({1, 2, 2, 2, 2}));
}