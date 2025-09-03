#pragma once

#include <iostream>
#include <vector>
#include <random>

#include "Vertex.hpp"


class Matrix {
    private:
        double* matrix_array;
        int size;
        int rows;
        int columns;
        
    public:
        Matrix();
        Matrix(int num_rows, int num_columns);
        Matrix(int num_rows, int num_columns, double lower_bound, double upper_bound);
        Matrix(double item, int num_rows, int num_columns);
        Matrix(const std::vector<double>& data, int num_rows, int num_columns);
        Matrix(const std::vector<Vertex>& data, int num_rows, int num_columns);
        Matrix(double* data, int data_size, int num_rows, int num_columns);
        Matrix(const Matrix& other);
        ~Matrix();

        // Operators
        Matrix& operator=(const Matrix& other);
        Matrix operator+(const Matrix& other);
        Matrix operator-(const Matrix& other);
        Matrix operator*(const Matrix& other);
        Matrix operator/(const Matrix& other);
        bool operator==(const Matrix& other);
        bool operator!=(const Matrix& other);

        std::vector<double> get_data() const ;

        double* get_data_address();

        int get_size();

        std::string get_string();

        double get_element(int row_index, int col_index);
        int get_rows();
        int get_cols();

        double sum_elements();
        double sum_row(int row);

        Matrix Transpose();
        double& operator()(int row_index, int col_index) const ;
        void clear();

        friend Matrix operator+(const Matrix& M, double val);
        friend Matrix operator-(const Matrix& M, double val);
        friend Matrix operator*(const Matrix& M, double val);
        friend Matrix operator/(const Matrix& M, double val);
        friend Matrix operator+(double val, const Matrix& M);
        friend Matrix operator-(double val, const Matrix& M);
        friend Matrix operator*(double val, const Matrix& M);
        friend Matrix operator/(double val, const Matrix& M);

        friend std::ostream& operator<<(std::ostream& os, const Matrix& A);
        friend Matrix dot(const Matrix& A, const Matrix& B);
        friend Matrix transpose(const Matrix& A);
};

std::ostream& operator<<(std::ostream& os, Matrix& A);

Matrix dot(const Matrix& A, const Matrix& B);