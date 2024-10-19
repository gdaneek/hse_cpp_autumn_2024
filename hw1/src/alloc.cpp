#include <iostream>
#include "../include/alloc.h"

Allocator::Allocator() : memory(nullptr), totalSize(0), offset(0) {}

void Allocator::makeAllocator(size_t maxSize) {
    if (memory != nullptr) 
        delete[] memory; 
    memory = new char[maxSize];
    totalSize = maxSize;
    offset = 0; 
}

char* Allocator::alloc(size_t size) {
     if (offset + size > totalSize) 
        return nullptr;
    char* ptr = memory + offset; 
    offset += size;              
    return ptr;
}

void Allocator::reset() { offset = 0; }

Allocator::~Allocator() {
    if (memory != nullptr) 
        delete[] memory;
}

 
