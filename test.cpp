#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Successful Test Example") {
    int a = 5;
    CHECK(a == 5);
}
TEST_CASE("Failing Test Examples") {
    CHECK(true == false);
}
