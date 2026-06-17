#include <catch2/catch_test_macros.hpp>

#include <string>

#include "bb/HashTable.hpp"

TEST_CASE("Construction with initialiser list fills table", "[hashtable]") {
    bb::HashTable<std::string, std::string> t{};
    // t.insert({"hello", "world"});

    REQUIRE(t.begin() == t.end());
    // REQUIRE(t["hello"] == "world");
}