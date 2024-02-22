#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "hse_project.h"

TEST_CASE("Constructor from string", "[Constructor]") {
    Number num1("12345.6789");
    REQUIRE(num1.getIntegerPart() == vector<char>{1, 2, 3, 4, 5});
    REQUIRE(num1.getDecimalPart() == vector<char>{6, 7, 8, 9});

    Number num2("-987.654");
    REQUIRE(num2.getIntegerPart() == vector<char>{-1, 9, 8, 7});
    REQUIRE(num2.getDecimalPart().empty());
}

// TEST_CASE("Constructor from double", "[Constructor]") {
//     CustomNumber::Number num3(1234.56789);
//     REQUIRE(num3.getIntegerPart() == std::vector<int>{1, 2, 3, 4});
//     REQUIRE(num3.getDecimalPart() == std::vector<int>{5, 6, 7, 8, 9});

//     CustomNumber::Number num4(-9876.543210);
//     REQUIRE(num4.getIntegerPart() == std::vector<int>{-1, 9, 8, 7, 6});
//     REQUIRE(num4.getDecimalPart() == std::vector<int>{5, 4, 3, 2, 1, 0});
// }

// TEST_CASE("Constructor from long long", "[Constructor]") {
//     CustomNumber::Number num5(1234567890123456LL);
//     REQUIRE(num5.getIntegerPart() == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6});
//     REQUIRE(num5.getDecimalPart().empty());

//     CustomNumber::Number num6(-9876543210987654321LL);
//     REQUIRE(num6.getIntegerPart() == std::vector<int>{-1, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 9, 8, 7, 6, 5, 4, 3, 2, 1});
//     REQUIRE(num6.getDecimalPart().empty());
// }

// TEST_CASE("Constructor from unsigned int", "[Constructor]") {
//     CustomNumber::Number num7(12345u);
//     REQUIRE(num7.getIntegerPart() == std::vector<int>{1, 2, 3, 4, 5});
//     REQUIRE(num7.getDecimalPart().empty());
// }

// TEST_CASE("Constructor from unsigned long long", "[Constructor]") {
//     CustomNumber::Number num8(9876543210ULL);
//     REQUIRE(num8.getIntegerPart() == std::vector<int>{9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
//     REQUIRE(num8.getDecimalPart().empty());
// }

// TEST_CASE("Addition operator", "[Operator]") {
//     CustomNumber::Number num9("123.456");
//     Number::Number num10("789.012");
//     Number::Number result = num9 + num10;

//     REQUIRE(result.getIntegerPart() == std::vector<int>{9, 1, 2});
//     REQUIRE(result.getDecimalPart() == std::vector<int>{2, 4, 6});
// }

// TEST_CASE("Subtraction operator", "[Operator]") {
//     Number::Number num11("789.012");
//     Number::Number num12("123.456");
//     Number::Number result = num11 - num12;

//     REQUIRE(result.getIntegerPart() == std::vector<int>{6, 6, 5});
//     REQUIRE(result.getDecimalPart() == std::vector<int>{5, 5, 6});
// }

// Добавьте тесты для других операторов, функций и случаев использования