#include "__split_buffer.h"
#include <iostream>

int main() {
    stl::allocator<int> a;    
    stl::__split_buffer<int, stl::allocator<int>&> buffer(10, 3, a);
    buffer.copy_construct_at_end_(1,1);
    buffer.copy_construct_at_end_(1,2);
    buffer.copy_construct_at_end_(1,3);
    buffer.copy_construct_at_end_(1,4);
    buffer.copy_construct_at_end_(1,5);
    for(auto && i : buffer) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    std::cout << "front sparse: " << buffer.front_spare_() << std::endl;
    std::cout << "back sparse: " << buffer.back_spare_() << std::endl;
    std::cout << "size: " << buffer.size() << std::endl;
    std::cout << "capacity: " << buffer.capacity() << std::endl;
    buffer.reserve(20);
    std::cout << "size: " << buffer.size() << std::endl;
    std::cout << "capacity: " << buffer.capacity() << std::endl;
    buffer.shrink_to_fit();
    std::cout << "size: " << buffer.size() << std::endl;
    std::cout << "capacity: " << buffer.capacity() << std::endl;
    return 0;
}