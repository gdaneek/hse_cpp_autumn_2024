#include <iostream>
#include <stdexcept>

class Matrix {
private:
    class ProxyRow {
    private:
        int32_t* data_;
        size_t cols_;
    public:
        ProxyRow(size_t cols) : cols_{cols} { data_ = new int32_t[cols](); }
        ~ProxyRow() { delete[] data_; }
        int32_t& operator[](size_t j) { 
            if (j >= cols_) 
                throw std::out_of_range("Col index out of range");
            return data_[j]; 
        }
        const int32_t& operator[](size_t j) const { 
            if (j >= cols_) 
                throw std::out_of_range("Col index out of range");
            return data_[j]; 
        }
    };
    size_t rows_;
    size_t cols_;
    ProxyRow** data_;
public:
    Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
        data_ = new ProxyRow*[rows];
        for (size_t i = 0; i < rows; ++i) 
            data_[i] = new ProxyRow(cols);
    }

    ~Matrix() {
        for (size_t i = 0; i < rows_; ++i) 
            delete data_[i];
        delete[] data_;
    }

    size_t getRows() const { return rows_; }
    size_t getColumns() const { return cols_; }

    ProxyRow& operator[](size_t i) {
        if (i >= rows_) 
            throw std::out_of_range("Row index out of range");
        return *data_[i];
    }

    const ProxyRow& operator[](size_t i) const {
        if (i >= rows_) 
            throw std::out_of_range("Row index out of range");
        return *data_[i];
    }

    Matrix& operator*=(int32_t scalar) {
        for (size_t i = 0; i < rows_; ++i) 
            for (size_t j = 0; j < cols_; ++j) 
                (*data_[i])[j] *= scalar;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        for (size_t i = 0; i < matrix.rows_; ++i,  os << std::endl)
            for (size_t j = 0; j < matrix.cols_; ++j)
                os << matrix[i][j] << " ";
        return os;
    }

    Matrix operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) 
            throw std::out_of_range("Matrix dimensions must match for addition");
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_; ++i) 
            for (size_t j = 0; j < cols_; ++j) 
                result[i][j] = (*this)[i][j] + other[i][j];
        return result;
    }

    bool operator==(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) 
            return false;
        for (size_t i = 0; i < rows_; ++i) 
            for (size_t j = 0; j < cols_; ++j) 
                if ((*this)[i][j] != other[i][j]) 
                    return false;
        return true;
    }

    bool operator!=(const Matrix& other) const { return !(*this == other); }
};
