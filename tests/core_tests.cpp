#include <catch2/catch_test_macros.hpp>
#include <database.hpp>

TEST_CASE("should be possible to create a collection", "[database]")
{
    Database db;
    SECTION("to create a new collection should return true")
    {
        bool result = db.use_collection("default");
        REQUIRE(result == true);
    }
}