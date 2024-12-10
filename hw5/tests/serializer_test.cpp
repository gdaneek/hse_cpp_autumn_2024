
#include "../include/serializer.hpp" 
#include <gtest/gtest.h>
#include <sstream>
#include <cassert>

struct Data {
    uint64_t a;
    bool b;
    uint64_t c;
    template <class Serializer>
    Error serialize(Serializer& serializer) { return serializer(a, b, c); }
};

TEST(SerializerTest, SaveAndLoad) { //good data
    Data x{1, true, 2};
    std::stringstream stream;
    Serializer serializer(stream);
    ASSERT_EQ(serializer.save(x), Error::NoError);
    Data y{0, false, 0};
    Deserializer deserializer(stream);
    ASSERT_EQ(deserializer.load(y), Error::NoError);
    EXPECT_EQ(x.a, y.a);
    EXPECT_EQ(x.b, y.b);
    EXPECT_EQ(x.c, y.c);
}

TEST(SerializerTest, CorruptedArchiveBool) { //bool
    std::stringstream stream("1 not_bool 3");
    Data y{0, false, 0};
    Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(y), Error::CorruptedArchive);
}

TEST(SerializerTest, CorruptedArchiveUint64) {  //uint64
    std::stringstream stream("not_uint64 true 3");
    Data y{0, false, 0};
    Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(y), Error::CorruptedArchive);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}