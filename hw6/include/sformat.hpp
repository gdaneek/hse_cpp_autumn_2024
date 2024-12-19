#ifndef SFORMAT_HPP
#define SFORMAT_HPP

#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <concepts> // c++20 мин

class FormatException : public std::runtime_error {
public:
    explicit FormatException(const std::string& message) : std::runtime_error(message) {}
};

// концепт на проверку поддержки вывода
template <typename T>
concept Streamable = requires(std::ostream& os, T value) {
    { os << value } -> std::same_as<std::ostream&>;
};

template <Streamable T>
std::string to_string(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <typename... Args>
std::string format(const std::string& formatString, const Args&... args) {
    std::vector<std::string> arguments = {to_string(args)...};
    std::string result;
    size_t i{0};
    while (i < formatString.size()) {
        if (formatString[i] == '{') {
            // "{{"
            if (i + 1 < formatString.size() && formatString[i + 1] == '{') {
                result += '{';
                i += 2;
                continue;
            }

            size_t closingBrace {formatString.find('}', i)};
            if (closingBrace == std::string::npos) 
                throw FormatException("Unmatched opening brace at pos " + std::to_string(i));

            std::string indexStr = formatString.substr(i + 1, closingBrace - i - 1);
            if (indexStr.empty() || !std::all_of(indexStr.begin(), indexStr.end(), ::isdigit)) 
                throw FormatException("Invalid arg ind at pos " + std::to_string(i));

            size_t index = std::stoi(indexStr);
            if (index >= arguments.size())
                throw FormatException("Arg ind out of range: " + std::to_string(index));

            result += arguments[index];
            i = closingBrace + 1;
        } else if (formatString[i] == '}') {
            // "}}"
            if (i + 1 < formatString.size() && formatString[i + 1] == '}') {
                result += '}';
                i += 2; 
                continue;
            }
            throw FormatException("Unmatched closing brace at pos " + std::to_string(i));
        } else {
            result += formatString[i];
            ++i;
        }
    }
    return result;
}

#endif // SFORMAT_HPP