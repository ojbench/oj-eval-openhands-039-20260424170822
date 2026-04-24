
#include <iostream>
#include <vector>
#include "CSRMatrix.hpp"

int main() {
    // Read the number of rows and columns
    size_t n, m;
    std::cin >> n >> m;
    
    // Read the number of non-zero elements
    size_t count;
    std::cin >> count;
    
    // Read the indptr array
    std::vector<size_t> indptr(n + 1);
    for (size_t i = 0; i <= n; ++i) {
        std::cin >> indptr[i];
    }
    
    // Read the indices array
    std::vector<size_t> indices(count);
    for (size_t i = 0; i < count; ++i) {
        std::cin >> indices[i];
    }
    
    // Read the data array
    std::vector<int> data(count);
    for (size_t i = 0; i < count; ++i) {
        std::cin >> data[i];
    }
    
    // Create the CSR matrix
    sjtu::CSRMatrix<int> matrix(n, m, count, indptr, indices, data);
    
    // Output the matrix in dense format
    auto dense = matrix.getMatrix();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            std::cout << dense[i][j];
            if (j < m - 1) std::cout << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
