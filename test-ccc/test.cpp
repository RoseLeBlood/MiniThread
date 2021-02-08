#include <vector>
#include <iostream>

#include "mn_allocator.hpp"

#define ALLOC_TESTS_STRUCTS 10

struct alloc_test {
    int var;
};

void test_alloc();

int main() {
    test_alloc();
    return 0;
}

void test_alloc() {
    basic_allocator_system<alloc_test> alloc; 

    alloc.create(2);

    alloc_test* test  = alloc.alloc_range(ALLOC_TESTS_STRUCTS, 200);
    
    for(int i=0; i < ALLOC_TESTS_STRUCTS; i++) {
        test[i].var = i;
    }

    for(int i=0; i < ALLOC_TESTS_STRUCTS; i++) {
        std::cout << test[i].var  << " ";
    }
    std::cout << alloc.get_free() <<  std::endl;
}