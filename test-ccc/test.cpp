#include <vector>
#include <iostream>

#include "mn_allocator.hpp"
#include "mn_basic_mempool.hpp"

#define ALLOC_TESTS_STRUCTS 20
#define ALLOC_TESTS_ALLOC 22

int START_VAR = 0;

struct alloc_test {
    int var;
    alloc_test() { var = 1; }
};

void test_alloc();
void test_pool();

int main() {
   test_pool(); return 0;
}
void test_pool() {
    // test_alloc();
    basic_mempool_vector<alloc_test, 20> pool;
    pool.create(2000);
    pool.set_blocked(3, true, 300);

    for(int i = 0; i < 5; i++)
        auto a = pool.allocate();

    int used = pool.get_used();
    int blocked = pool.get_blocked();
    int free = pool.get_free();
    int max = used + blocked + free;

    std::cout << "U: " << used << " B: " << blocked << " F: " << free << std::endl;
    std::cout << "R: " << max << " S: " << pool.size() << std::endl;

    auto chunk = pool.get_chunks(); int i = 0;
    for(auto it = chunk.begin(); it != chunk.end(); it++, i++) {
        std::cout << "-------" << i <<"-------" << std::endl;
        pool.print_chunk(*it);
    }
    
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