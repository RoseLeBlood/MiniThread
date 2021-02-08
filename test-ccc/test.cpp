#include <vector>
#include <iostream>

#include "mn_allocator.hpp"

#define ALLOC_TESTS_STRUCTS 20
#define ALLOC_TESTS_ALLOC 22

int START_VAR = 0;

struct alloc_test {
    int var;
    alloc_test() { var = 1; }
};

void test_alloc();

int main() {
    test_alloc();
    return 0;
}

void test_alloc() {
    //allocator_stack_t<alloc_test, ALLOC_TESTS_STRUCTS> alloc; 
    allocator_buffer_t<alloc_test> alloc;
    alloc.create(ALLOC_TESTS_STRUCTS);

    alloc_test* test;

    alloc.alloc(200);
    std::cout << "free: " << alloc.get_free() << "/" << alloc.get_max() << " (" 
        << alloc.get_allocated() << ")" << std::endl;

    size_t n = alloc.calloc(ALLOC_TESTS_ALLOC, &test,200);
    std::cout << "AL: " << n << " from " << ALLOC_TESTS_ALLOC <<  std::endl;

    for(int i=0; i < n; i++) 
        std::cout << test->var << "::";
    
    std::cout << "free: " << alloc.get_free() << "/" << alloc.get_max() << " (" 
        << alloc.get_allocated() << ")" << std::endl;
}