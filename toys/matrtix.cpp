#include <iostream>
#include <algorithm>
#include <vector>

template<typename T>
struct Matrix {
    int rows, cols;
    std::vector<std::vector<T>> _data;
    
    Matrix(int rows, int cols, const std::vector<std::vector<T>> &_data)
    : rows(rows), cols(cols), _data(_data)
    {}

    Matrix(int rows, int cols) 
    : rows(rows), cols(cols)
    {
        _data = std::move(std::vector<std::vector<T>>(rows, std::vector<T>(cols, 0)));
    }

    Matrix(const Matrix<T>&) = default;

    Matrix(Matrix<T>&& matrix)
    {
        _data = std::move(matrix._data);
        rows = matrix.rows;
        cols = matrix.cols;
    }

    Matrix<T>& operator=(Matrix<T>&& matrix)
    {
        _data = std::move(matrix._data);
        rows = matrix.rows;
        cols = matrix.cols;

        return *this;
    }

    Matrix& operator=(const Matrix<T>& m) = default;

    static Matrix mul(const Matrix<T>& m1, const Matrix<T>& m2, const T MOD)
    {
        Matrix res(m1.rows, m2.cols);
        for (int i = 0; i < res.rows; i++)
            for (int j = 0; j < res.cols; j++)
                for (int k = 0; k < m1.cols; k++)
                    res._data[i][j] = (res._data[i][j] + (m1._data[i][k] * m2._data[k][j]) % MOD) % MOD;

        return res;
    }

    Matrix operator*(const Matrix<T>& m)
    {
        Matrix res(rows, m.cols);
        for (int i = 0; i < res.rows; i++)
            for (int j = 0; j < res.cols; j++)
                for (int k = 0; k < cols; k++)
                    res._data[i][j] += _data[i][k] * m._data[k][j];

        return res;
    }

    static Matrix<T> pow(Matrix<T> a, Matrix<T> res, int n, T MOD)
    {
        while (n) {
            if (n & 1) res = Matrix::mul(a, res, MOD);
            a = Matrix::mul(a, a, MOD);
            n >>= 1;
        }

        return res;
    }
};

int main()
{
    
}