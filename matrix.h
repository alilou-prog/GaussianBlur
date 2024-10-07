#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
using std::vector;

template <typename Object>
class matrix
{
    friend std::ostream & operator << (std::ostream & out, const matrix &m)
    {
        std::cout << std::endl;
        for(int i{0}; i < m.numrows(); ++i)
        {
            for(int j{0}; j < m.numcols(); ++j)
            {
                std::cout << m[i][j] << ' ';
            }
            std::cout << std::endl;
        }
        return out;
    }

public:
    // constructs a "null" matrix 
    // provides a null return for functions below
    matrix() = default; 

    // constructor
    matrix(int rows, int cols) : array(rows)
    {
        for (auto &thisRow : array)
            thisRow.resize(cols);
    }

    // BIG FIVE

    // copy constructor (array(v) use move constructor)
    matrix(vector<vector<Object>> m) : array(m) {}

    // move constructor
    matrix(vector<vector<Object>> &&m) : array(std::move(m)) {}

    // BIG FIVE

    const vector<Object> &operator[](int row) const
    {
        return array[row];
    }

    vector<Object> &operator[](int row)
    {
        return array[row];
    }

    size_t numrows() const
    {
        return array.size();
    }
    size_t numcols() const
    {
        return numrows() ? array[0].size() : 0;
    }

    // equality operator
    bool operator == (const matrix<Object> &rhs)
    {
        if(!hasSameSize(rhs))
        {
            return false;
        }
        else
        {
            for(int i{0}; i < numrows(); ++i)
            {
                for(int j{0}; j < numcols(); ++j)
                {
                    if (array[i][j] != rhs[i][j])
                        return false;
                }
            }
            return true;
        }
    }
    bool operator != (const matrix<Object> rhs)
    {
        return !(*this == rhs);
    }

    // linear algebra operations
    // -- rhs : right hand side --
    matrix operator+(const matrix<Object> &rhs)
    {
        // both matrices must have the same size
        if(hasSameSize(rhs))
        {
            size_t rows = numrows();
            size_t cols = numcols();

            matrix result(rows, cols);
            for(int i{0}; i < rows; ++i)
            {
                for(int j{0}; j < cols; ++j)
                {
                    result[i][j] = array[i][j] + rhs[i][j];
                }
            }

            return result;
        }

        return {};
    }

    matrix operator-()
    {
        size_t rows = numrows();
        size_t cols = numcols();

        matrix result(rows,cols);

        for(size_t i{0}; i < rows; ++i)
        {
            for(size_t j{0}; j < cols; ++j)
            {
                result[i][j] = - array[i][j];
            }
        }
        return result;
    }

    matrix operator-(const matrix<Object> &rhs)
    {
        return (*this) + (-rhs);
    }

    matrix operator* (const matrix<Object> &rhs)
    {
        // lhs : left hand side
        matrix &lhs = *this;

        size_t lhs_cols = lhs.numcols();
        size_t rhs_rows = rhs.numrows();

        if(lhs_cols == rhs_rows)
        {
            size_t result_rows = lhs.numrows();
            size_t result_cols = rhs.numcols();

            matrix result(result_rows, result_cols);
            for(size_t i{0}; i < result_rows; ++i)
            {
                for(size_t j{0}; j < result_rows; ++j)
                {
                    // rhs_rws equal lhs_cols
                    for(size_t k{0}; k < lhs_cols; ++k)
                    {
                        result[i][j] += lhs[i][k] * rhs[k][j];
                    }
                }
            }
            return result;
        }
        return {};
    }

    matrix transpose()
    {
        // t_ for transpose
        size_t t_rows {this->numcols()};
        size_t t_cols {this->numrows()};

        matrix tr(t_rows, t_cols);
        for(size_t i{0}; i < t_rows; ++i)
        {
            for(size_t j{0}; j < t_cols; ++j)
            {
                tr[i][j] = array[j][i];
            }
        }
        return tr;
    }

private:
    vector<vector<Object>> array;
    bool hasSameSize(const matrix<Object> &m)
    {
        size_t m_rows{m.numrows()};
        size_t m_cols{m.numcols()};
        if (m_rows == this->numrows() || m_cols == this->numcols())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

#endif
