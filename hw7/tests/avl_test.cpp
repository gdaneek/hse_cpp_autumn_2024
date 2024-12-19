#include "../include/avl.hpp" 
#include <gtest/gtest.h>
#include <sstream>
#include <cassert>

TEST(AVLTreeTest, InsertAndFind) {
    avl<int, int> tree;
    tree.insert({10, 100});
    tree.insert({20, 200});
    tree.insert({5, 50});

    EXPECT_TRUE(tree.contains(10));
    EXPECT_TRUE(tree.contains(20));
    EXPECT_TRUE(tree.contains(5));
    EXPECT_FALSE(tree.contains(15));
}

TEST(AVLTreeTest, EraseAndSize) {
    avl<int, int> tree;
    tree.insert({10, 100});
    tree.insert({20, 200});
    tree.insert({5, 50});
    EXPECT_EQ(tree.size(), 3);

    tree.erase(10);
    EXPECT_FALSE(tree.contains(10));
    EXPECT_EQ(tree.size(), 2);

    tree.erase(5);
    EXPECT_FALSE(tree.contains(5));
    EXPECT_EQ(tree.size(), 1);
}

TEST(AVLTreeTest, Iterators) {
    avl<int, int> tree;
    tree.insert({10, 100});
    tree.insert({20, 200});
    tree.insert({5, 50});

    auto it = tree.begin();
    EXPECT_EQ(it->first, 5);
    EXPECT_EQ((++it)->first, 10);
    EXPECT_EQ((++it)->first, 20);
    EXPECT_EQ((--it)->first, 10);
    EXPECT_EQ(++(++it), tree.end());
}

TEST(AVLTreeTest, ClearAndEmpty) {
    avl<int, int> tree;
    tree.insert({10, 100});
    tree.insert({20, 200});

    tree.clear();
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}