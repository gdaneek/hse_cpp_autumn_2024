#include <gtest/gtest.h>
#include <sstream>
#include "../include/matrix.hpp" 

TEST(MatrixTest, ConstructorAndSize) {
    Matrix m(5, 3);
    EXPECT_EQ(m.getRows(), 5);
    EXPECT_EQ(m.getColumns(), 3);
}

TEST(MatrixTest, ElementAccess) {
    Matrix m(3, 3);
    m[1][2] = 10;
    EXPECT_EQ(m[1][2], 10);

    EXPECT_THROW(m[3][2], std::out_of_range);
    EXPECT_THROW(m[1][3], std::out_of_range);
}

TEST(MatrixTest, ScalarMultiplication) {
    Matrix m(2, 2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;
    m *= 2;
    EXPECT_EQ(m[0][0], 2);
    EXPECT_EQ(m[0][1], 4);
    EXPECT_EQ(m[1][0], 6);
    EXPECT_EQ(m[1][1], 8);
}

TEST(MatrixTest, MatrixAddition) {
    Matrix m1(2, 2);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 3;
    m1[1][1] = 4;

    Matrix m2(2, 2);
    m2[0][0] = 5;
    m2[0][1] = 6;
    m2[1][0] = 7;
    m2[1][1] = 8;

    Matrix result = m1 + m2;

    EXPECT_EQ(result[0][0], 6);
    EXPECT_EQ(result[0][1], 8);
    EXPECT_EQ(result[1][0], 10);
    EXPECT_EQ(result[1][1], 12);
}

TEST(MatrixTest, MatrixAdditionDifferentSize) {
    Matrix m1(2, 2);
    Matrix m2(3, 3);
    EXPECT_THROW(m1 + m2, std::out_of_range);
}

TEST(MatrixTest, EqualityAndInequality) {
    Matrix m1(2, 2);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 3;
    m1[1][1] = 4;

    Matrix m2(2, 2);
    m2[0][0] = 1;
    m2[0][1] = 2;
    m2[1][0] = 3;
    m2[1][1] = 4;

    Matrix m3(2, 2);
    m3[0][0] = 5;
    m3[0][1] = 6;
    m3[1][0] = 7;
    m3[1][1] = 8;

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 == m3);
    EXPECT_TRUE(m1 != m3);
}

TEST(MatrixTest, OutputStream) {
    Matrix m(2, 2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;
    std::ostringstream os;
    os << m;
    std::string expected_output = "1 2 \n3 4 \n";
    EXPECT_EQ(os.str(), expected_output);
}
