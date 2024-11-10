#include "../include/parser.h"

void TokenParser::SetStartCallback(const std::function<void()>& callback) {
    start_callback_ = callback;
}

void TokenParser::SetEndCallback(const std::function<void()>& callback) {
    end_callback_ = callback;
}

void TokenParser::SetDigitTokenCallback(const std::function<void(uint64_t)>& callback) {
    digit_callback_ = callback;
}

void TokenParser::SetStringTokenCallback(const std::function<void(const std::string&)>& callback) {
    string_callback_ = callback;
}

void TokenParser::Parse(const std::string &input) {
    if (start_callback_) start_callback_();
    size_t pos = 0;
    while (pos < input.size()) {
        while (pos < input.size() && std::isspace(input[pos])) ++pos;
        if (pos >= input.size()) break;
        size_t start = pos;
        bool is_digit = true;
        while (pos < input.size() && !std::isspace(input[pos])) {
            if (!std::isdigit(input[pos])) is_digit = false;
            ++pos;
        }
        std::string token = input.substr(start, pos - start);
        if (is_digit) {
            try {
                uint64_t value = std::stoull(token);
                if (digit_callback_) digit_callback_(value);
            } 
            catch (const std::out_of_range&) {
                if (string_callback_) string_callback_(token);
            }
        } 
        else {
            if (string_callback_) string_callback_(token);      
        }
    }
    if (end_callback_) end_callback_();
}

/*
int main() {
    TokenParser parser;
    parser.SetStartCallback([]() {
        std::cout << "Начинаем парсинг...\n";
    });
    parser.SetEndCallback([]() {
        std::cout << "Парсинг завершен.\n";
    });
    parser.SetDigitTokenCallback([](uint64_t number) {
        std::cout << "Найдено число: " << number << "\n";
    });
    parser.SetStringTokenCallback([](const std::string& str) {
        std::cout << "Найдена строка: " << str << "\n";
    });
    std::string line;
    while (std::getline(std::cin, line)) {
        parser.Parse(line);
    }

    return 0;
}
*/
