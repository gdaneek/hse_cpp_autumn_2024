#include "../include/sformat.hpp" 
#include <gtest/gtest.h>
#include <sstream>
#include <cassert>

TEST(FormatTest, BasicForm) {
    auto text = format("{1} + {1} = {0}", 2, "one");
    EXPECT_EQ(text, "one + one = 2");
}

TEST(FormatTest, SingleArg) {
    auto text = format("Hello, {0}!", "world");
    EXPECT_EQ(text, "Hello, world!");
}

TEST(FormatTest, MultipleArg) {
    auto text = format("{0} {1} {2}", "ab", "bc", "ac");
    EXPECT_EQ(text, "ab bc ac");
}

TEST(FormatTest, Empty) {
    auto text = format("");
    EXPECT_EQ(text, "");
}

TEST(FormatTest, EscapedBraces) {
    auto text = format("ftext {{ aaa }} btext");
    EXPECT_EQ(text, "ftext { aaa } btext");
}

TEST(FormatTest, ArgumentOOR) {
    EXPECT_THROW(format("Out of range: {2}", 1, 2), FormatException);
}

TEST(FormatTest, InvalidInd) {
    EXPECT_THROW(format("Invalid index: {a}", 1), FormatException);
}

TEST(FormatTest, UnmatchedOBrace) {
    EXPECT_THROW(format("Unmatched { brace", 1), FormatException);
}

TEST(FormatTest, UnmatchedCBrace) {
    EXPECT_THROW(format("Unmatched } brace", 1), FormatException);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}