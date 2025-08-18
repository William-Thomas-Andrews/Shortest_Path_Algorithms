#pragma once

#include <iostream>
#include <vector>


class Matrix {
    private:
        double* matrix_array;
        int size;
        int rows;
        int columns;
        
    public:
        Matrix();
        Matrix(int num_rows, int num_columns);
        Matrix(double item, int num_rows, int num_columns);
        Matrix(const std::vector<double>& data, int num_rows, int num_columns);
        Matrix(double* data, int data_size, int num_rows, int num_columns);

        ~Matrix();

        // Operators
        Matrix operator+(const Matrix& other);
        Matrix operator-(const Matrix& other);
        Matrix operator*(const Matrix& other);
        Matrix operator/(const Matrix& other);
        void operator=(const Matrix& other);
        bool operator==(const Matrix& other);
        bool operator!=(const Matrix& other);

        Matrix operator+(double val);
        Matrix operator-(double val);
        Matrix operator*(double val);
        Matrix operator/(double val);

        std::vector<double> get_data() const ;

        int get_size();
        int get_rows();
        int get_cols();

        std::string get_string();

        double get_element(int row_index, int col_index);

        double sum_elements();

        Matrix Transpose();
        Matrix operator()(int row_index) ;
        double& operator()(int row_index, int col_index) const ;

        friend std::ostream& operator<<(std::ostream& os, const Matrix& A);
        friend Matrix dot(const Matrix& A, const Matrix& B);
        friend Matrix transpose(const Matrix& A);
};

std::ostream& operator<<(std::ostream& os, Matrix& A);

Matrix dot(const Matrix& A, const Matrix& B);