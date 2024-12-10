#ifndef LA_HPP
#define LA_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>

class BigInt {
private:
    char* m_digits;    // Цифры (младший разряд - первый элемент)
    size_t m_size;   
    bool m_is_negative;

    void allocate(size_t size) {
        m_digits = new char[size]{};
        m_size = size;
    }

    void free() {
        if(m_digits != nullptr)
            delete[] m_digits;
        m_digits = nullptr;
        m_size = 0;
    }

    void trim() {
        while (m_size > 1 && m_digits[m_size - 1] == 0) --m_size;
        if (m_size == 1 && m_digits[0] == 0) m_is_negative = false;
    }

    int compare_abs(const BigInt& other) const {
        if (m_size != other.m_size)
            return m_size < other.m_size ? -1 : 1;
        for (size_t i = m_size; i > 0; --i)
            if (m_digits[i - 1] != other.m_digits[i - 1])
                return m_digits[i - 1] < other.m_digits[i - 1] ? -1 : 1;
        return 0;
    }

    BigInt add(const BigInt& other) const {
        size_t max_size = std::max(m_size, other.m_size);
        BigInt result;
        result.allocate(max_size + 1);
        int carry = 0;
        for (size_t i = 0; i < max_size || carry; ++i) {
            int current = carry;
            if (i < m_size) current += m_digits[i];
            if (i < other.m_size) current += other.m_digits[i];
            result.m_digits[i] = current % 10;
            carry = current / 10;
        }
        result.trim();
        return result;
    }

    BigInt subtract(const BigInt& other) const {
        BigInt result;
        result.allocate(m_size);
        int borrow = 0;
        for (size_t i = 0; i < m_size; ++i) {
            int current = m_digits[i] - borrow - (i < other.m_size ? other.m_digits[i] : 0);
            if (current < 0) {
                current += 10;
                borrow = 1;
            } else borrow = 0;
            result.m_digits[i] = current;
        }
        result.trim();
        return result;
    }

public:
    BigInt() : m_digits(nullptr), m_size(0), m_is_negative(false) { allocate(1); }

    BigInt(int32_t value) : m_digits(nullptr), m_size(0), m_is_negative(value < 0) {
        value = std::abs(value);
        m_size = value == 0 ? 1 : 0;
        int temp = value;
        while (temp > 0) {
            temp /= 10;
            ++m_size;
        }
        allocate(m_size);
        for (size_t i = 0; i < m_size; ++i) {
            m_digits[i] = value % 10;
            value /= 10;
        }
    }

    BigInt(const std::string& str) {
        if (str.empty())
            throw std::invalid_argument("Empty string not valid number");
        size_t start = 0;
        m_is_negative = false;
        if (str[0] == '-') {
            m_is_negative = true;
            start = 1;
        } else if (str[0] == '+') start = 1;
        m_size = str.size() - start;
        allocate(m_size);
        for (size_t i = 0; i < m_size; ++i) {
            if (!isdigit(str[start + i]))
                throw std::invalid_argument("Invalid char in number string");
            m_digits[m_size - 1 - i] = str[start + i] - '0';
        }
    }

    BigInt(const BigInt& other) : m_digits(nullptr), m_size(0), m_is_negative(other.m_is_negative) {
        allocate(other.m_size);
        std::memcpy(m_digits, other.m_digits, m_size);
    }

    BigInt(BigInt&& other) noexcept : m_digits(other.m_digits), m_size(other.m_size), m_is_negative(other.m_is_negative) {
        other.m_digits = nullptr;
        other.m_size = 0;
    }

    BigInt& operator=(const BigInt& other) {
        if (this != &other) {
            free();
            m_is_negative = other.m_is_negative;
            allocate(other.m_size);
            std::memcpy(m_digits, other.m_digits, m_size);
        }
        return *this;
    }

    BigInt& operator=(BigInt&& other) noexcept {
        if (this != &other) {
            free();
            m_digits = other.m_digits;
            m_size = other.m_size;
            m_is_negative = other.m_is_negative;
            other.m_digits = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    ~BigInt() {
        free();
    }

    BigInt operator+(const BigInt& other) const {
        if (m_is_negative == other.m_is_negative) {
            BigInt result = add(other);
            result.m_is_negative = m_is_negative;
            return result;
        }
        if (compare_abs(other) >= 0) {
            BigInt result = subtract(other);
            result.m_is_negative = m_is_negative;
            return result;
        }
        BigInt result = other.subtract(*this);
        result.m_is_negative = other.m_is_negative;
        return result;
    }

    BigInt operator-(const BigInt& other) const {
        return *this + (-other);
    }

    BigInt operator-() const {
        BigInt result(*this);
        if (m_size > 1 || m_digits[0] != 0) 
            result.m_is_negative = !m_is_negative;
        return result;
    }

    BigInt operator*(const BigInt& other) const {
        BigInt result;
        result.allocate(m_size + other.m_size);

        for (size_t i = 0; i < m_size; ++i) {
            int carry = 0;
            for (size_t j = 0; j < other.m_size || carry; ++j) {
                int64_t current = result.m_digits[i + j] + m_digits[i] * (j < other.m_size ? other.m_digits[j] : 0) + carry;
                result.m_digits[i + j] = current % 10;
                carry = current / 10;
            }
        }
        result.m_is_negative = m_is_negative != other.m_is_negative;
        result.trim();
        return result;
    }

    bool operator==(const BigInt& other) const {
        return m_is_negative == other.m_is_negative && compare_abs(other) == 0;
    }

    bool operator!=(const BigInt& other) const { return !(*this == other); }


    bool operator<(const BigInt& other) const {
        if (m_is_negative != other.m_is_negative)
            return m_is_negative;
        int cmp = compare_abs(other);
        return m_is_negative ? cmp > 0 : cmp < 0;
    }

    bool operator>(const BigInt& other) const {
        return other < *this;
    }

    bool operator<=(const BigInt& other) const {
        return !(other < *this);
    }

    bool operator>=(const BigInt& other) const {
        return !(*this < other);
    }

    friend std::ostream& operator<<(std::ostream& out, const BigInt& value) {
        if (value.m_is_negative) out << '-';
        for (size_t i = value.m_size; i > 0; --i) {
            out << char('0' + value.m_digits[i - 1]);
        }
        return out;
    }
};

#endif // LA_HPP
