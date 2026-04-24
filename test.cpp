
#include <iostream>
#include <vector>
#include "CSRMatrix.hpp"

int main() {
    // Test case 1: Create a 3x3 matrix with some non-zero elements
    std::vector<std::vector<int>> dense = {
        {1, 0, 2},
        {0, 0, 0},
        {3, 4, 5}
    };
    
    sjtu::CSRMatrix<int> matrix(3, 3, dense);
    
    // Test getRowSize, getColSize, getNonZeroCount
    std::cout << "Rows: " << matrix.getRowSize() << std::endl;
    std::cout << "Cols: " << matrix.getColSize() << std::endl;
    std::cout << "Non-zero count: " << matrix.getNonZeroCount() << std::endl;
    
    // Test get
    std::cout << "matrix[0][0]: " << matrix.get(0, 0) << std::endl;
    std::cout << "matrix[0][1]: " << matrix.get(0, 1) << std::endl;
    std::cout << "matrix[0][2]: " << matrix.get(0, 2) << std::endl;
    std::cout << "matrix[2][2]: " << matrix.get(2, 2) << std::endl;
    
    // Test set
    matrix.set(1, 1, 9);
    std::cout << "matrix[1][1] after set: " << matrix.get(1, 1) << std::endl;
    
    // Test getMatrix
    auto dense_result = matrix.getMatrix();
    std::cout << "Dense matrix:" << std::endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << dense_result[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    // Test matrix-vector multiplication
    std::vector<int> vec = {1, 2, 3};
    auto result = matrix * vec;
    std::cout << "Matrix-vector multiplication result:" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;
    
    // Test getRowSlice
    auto slice = matrix.getRowSlice(1, 3);
    std::cout << "Row slice dimensions: " << slice.getRowSize() << "x" << slice.getColSize() << std::endl;
    
    return 0;
}
