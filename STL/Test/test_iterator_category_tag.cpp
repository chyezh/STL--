#include "stldef.h"
#include <vector>
#include <iostream>

int main() {
  if(stl::__is_random_access_iterator<std::vector<int>::iterator>::value)
    std::cout << "is input iterator" << std::endl;
  return 0;
}