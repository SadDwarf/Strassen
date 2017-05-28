#include <cmath>
#include <vector>
#include <exception>
#include <iostream>
#include <fstream>

#include "Matrix.h"

int main() {
    std::ofstream fout("output");
    std::ifstream fin("input");
    size_t n;
    fin >> n;
    Matrix<int> a(n);
    Matrix<int> b(n);
    std::cin >> a >> b;

    fout << a << std::endl << b << std::endl;
    
    Matrix<int> c = a * b;
    fout << c;
    fin.close();
    fout.close();
}