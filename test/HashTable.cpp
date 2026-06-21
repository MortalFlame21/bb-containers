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

TEST_CASE("Insert increases size of HashTable", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht{{"s420", "Steven"}};

    // WHEN
    ht.insert({"s069", "Tekashi"});

    // THEN
    REQUIRE(ht.size() == 2);
    REQUIRE_THAT(ht, Catch::Matchers::UnorderedRangeEquals({Pair{"s420", "Steven"}, Pair{"s069", "Tekashi"}}));
}

TEST_CASE("Double insert returns false insert iterator pair value", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht{};

    // WHEN
    auto p1{ht.insert({"a", "apple"})};
    auto p2{ht.insert({"a", "apple"})};

    // INSERT
    REQUIRE(p1.second);
    REQUIRE(p1.second != p2.second);
    REQUIRE(ht.size() == 1);
}

TEST_CASE("Insert causing a load factor equal or greater than max load factor causes rehash of table", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht{};

    // WHEN
    ht.insert({"a", "apple"});

    // THEN
    REQUIRE(ht.bucket_count() == 1);
    REQUIRE(ht.load_factor() == 1);
    REQUIRE(ht.load_factor() == ht.max_load_factor());

    // WHEN
    ht.insert({"b", "banana"});

    // THEN
    REQUIRE(ht.load_factor() < 1);
    REQUIRE_THAT(ht, Catch::Matchers::UnorderedRangeEquals({Pair{"a", "apple"}, Pair{"b", "banana"}}));
}

TEST_CASE("Finding existing key returns the correct iterator", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht{
        {"VOO", "Vanguard S&P 500 ETF"},
        {"VGT", "Vanguard Information Technology ETF"},
    };

    // WHEN
    auto it{ht.find("VOO")};

    // THEN
    REQUIRE(it != ht.end());
    REQUIRE(*it == Pair{"VOO", "Vanguard S&P 500 ETF"});
}

TEST_CASE("Finding non-existing key returns the end iterator", "[hashtable]") {
    bb::HashTable<std::string, std::string> ht{
        {"NVDA", "NVIDIA"},
        {"AAPL", "Apple"}
    };

    // WHEN
    auto it{ht.find("VOO")};

    // THEN
    REQUIRE(it == ht.end());
}

TEST_CASE("Subscript indexing updates key value pair and does not add new pair", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht{
        {"NVDA", "NVIDIA"},
        {"AAPL", "Appl"}
    };

    // WHEN
    ht["AAPL"] += "e";

    // THEN
    REQUIRE(ht["AAPL"] == "Apple");
    REQUIRE(ht.size() == 2);
    REQUIRE_THAT(ht, Catch::Matchers::UnorderedRangeEquals({Pair{"NVDA", "NVIDIA"}, Pair{"AAPL", "Apple"}}));
}

TEST_CASE("Subscript indexing updates inserts, increasing size", "[hashtable]") {
    // GIVEN
    bb::HashTable<std::string, std::string> ht{
        {"NVDA", "NVIDIA"},
        {"AAPL", "Apple"}
    };

    // WHEN
    ht["VOO"] = "Vanguard S&P 500 ETF";

    // THEN
    REQUIRE(ht.find("VOO") != ht.end());
    REQUIRE(ht.size() == 3);
    REQUIRE_THAT(ht, Catch::Matchers::UnorderedRangeEquals({
        Pair{"NVDA", "NVIDIA"}, Pair{"AAPL", "Apple"}, Pair{"VOO", "Vanguard S&P 500 ETF"}}));
}