#pragma once

#include <stdexcept>
#include <iostream>

template <typename Num>
class Matrix {
private:
    std::vector<Num> array;
    size_t m_size;
/**
 *
 * @param quarter number of quarter for division for recursion
 * @return return quarter of input matrix
 */
    Matrix quarter(int quarter) const {
        Matrix c(size() / 2);
        int i, j;
        switch (quarter) {
            case 1:
                i = 0;          j = 0;          break;
            case 2:
                i = size() / 2; j = 0;          break;
            case 3:
                i = 0;          j = size() / 2; break;
            case 4:
                i = size() / 2; j = size() / 2; break;
            default:
                throw std::out_of_range("quarter = " + std::to_string(quarter));
        }
        for (size_t l = 0; l < size() / 2; ++l)
            for (size_t k = 0; k < size() / 2; ++k)
                c(l, k) = (*this)(i + l, j + k);
        return c;
    }
/**
 * n^3 multiplication without strassen algorithm
 * @param b
 * @return
 */
    Matrix simple_multiplication(const Matrix& b) const {
        Matrix c(size());
        for (size_t i = 0; i < size(); ++i)
            for (size_t j = 0; j < size(); ++j)
                for (size_t k = 0; k < size(); ++k)
                    c(i, j)+= (*this)(k, j) * b(i, k);
        return c;
    }
/**
 * wrong sizes check for multiplication
 * @param b
 */
    void check_size(const Matrix& b) const {
        if (size() != b.size())
            throw std::invalid_argument("this->size = " + std::to_string(size()) +
                                        " other.size = " + std::to_string(b.size()));
    }
/**
* bad indexes check
* @param i
* @param j
*/
    void check_range(size_t i, size_t j) const {
        if (m_size <= i || m_size <= j)
            throw std::out_of_range("size = " + std::to_string(m_size) +
                                    " i = " + std::to_string(i) +
                                    " j = " + std::to_string(j));
    }

public:
    Matrix(size_t size) {
        double k = std::log2(size);
        if (int(k) != k) {
            m_size = std::pow(2, int(k) + 1);
        } else
            m_size = std::pow(2, int(k));
        array.resize(m_size * m_size);
    }
/**
 * constructor to unite 4 matrixes in bigger one
 */
    Matrix(const Matrix& a, const Matrix& b, const Matrix& c, const Matrix& d) {
        m_size = a.size() * 2;
        array.resize(m_size * m_size);
        for (size_t i = 0; i < a.size(); ++i)
            for (size_t j = 0; j < a.size(); ++j) {
                (*this)(i,            j           ) = a(i, j);
                (*this)(i + a.size(), j           ) = b(i, j);
                (*this)(i,            j + a.size()) = c(i, j);
                (*this)(i + a.size(), j + a.size()) = d(i, j);
            }
    }

    int & operator () (size_t i, size_t j) {
        check_range(i, j);
        return array[i * m_size + j];
    }

    const int & operator() (size_t i, size_t j) const {
        check_range(i, j);
        return array[i * m_size + j];
    }

    size_t size() const {
        return m_size;
    }

    Matrix operator* (const Matrix& b) const {
        check_size(b);
        if (size() <= 2)
            return simple_multiplication(b);
        Matrix m1 = (quarter(1) + quarter(4)) * (b.quarter(1) + b.quarter(4));
        Matrix m2 = (quarter(3) + quarter(4)) * (b.quarter(1)               );
        Matrix m3 = (quarter(1)             ) * (b.quarter(2) - b.quarter(4));
        Matrix m4 = (quarter(4)             ) * (b.quarter(3) - b.quarter(1));
        Matrix m5 = (quarter(1) + quarter(2)) * (b.quarter(4)               );
        Matrix m6 = (quarter(3) - quarter(1)) * (b.quarter(1) + b.quarter(2));
        Matrix m7 = (quarter(2) - quarter(4)) * (b.quarter(3) + b.quarter(4));

        return Matrix(m1 + m4 - m5 + m7, m3 + m5, m2 + m4, m1 - m2 + m3 + m6);
    }

    Matrix& operator+= (const Matrix& a) {
        check_size(a);
        for (size_t i = 0; i < a.size(); ++i)
            for (size_t j = 0; j < a.size(); ++j)
                (*this)(i, j) += a(i, j);
        return *this;
    }

    Matrix operator+ (const Matrix& b) const {
        Matrix a = *this;
        return a += b;
    }

    Matrix& operator-= (const Matrix& a) {
        check_size(a);
        for (size_t i = 0; i < a.size(); ++i)
            for (size_t j = 0; j < a.size(); ++j)
                (*this)(i, j) -= a(i, j);
        return *this;
    }

    Matrix operator- (const Matrix& b) const {
        Matrix a = *this;
        return a -= b;
    }
};

template <typename Num>
std::ostream& operator << (std::ostream& out, const Matrix<Num>& m) noexcept {
    for (size_t i = 0; i < m.size(); ++i) {
        for (size_t j = 0; j < m.size(); ++j)
            out << m(i, j) << ' ';
        out << '\n';
    }
    return out;
}

template <typename Num>
std::istream& operator >> (std::istream& in, Matrix<Num>& m) noexcept {
    for (size_t i = 0; i < m.size(); ++i)
        for (size_t j = 0; j < m.size(); ++j)
            in >> m(i, j);
    return in;
}
