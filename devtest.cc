#include <typeinfo>
#include <iostream>
#include "utils.h"

char foo(int a, float b, char c, bool d) {
  return 'c';
}

int main() {
  // std::cout << make_currying(foo)(1)(0.1)('c')(false) << std::endl;
  //auto c = make_currying(foo)(1)(0.1)('c')(false);
  auto f1 = make_currying(foo)(1);
  std::cout << f1(0.1)('c')(false) << std::endl;
  return 0;
}