#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

#include <string>

#include "bb/HashTable.hpp"

using Pair = std::pair<std::string, std::string>;

TEST_CASE("Construction with initialiser list fills table", "[hashtable]") {
    bb::HashTable<std::string, std::string> t{
        {"hello", "world"},
        {"world", "hello"}
    };

    REQUIRE(t["hello"] == "world");
    REQUIRE(t["world"] == "hello");
    REQUIRE_THAT(t, Catch::Matchers::UnorderedRangeEquals({Pair{"hello", "world"}, Pair{"world", "hello"}}));
}