#include <cmath>
#include <vector>
#include <exception>
#include <iostream>

struct Cords {
    size_t x1 = 0;
    size_t x2 = 0;
    size_t size = 0;
};

class Matrix {
private:
    std::vector<int> array;
    size_t size_;
public:
    Matrix(size_t size) {
        double k = std::log2(size);
        if (int(k) != k) {
            size_ = std::pow(2, int(k) + 1);
        } else
            size_ = std::pow(2, int(k));
        array.resize(size_);
    }

    Matrix(Matrix& a, Matrix& b, Matrix& c, Matrix& d) {
        size_ = a.size() * 2;
        array.resize(size_);
        for (size_t i = 0; i < a.size(); ++i)
            for (size_t j = 0; j < a.size(); ++j) {
                (*this)(i, j) = a(i, j);
                (*this)(i, j) = b(i + a.size(), j);
                (*this)(i, j) = a(i, j + a.size());
                (*this)(i, j) = a(i + a.size(), j + a.size());
            }
    }

    int & operator ()(size_t i, size_t j) {
        return array[j * size_ + i];
    }

    int & operator() (size_t i, size_t j) const {
        return array[j * size_ + i];
    }

    size_t size() {
        return size_;
    }

    size_t size() const {
        return size_;
    }

    friend Matrix operator* (Matrix& a, Matrix& b) {
        Matrix c(a.size());
        for (size_t i = 0; i < a.size(); ++i)
            for (size_t j = 0; j < a.size(); ++j)
                for (size_t k = 0; k < a.size(); ++k)
                    c(i, j)+= a(k, j) * b(i, k);
        return c;
    }

    Matrix operator+= (Matrix& a) {
        for (size_t i = 0; i < a.size(); ++i)
            for (size_t j = 0; j < a.size(); ++j)
                (*this)(i, j) += a(i, j);
        return *this;
    }

    friend Matrix operator+ (Matrix a, Matrix& b) {
        a += b;
        return a;
    }

    Matrix operator-= (Matrix& a) {
        for (size_t i = 0; i < a.size(); ++i)
            for (size_t j = 0; j < a.size(); ++j)
                (*this)(i, j) -= a(i, j);
        return *this;
    }

    friend Matrix operator- (Matrix a, Matrix& b) {
        a -= b;
        return a;
    }
};

Matrix quarter(Matrix& a, int quarter) {
    Matrix c(a.size() / 2);
    int i, j, m, n;
    if (quarter == 1) {
        i = 0;
        j = 0;
    } else if (quarter == 2) {
        i = a.size() / 2;
        j = 0;
    } else if (quarter == 3) {
        i = 0;
        j = a.size() / 2;
    } else {
        i = a.size() / 2;
        j = a.size() / 2;
    }
    for (int l = 0; l < a.size() / 2; ++l)
        for (int k = 0; k < a.size() / 2; ++k)
            c(l, k) = a(i + l, j + k);
    return c;
}

Matrix strassen(Matrix& a, Matrix& b) {
    if (a.size() <= 64) {
        Matrix c(a.size());
        c = a * b;
        return c;
    } else
    Matrix m1(a.size() / 2) = strassen(quarter(a, 1) + quarter(a, 4), quarter(b, 1) + quarter(b, 4));
    Matrix m2(a.size() / 2) = strassen(quarter(a, 3) + quarter(a, 4), quarter(b, 1));
    Matrix m3(a.size() / 2) = strassen(quarter(a, 1), quarter(b, 2) - quarter(b, 4));
    Matrix m4(a.size() / 2) = strassen(quarter(a, 4), quarter(b, 3) - quarter(b, 1));
    Matrix m5(a.size() / 2) = strassen(quarter(a, 1) + quarter(a, 2), quarter(b, 4));
    Matrix m6(a.size() / 2) = strassen(quarter(a, 3) - quarter(a, 1), quarter(b, 1) + quarter(b, 2));
    Matrix m7(a.size() / 2) = strassen(quarter(a, 2) - quarter(a, 4), quarter(b, 3) + quarter(b, 4));
    Matrix c(m1 + m4 - m5 + m7, m3 + m5, m2 + m4, m1 - m2 + m3 + m6);
    return c;
};

int main() {
    size_t n;
    std::cin >> n;
    Matrix a(n), b(n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            int temp;
            std::cin >> temp;
            a(i, j) = temp;
        }
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            int temp;
            std::cin >> temp;
            b(i, j) = temp;
        }
    }
    Matrix c = strassen(a, b);
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j)
            std::cout << c(i, j);
}