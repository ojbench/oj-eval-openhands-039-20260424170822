
#ifndef CSR_MATRIX_HPP
#define CSR_MATRIX_HPP

#include <vector>
#include <exception>

namespace sjtu {

class size_mismatch : public std::exception {
public:
    const char *what() const noexcept override {
        return "Size mismatch";
    }
};

class invalid_index : public std::exception {
public:
    const char *what() const noexcept override {
        return "Index out of range";
    }
};

// TODO: Implement a CSR matrix class
// You only need to implement the TODOs in this file
// DO NOT modify other parts of this file
// DO NOT include any additional headers
// DO NOT use STL other than std::vector

template <typename T>
class CSRMatrix {

private:
    size_t n_rows;        // number of rows
    size_t n_cols;        // number of columns
    std::vector<size_t> indptr;   // row pointer array
    std::vector<size_t> indices;  // column indices array
    std::vector<T> data;          // non-zero values array

public:
    // Assignment operators are deleted
    CSRMatrix &operator=(const CSRMatrix &other) = delete;
    CSRMatrix &operator=(CSRMatrix &&other) = delete;

    // Constructor for empty matrix with dimensions
    // TODO: Initialize an empty CSR matrix with n rows and m columns
    CSRMatrix(size_t n, size_t m) : n_rows(n), n_cols(m), indptr(n + 1, 0), indices(), data() {
        // Initialize indptr with zeros
        for (size_t i = 0; i <= n; ++i) {
            indptr[i] = 0;
        }
    }

    // Constructor with pre-built CSR components
    // TODO: Initialize CSR matrix from existing CSR format data, validate sizes
    CSRMatrix(size_t n, size_t m, size_t count,
        const std::vector<size_t> &indptr, 
        const std::vector<size_t> &indices,
        const std::vector<T> &data) : n_rows(n), n_cols(m), indptr(indptr), indices(indices), data(data) {
        
        // Validate dimensions
        if (indptr.size() != n + 1) {
            throw size_mismatch();
        }
        
        if (indices.size() != count || data.size() != count) {
            throw size_mismatch();
        }
        
        // Validate indptr values
        if (indptr[0] != 0) {
            throw size_mismatch();
        }
        
        for (size_t i = 0; i < n; ++i) {
            if (indptr[i] > indptr[i + 1]) {
                throw size_mismatch();
            }
        }
        
        // Validate column indices
        for (size_t i = 0; i < indices.size(); ++i) {
            if (indices[i] >= m) {
                throw invalid_index();
            }
        }
    }

    // Copy constructor
    CSRMatrix(const CSRMatrix &other) = default;

    // Move constructor
    CSRMatrix(CSRMatrix &&other) = default;

    // Constructor from dense matrix format (given as vector of vectors)
    // TODO: Convert dense matrix representation to CSR format
    CSRMatrix(size_t n, size_t m, const std::vector<std::vector<T>> &dense_data) : n_rows(n), n_cols(m), indptr(n + 1, 0), indices(), data() {
        // First pass: count non-zero elements in each row
        for (size_t i = 0; i < n; ++i) {
            if (dense_data[i].size() != m) {
                throw size_mismatch();
            }
            size_t count = 0;
            for (size_t j = 0; j < m; ++j) {
                if (dense_data[i][j] != T{}) {
                    count++;
                }
            }
            indptr[i + 1] = indptr[i] + count;
        }
        
        // Resize indices and data vectors
        indices.resize(indptr[n]);
        data.resize(indptr[n]);
        
        // Second pass: fill indices and data
        size_t idx = 0;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                if (dense_data[i][j] != T{}) {
                    indices[idx] = j;
                    data[idx] = dense_data[i][j];
                    idx++;
                }
            }
        }
    }

    // Destructor
    ~CSRMatrix() = default;

    // Get dimensions and non-zero count
    // TODO: Return the number of rows
    size_t getRowSize() const {
        return n_rows;
    }

    // TODO: Return the number of columns
    size_t getColSize() const {
        return n_cols;
    }

    // TODO: Return the count of non-zero elements
    size_t getNonZeroCount() const {
        return data.size();
    }

    // Element access
    // TODO: Retrieve element at position (i,j)
    T get(size_t i, size_t j) const {
        // Check bounds
        if (i >= n_rows || j >= n_cols) {
            throw invalid_index();
        }
        
        // Find the element in the i-th row
        size_t start = indptr[i];
        size_t end = indptr[i + 1];
        
        for (size_t k = start; k < end; ++k) {
            if (indices[k] == j) {
                return data[k];
            }
        }
        
        // If not found, return default-constructed T
        return T{};
    }

    // TODO: Set element at position (i,j), updating CSR structure as needed
    void set(size_t i, size_t j, const T &value) {
        // Check bounds
        if (i >= n_rows || j >= n_cols) {
            throw invalid_index();
        }
        
        // Find the element in the i-th row
        size_t start = indptr[i];
        size_t end = indptr[i + 1];
        
        // Search for existing element
        for (size_t k = start; k < end; ++k) {
            if (indices[k] == j) {
                // Update existing element
                data[k] = value;
                return;
            }
        }
        
        // Element not found, need to insert
        // Find insertion position to keep indices sorted
        size_t pos = start;
        while (pos < end && indices[pos] < j) {
            pos++;
        }
        
        // Insert at position pos
        indices.insert(indices.begin() + pos, j);
        data.insert(data.begin() + pos, value);
        
        // Update indptr for all subsequent rows
        for (size_t r = i + 1; r <= n_rows; ++r) {
            indptr[r]++;
        }
    }

    // Access CSR components
    // TODO: Return the row pointer array
    const std::vector<size_t> &getIndptr() const {
        return indptr;
    }

    // TODO: Return the column indices array
    const std::vector<size_t> &getIndices() const {
        return indices;
    }

    // TODO: Return the data values array
    const std::vector<T> &getData() const {
        return data;
    }

    // Convert to dense matrix format
    // TODO: Convert CSR format to dense matrix representation
    std::vector<std::vector<T>> getMatrix() const {
        std::vector<std::vector<T>> result(n_rows, std::vector<T>(n_cols, T{}));
        
        for (size_t i = 0; i < n_rows; ++i) {
            size_t start = indptr[i];
            size_t end = indptr[i + 1];
            
            for (size_t k = start; k < end; ++k) {
                result[i][indices[k]] = data[k];
            }
        }
        
        return result;
    }

    // Matrix-vector multiplication
    // TODO: Implement multiplication of this matrix with vector vec
    std::vector<T> operator*(const std::vector<T> &vec) const {
        // Check dimensions
        if (vec.size() != n_cols) {
            throw size_mismatch();
        }
        
        std::vector<T> result(n_rows, T{});
        
        for (size_t i = 0; i < n_rows; ++i) {
            size_t start = indptr[i];
            size_t end = indptr[i + 1];
            
            for (size_t k = start; k < end; ++k) {
                result[i] += data[k] * vec[indices[k]];
            }
        }
        
        return result;
    }

    // Row slicing
    // TODO: Extract submatrix containing rows [l,r)
    CSRMatrix getRowSlice(size_t l, size_t r) const {
        // Check bounds
        if (l >= r || r > n_rows) {
            throw invalid_index();
        }
        
        // Calculate number of non-zero elements in the slice
        size_t count = indptr[r] - indptr[l];
        
        // Create new indptr array for the slice
        std::vector<size_t> new_indptr(r - l + 1);
        for (size_t i = 0; i <= r - l; ++i) {
            new_indptr[i] = indptr[l + i] - indptr[l];
        }
        
        // Create new indices and data arrays
        std::vector<size_t> new_indices(count);
        std::vector<T> new_data(count);
        
        for (size_t i = 0; i < count; ++i) {
            new_indices[i] = indices[indptr[l] + i];
            new_data[i] = data[indptr[l] + i];
        }
        
        return CSRMatrix(r - l, n_cols, count, new_indptr, new_indices, new_data);
    }
};

}

#endif // CSR_MATRIX_HPP
