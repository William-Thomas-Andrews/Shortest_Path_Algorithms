#include "Matrix.hpp"

Matrix::Matrix() {}

Matrix::Matrix(int num_rows, int num_columns) : rows(num_rows), columns(num_columns), size(num_rows*num_columns) { // No item specification results in a Matrix of all 0s
    matrix_array = (double *) malloc(size * sizeof(double));
    for (int i = 0; i < size; i++) { matrix_array[i] = 0; }
}

Matrix::Matrix(double item, int num_rows, int num_columns) : rows(num_rows), columns(num_columns), size(num_rows*num_columns) { // A one valued matrix
    matrix_array = (double *) malloc(size * sizeof(double));
    for (int i = 0; i < size; i++) { matrix_array[i] = item; }
}

Matrix::Matrix::Matrix(const std::vector<double>& data, int num_rows, int num_columns) : rows(num_rows), columns(num_columns), size(num_rows*num_columns) { // Vector input
    if (data.size() != size) { throw std::invalid_argument("Size of array does not match dimension sizes."); }
    matrix_array = (double *) malloc(size * sizeof(double));
    for (int i = 0; i < size; i++) { matrix_array[i] = data[i]; }
}

Matrix::Matrix::Matrix(const std::vector<Vertex>& data, int num_rows, int num_columns) : rows(num_rows), columns(num_columns), size(num_rows*num_columns) { // Vector input
    if (data.size() != size) { throw std::invalid_argument("Size of array does not match dimension sizes."); }
    matrix_array = (double *) malloc(size * sizeof(double));
    for (int i = 0; i < size; i++) { matrix_array[i] = (double) (data[i]).val; }
    std::cout << matrix_array[3] << std::endl;
}

Matrix::Matrix(double* data, int data_size, int num_rows, int num_columns) : rows(num_rows), columns(num_columns), size(num_rows*num_columns) { // C-style array input
    if (data_size != size) { throw std::invalid_argument("Size of array does not match dimension sizes."); }
    matrix_array = (double *) malloc(size * sizeof(double));
    for (int i = 0; i < size; i++) { matrix_array[i] = data[i]; }
}

Matrix::~Matrix() { free(matrix_array); }



// Matrix Operators
Matrix Matrix::operator+(const Matrix& other) {
    if (rows != other.rows) { throw std::invalid_argument("Row sizes must match to perform matrix addition."); }
    if (columns != other.columns) { throw std::invalid_argument("Column sizes must match to perform matrix addition."); }
    Matrix return_matrix = Matrix(matrix_array, size, rows, columns);
    for (int i = 0; i < size; i++) { 
        return_matrix.matrix_array[i] += other.matrix_array[i]; 
    }
    return return_matrix;
}

Matrix Matrix::operator-(const Matrix& other) {
    if (rows != other.rows) { throw std::invalid_argument("Row sizes must match to perform matrix addition."); }
    if (columns != other.columns) { throw std::invalid_argument("Column sizes must match to perform matrix addition."); }
    Matrix return_matrix = Matrix(matrix_array, size, rows, columns);
    for (int i = 0; i < size; i++) {
        return_matrix.matrix_array[i] -= other.matrix_array[i];
    }
    return return_matrix;
}

Matrix Matrix::operator*(const Matrix& other) {
    if (rows != other.rows) { throw std::invalid_argument("Row sizes must match to perform matrix addition."); }
    if (columns != other.columns) { throw std::invalid_argument("Column sizes must match to perform matrix addition."); }
    Matrix return_matrix = Matrix(matrix_array, size, rows, columns);
    for (int i = 0; i < size; i++) {
        return_matrix.matrix_array[i] *= other.matrix_array[i];
    }
    return return_matrix;
}

Matrix Matrix::operator/(const Matrix& other) {
    if (rows != other.rows) { throw std::invalid_argument("Row sizes must match to perform matrix addition."); }
    if (columns != other.columns) { throw std::invalid_argument("Column sizes must match to perform matrix addition."); }
    Matrix return_matrix = Matrix(matrix_array, size, rows, columns);
    for (int i = 0; i < size; i++) {
        return_matrix.matrix_array[i] /= other.matrix_array[i];
    }
    return return_matrix;
}

void Matrix::operator=(const Matrix& other) { // Copy assignment
    if (this == &other) { return; }
    double* new_array = (double*) malloc(other.size * sizeof(double));
    for (int i = 0; i < other.size; i++) {
        new_array[i] = other.matrix_array[i];
    }
    // free(matrix_array)
    matrix_array = new_array;
    size = other.size;
    rows = other.rows;
    columns = other.columns;
}

bool Matrix::operator==(const Matrix& other) {
    if ((rows != other.rows) || (columns != other.columns)) { return false; }
    if (size != other.size) {
        return false;
    }
    for (int i = 0; i < other.size; i++) {
        if (matrix_array[i] != other.matrix_array[i]) {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) {
    if ((rows != other.rows) || (columns != other.columns)) {
        return true;
    }
    if (size != other.size) {
        return true;
    }
    for (int i = 0; i < other.size; i++) {
        if (matrix_array[i] != other.matrix_array[i]) {
            return true;
        }
    }
    return false;
}

std::vector<double> Matrix::get_data() const {
    std::vector<double> vec({});
    for (int i = 0; i < size; i++) {
        vec.push_back(matrix_array[i]);
    }
    return vec;
}

int Matrix::get_rows() {
    return rows;
}

int Matrix::get_cols() {
    return columns;
}

double* Matrix::get_data_address() {
    return matrix_array;
}

// int Matrix::get_size() { return sizeof(matrix_array); }
int Matrix::get_size() { return size; }

std::string Matrix::get_string() { 
    std::string str = "";
    for (int i = 0; i < size; i++) {
        str += std::to_string(matrix_array[i]);
        str += " ";
        if ((((i+1) % columns) == 0) && (i != size-1)) {
            str += '\n';
        }
    }
    return str;
}

double Matrix::get_element(int row_index, int col_index) {
    if (rows <= row_index) {
        throw std::out_of_range("Row index out of range");
        return INT_MIN;
    }
    if (0 > row_index) {
        throw std::out_of_range("Row index cannot be negative");
        return INT_MIN;
    }
    if (columns <= col_index) {
        throw std::out_of_range("Column index out of range");
        return INT_MIN;
    }
    if (0 > col_index) {
        throw std::out_of_range("Column index cannot be negative");
        return INT_MIN;
    }

    return matrix_array[(row_index * columns) + col_index];
}

double Matrix::sum_elements() {
    double sum = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            sum += get_element(i, j);
        }
    }
    return sum;
}

Matrix Matrix::Transpose() {
    Matrix B = Matrix(columns, rows);
    int b_index = 0;
    for (int i = 0; i < columns; i++) {
        for (int j = i; j < i+(columns*rows); j+=columns) {
            B.matrix_array[b_index] = matrix_array[j];
            b_index++;
        }
    }
    return B;
}

Matrix Matrix::operator()(int row_index) {
    if (rows <= row_index) { throw std::out_of_range("Row index out of range"); }
    if (0 > row_index) { throw std::out_of_range("Row index cannot be negative"); }
    double *return_data = (double *) malloc(columns * sizeof(double));
    for (int i = 0; i < columns; i++) {
        return_data[i] = matrix_array[i + row_index*columns];
    }
    return Matrix(return_data, columns, 1, columns);
}

double& Matrix::operator()(int row_index, int col_index) const {
    if (rows <= row_index) { throw std::out_of_range("Row index out of range"); }
    if (0 > row_index) { throw std::out_of_range("Row index cannot be negative"); }
    if (columns <= col_index) { throw std::out_of_range("Column index out of range"); }
    if (0 > col_index) { throw std::out_of_range("Column index cannot be negative"); }

    return matrix_array[(row_index * columns) + col_index];
}

std::ostream& operator<<(std::ostream& os, Matrix& A) {
    os << A.get_string();
    return os;
}

Matrix dot(const Matrix& A, const Matrix& B) {
    if (A.columns != B.rows) {
        throw std::invalid_argument("Matrix 1 colums do not match Matrix 2 rows.");
    }
    Matrix to_return = Matrix(A.rows, B.columns);
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < B.columns; j++) {
            for (int k = 0; k < B.rows; k++) {
                to_return(i, j) += A(i, k) * B(k, j);
            }
        }
    }
    return to_return;
}




// Extra-Class Scalar Operators
Matrix operator+(const Matrix& M, double val) {
    Matrix return_matrix = Matrix(M.matrix_array, M.size, M.rows, M.columns);
    for (int i = 0; i < return_matrix.rows; i++) {
        for (int j = 0; j < return_matrix.columns; j++) {
            return_matrix(i, j) += val;
        }
    }
    return return_matrix;
}

Matrix operator-(const Matrix& M, double val) {
    Matrix return_matrix = Matrix(M.matrix_array, M.size, M.rows, M.columns);
    for (int i = 0; i < return_matrix.rows; i++) {
        for (int j = 0; j < return_matrix.columns; j++) {
            return_matrix(i, j) -= val;
        }
    }
    return return_matrix;
}

Matrix operator*(const Matrix& M, double val) {
    Matrix return_matrix = Matrix(M.matrix_array, M.size, M.rows, M.columns);
    for (int i = 0; i < return_matrix.rows; i++) {
        for (int j = 0; j < return_matrix.columns; j++) {
            return_matrix(i, j) *= val;
        }
    }
    return return_matrix;
}

Matrix operator/(const Matrix& M, double val) {
    Matrix return_matrix = Matrix(M.matrix_array, M.size, M.rows, M.columns);
    for (int i = 0; i < return_matrix.rows; i++) {
        for (int j = 0; j < return_matrix.columns; j++) {
            return_matrix(i, j) /= val;
        }
    }
    return return_matrix;
}


// Extra-Class Scalar Operators - reversed
Matrix operator+(double val, const Matrix& M) {
    Matrix return_matrix = Matrix(M.matrix_array, M.size, M.rows, M.columns);
    for (int i = 0; i < return_matrix.rows; i++) {
        for (int j = 0; j < return_matrix.columns; j++) {
            return_matrix(i, j) += val;
        }
    }
    return return_matrix;
}

Matrix operator-(double val, const Matrix& M) {
    Matrix return_matrix = Matrix(M.matrix_array, M.size, M.rows, M.columns);
    for (int i = 0; i < return_matrix.rows; i++) {
        for (int j = 0; j < return_matrix.columns; j++) {
            return_matrix(i, j) = val - return_matrix(i, j);
        }
    }
    return return_matrix;
}

Matrix operator*(double val, const Matrix& M) {
    Matrix return_matrix = Matrix(M.matrix_array, M.size, M.rows, M.columns);
    for (int i = 0; i < return_matrix.rows; i++) {
        for (int j = 0; j < return_matrix.columns; j++) {
            return_matrix(i, j) *= val;
        }
    }
    return return_matrix;
}
Matrix operator/(double val, const Matrix& M) {
    Matrix return_matrix = Matrix(M.matrix_array, M.size, M.rows, M.rows);
    std::cout << "le " << return_matrix << std::endl;
    for (int i = 0; i < return_matrix.rows; i++) {
        for (int j = 0; j < return_matrix.columns; j++) {
            return_matrix(i, j) = val / return_matrix(i, j);
        }
    }
    return return_matrix;
}