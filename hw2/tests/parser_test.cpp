#include <gtest/gtest.h>
#include "../include/parser.h"

// Проверка базового парсинга строки и числа
TEST(TokenParserTest, BasicParsing) {
    TokenParser parser;
    bool digit_callback_called = false;
    bool string_callback_called = false;
    parser.SetDigitTokenCallback([&](uint64_t) { digit_callback_called = true; });
    parser.SetStringTokenCallback([&](const std::string&) { string_callback_called = true;});
    parser.Parse("123 hahaha");
    ASSERT_TRUE(digit_callback_called);
    ASSERT_TRUE(string_callback_called);
}

// Проверка обработки числовых токенов при сбросе
TEST(TokenParserTest, ResetParsing) {
    TokenParser parser;
    int digit_count = 0;
    parser.SetDigitTokenCallback([&](uint64_t) { digit_count++; });
    parser.Parse("353 534 974");
    ASSERT_EQ(digit_count, 3);
    digit_count = 0;
    parser.Parse("111 222");
    ASSERT_EQ(digit_count, 2);
}

// Проверка вызовов SetStartCallback и SetEndCallback при повторном парсинге
TEST(TokenParserTest, MultipleParsingWithStartEndCallbacks) {
    TokenParser parser;
    int start_count = 0;
    int end_count = 0;
    parser.SetStartCallback([&]() { start_count++; });
    parser.SetEndCallback([&]() { end_count++; });
    parser.Parse("hello world");
    parser.Parse("123 456");
    ASSERT_EQ(start_count, 2);
    ASSERT_EQ(end_count, 2);
}

TEST(TokenParserTest, LargeNumberAndStringParsing) {
    TokenParser parser;
    bool large_number_as_string = false;
    std::string string_value;
    parser.SetStringTokenCallback([&](const std::string& token) {
        large_number_as_string = true;
        string_value = token;
    });
    parser.Parse("18446744073709551616"); 
    ASSERT_TRUE(large_number_as_string);
    ASSERT_EQ(string_value, "18446744073709551616");
}