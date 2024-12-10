#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP 

#include <iostream>
#include <sstream>
#include <string>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char m_sep = ' ';
    std::ostream& m_out;
    Error process(bool value) {
        m_out << (value ? "true" : "false") << m_sep;
        return Error::NoError;
    }

    Error process(uint64_t value) {
        m_out << value << m_sep;
        return Error::NoError;
    }

    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args) {
        if (process(std::forward<T>(value)) == Error::NoError)
            return process(std::forward<ArgsT>(args)...);
        return Error::CorruptedArchive;
    }
public:
    explicit Serializer(std::ostream& out) : m_out(out) {}
    template <class T>
    Error save(T& object) { return object.serialize(*this); }
    template <class... ArgsT>
    Error operator()(ArgsT... args) { return process(std::forward<ArgsT>(args)...); }    
};

class Deserializer
{
    std::istream& m_in;
     Error process(bool& value) {
        std::string text;
        m_in >> text;
        if (text == "true") value = true;
        else if (text == "false") value = false;
        else return Error::CorruptedArchive;
        return Error::NoError;
    }

    Error process(uint64_t& value) {
        std::string text;
        m_in >> text;
        try { value = std::stoull(text); }
        catch (...) { return Error::CorruptedArchive; }
        return Error::NoError;
    }

    template <class T, class... ArgsT>
    Error process(T& value, ArgsT&... args) {
        if (process(value) == Error::NoError)
            return process(args...);
        return Error::CorruptedArchive;
    }
public:
    explicit Deserializer(std::istream& in) : m_in(in) {}
    template <class T>
    Error load(T& object){ return object.serialize(*this); }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) { return process(args...); }
};

#endif // SERIALIZER_HPP