#include <gtest/gtest.h>
#include "../include/alloc.h"

// Проверка alloc и изменения offset  
TEST(AllocatorTest, BasicAllocation) {
    Allocator allocator;
    allocator.makeAllocator(100);

    char* ptr1 = allocator.alloc(50);
    ASSERT_NE(ptr1, nullptr);

    char* ptr2 = allocator.alloc(50);
    ASSERT_NE(ptr2, nullptr);

    char* ptr3 = allocator.alloc(1);
    ASSERT_EQ(ptr3, nullptr);
}

// Проверка reset
TEST(AllocatorTest, Reset) {
    Allocator allocator;
    allocator.makeAllocator(100);

    char* ptr1 = allocator.alloc(50);
    ASSERT_NE(ptr1, nullptr);

    allocator.reset();

    char* ptr2 = allocator.alloc(100);
    ASSERT_NE(ptr2, nullptr);
}

// проверка makeAllocator; много вызовов
TEST(AllocatorTest, MultipleMakeAllocatorCalls) {
    Allocator allocator;

    allocator.makeAllocator(100);
    char* ptr1 = allocator.alloc(100);
    ASSERT_NE(ptr1, nullptr);

    allocator.makeAllocator(50);
    char* ptr2 = allocator.alloc(50);
    ASSERT_NE(ptr2, nullptr);

    char* ptr3 = allocator.alloc(1); 
    ASSERT_EQ(ptr3, nullptr);
}