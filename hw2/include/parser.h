#include <iostream>
#include <functional>
#include <string>
#include <cctype>
#include <cstdint>

class TokenParser {
    std::function<void()> start_callback_;
    std::function<void()> end_callback_;
    std::function<void(uint64_t)> digit_callback_;
    std::function<void(const std::string&)> string_callback_;
public:
    void SetStartCallback(const std::function<void()>& callback);
    void SetEndCallback(const std::function<void()>& callback);
    void SetDigitTokenCallback(const std::function<void(uint64_t)>& callback);
    void SetStringTokenCallback(const std::function<void(const std::string&)>& callback);
    void Parse(const std::string &input); 
};

