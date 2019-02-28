#include <iostream>
#include <chrono>

#include "matrix.h"

using namespace matrix;

int main() {
  auto ident = make_ident<double>(8);
  std::cout << ident;

  M<double> a =
    {{ 6,  7, -3,  4}, 
     {-2,  0, -1,  5}};
  std::cout << a;

  M<double> b =
    {{ 6,  7},
     { 2,  3}, 
     { 8,  0},
     { 9, -2}};
  std::cout << b;

  auto c = a * b;

  std::cout << c;
  
  std::vector<decltype(&mul_00<double>)> funcs = {
    &mul_00,
    &mul_01
  };

  auto a1 = make_rand<double>(2000, 400);
  auto b1 = make_rand<double>(400, 1000);
  auto c1 = M<double>(a1.m_, b1.n_);

  int i = 0;
  for (auto func : funcs) {
    auto start = std::chrono::high_resolution_clock::now();
    func(a1, b1, c1);
    std::cout 
      << i++ << " - "
      << std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start).count()
      << std::endl;
  }

  return 0;
}
