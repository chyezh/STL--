#include <iostream>
#include "vector.h"


class A {
 public:
  A() : i_(0) { std::cout << "default ctor" << std::endl; }
  A(const A &a) : i_(a.i_) { std::cout << "copy ctor" << std::endl; }
  A(int i) : i_(i) { std::cout << "normal ctor " << i << std::endl; }
  ~A() { std::cout << "dtor" << std::endl; }

 private:
  int i_;
};

int main() {
  stl::vector<A> a1(4);
  stl::vector<A> a2(4, A{2});
  stl::vector<A> a3{1, 2, 3, 4};
  return 0;
}
