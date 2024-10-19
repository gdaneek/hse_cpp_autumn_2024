#include <iostream>
class Allocator {
private:
    char* memory;     // Указатель на начало выделенной памяти
    size_t totalSize; // Общий размер выделенной памяти
    size_t offset;    // Смещение для следующего выделения
public:
    Allocator();
    void makeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
    ~Allocator();
};
 
