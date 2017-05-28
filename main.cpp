#include <cmath>
#include <vector>
#include <exception>
#include <iostream>

#include "Matrix.h"

int main() {
    size_t n;
    std::cin >> n;
    Matrix<int> a(n);
    Matrix<int> b(n);
    std::cin >> a >> b;

    std::cout << a << std::endl << b << std::endl;

    Matrix<int> c = a * b;
    std::cout << c;
}