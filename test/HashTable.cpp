#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

#include <string>

#include "bb/HashTable.hpp"

using Pair = std::pair<std::string, std::string>;

TEST_CASE("Default constructor creates a bucket size one", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht{};

    // THEN
    REQUIRE(ht.empty());
    REQUIRE(ht.size() == 0);
    REQUIRE(ht.bucket_count() == 1);
}

TEST_CASE("Construction with initializer list fills table", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht{
        {"hello", "world"},
        {"world", "hello"}
    };

    // THEN
    REQUIRE(ht.size() == 2);
    REQUIRE(ht["hello"] == "world");
    REQUIRE(ht["world"] == "hello");
    REQUIRE_THAT(ht, Catch::Matchers::UnorderedRangeEquals({Pair{"hello", "world"}, Pair{"world", "hello"}}));
}

TEST_CASE("Copy construction copies table correctly", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht1{
        {"john", "pork"},
        {"fort", "nite"}
    };

    // WHEN
    bb::HashTable<std::string, std::string> ht2{ht1};

    // THEN
    REQUIRE(ht1.size() == ht2.size());
    REQUIRE_THAT(ht1, Catch::Matchers::UnorderedRangeEquals({Pair{"john", "pork"}, Pair{"fort", "nite"}}));
    REQUIRE_THAT(ht2, Catch::Matchers::UnorderedRangeEquals({Pair{"john", "pork"}, Pair{"fort", "nite"}}));
}

TEST_CASE("Copy assignment copies table correctly", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht1{
        {"john", "pork"},
        {"fort", "nite"}
    };
    bb::HashTable<std::string, std::string> ht2{};

    // WHEN
    ht2 = ht1;

    // THEN
    REQUIRE(ht1.bucket_count() == ht2.bucket_count());
    REQUIRE_THAT(ht1, Catch::Matchers::UnorderedRangeEquals({Pair{"john", "pork"}, Pair{"fort", "nite"}}));
    REQUIRE_THAT(ht2, Catch::Matchers::UnorderedRangeEquals({Pair{"john", "pork"}, Pair{"fort", "nite"}}));
}

TEST_CASE("Move constructor copies table correctly", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht1{
        {"john", "pork"},
        {"fort", "nite"}
    };

    // WHEN
    bb::HashTable<std::string, std::string> ht2{std::move(ht1)};

    // THEN
    REQUIRE(ht1.bucket_count() == 1);
    REQUIRE(ht1.empty());
    REQUIRE(ht1.size() == 0);

    REQUIRE_THAT(ht2, Catch::Matchers::UnorderedRangeEquals({Pair{"john", "pork"}, Pair{"fort", "nite"}}));
}

TEST_CASE("Move assignment copies table correctly", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht1{
        {"john", "pork"},
        {"fort", "nite"}
    };
    bb::HashTable<std::string, std::string> ht2{};

    // WHEN
    ht2 = std::move(ht1);

    // THEN
    REQUIRE(ht1.bucket_count() == 1);
    REQUIRE(ht1.empty());
    REQUIRE(ht1.size() == 0);

    REQUIRE_THAT(ht2, Catch::Matchers::UnorderedRangeEquals({Pair{"john", "pork"}, Pair{"fort", "nite"}}));
}