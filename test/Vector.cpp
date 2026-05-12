#include <catch2/catch_test_macros.hpp>
#include "bb/Vector.hpp"

TEST_CASE("Vector", "[ctor]") {
    SECTION("default") {
        bb::Vector<int> v{};

        REQUIRE(v.size() == 0);
        REQUIRE(v.empty());
        REQUIRE(v.capacity() == 0);
    }
}