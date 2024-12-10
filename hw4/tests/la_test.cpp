#include "../include/la.hpp"
#include <gtest/gtest.h>

TEST(BigIntTest, ConstructorAndOutput) {
    EXPECT_EQ(BigInt(123).operator==(BigInt("123")), true);
    EXPECT_EQ(BigInt(-456).operator==(BigInt("-456")), true);
    EXPECT_EQ(BigInt(0).operator==(BigInt("0")), true);
}

TEST(BigIntTest, Addition) {
    EXPECT_EQ(BigInt("999999999999999999") + BigInt("1"), BigInt("1000000000000000000"));
    EXPECT_EQ(BigInt(123) + 456, BigInt("579"));
    EXPECT_EQ(BigInt("123456") + BigInt("-123456"), BigInt(0));
}

TEST(BigIntTest, Subtraction) {
    EXPECT_EQ(BigInt("1000") - BigInt("1"), BigInt("999"));
    EXPECT_EQ(BigInt("500") - BigInt("123"), BigInt("377"));
    EXPECT_EQ(BigInt(0) - BigInt(1), BigInt("-1"));
}

TEST(BigIntTest, UnaryMinus) {
    EXPECT_EQ(-BigInt("123"), BigInt("-123"));
    EXPECT_EQ(-BigInt("-123"), BigInt("123"));
    EXPECT_EQ(-BigInt(0), BigInt(0));
}

TEST(BigIntTest, Equality) {
    EXPECT_EQ(BigInt("123"), BigInt("123"));
    EXPECT_NE(BigInt("123"), BigInt("-123"));
    EXPECT_EQ(BigInt("-123"), BigInt("-123"));
}

TEST(BigIntTest, Multiplication) {
    EXPECT_EQ(BigInt("123") * BigInt("456"), BigInt("56088"));
    EXPECT_EQ(BigInt("0") * BigInt("12345"), BigInt("0"));
    EXPECT_EQ(BigInt("-123") * BigInt("456"), BigInt("-56088"));
    EXPECT_EQ(BigInt("-123") * BigInt("-456"), BigInt("56088"));
}

TEST(BigIntTest, ComparisonOperators) {
    BigInt a("123");
    BigInt b("456");
    BigInt c("123");
    BigInt d("-123");
    BigInt e("-456");

    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(a >= c);
    EXPECT_TRUE(a == c);
    EXPECT_TRUE(a != b);
    
    EXPECT_TRUE(d < a);
    EXPECT_TRUE(d > e);
    EXPECT_FALSE(e > d);
    EXPECT_TRUE(e <= d);
    EXPECT_TRUE(d <= a);
    EXPECT_FALSE(d >= a);
    EXPECT_FALSE(d == a);
    EXPECT_TRUE(d != a);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
