#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "matrix.h"

template <typename T> DenseMatrix<T>::DenseMatrix() {
    dim1 = 0;
    dim2 = 0;
    value = NULL;
}

template <typename T> void DenseMatrix<T>::setSize(uint p_dim1, uint p_dim2) {
    if (p_dim1 == dim1 && p_dim2 == dim2) return;
    
    if (value != NULL) {
        delete [] value[0];
        delete [] value;
        value = NULL;
    }
    
    dim1 = p_dim1;
    dim2 = p_dim2;
    
    value = new T*[dim1];
    value[0] = new T[dim1 * dim2];
    for (unsigned i = 1; i < dim1; i++) value[i] = value[0] + i * dim2;
    
    col_names.resize(dim2);
    for (unsigned i = 0; i < dim2; i ++) col_names[i] = std::to_string(i + 1);
}

template <typename T> DenseMatrix<T>::DenseMatrix(uint p_dim1, uint p_dim2) {
    dim1 = 0;
    dim2 = 0;
    value = NULL;
    setSize(p_dim1, p_dim2);
}

template <typename T> DenseMatrix<T>::~DenseMatrix() {
    if (value != NULL) {
        delete [] value[0];
        delete [] value;
    }
}

template <typename T> void DenseMatrix<T>::assign(DenseMatrix<T> &v) {
    setSize(v.dim1, v.dim2);
    for (unsigned i = 0; i < dim1; i++) {
        for (unsigned j = 0; j < dim2; j++) {
            value[i][j] = v.value[i][j];
        }
    }
}

template <typename T> void DenseMatrix<T>::init(T v) {
    if (value != NULL) {
        for (int i = 0; i < dim1; i++) {
            for (int j = 0; j < dim2; j++) {
                value[i][j] = v;
            }
        }
    }
}

template <typename T> T DenseMatrix<T>::get(uint x, uint y) {
    assert((x < dim1 && y < dim2));
    return value[x][y];
}

template <typename T> T& DenseMatrix<T>::operator()(unsigned x, unsigned y) {
    assert((x < dim1 && y < dim2));
    return value[x][y];
}

template <typename T> T DenseMatrix<T>::operator()(unsigned x, unsigned y) const {
    assert((x < dim1 && y < dim2));
    return value[x][y];
}

template <typename T> T* DenseMatrix<T>::operator()(unsigned x) const {
    assert(x < dim1);
    return value[x];
}

template <typename T> void DenseMatrix<T>::save(std::string filename) {
    std::ofstream out(filename);
    if (!out.is_open() || value == NULL) return;
    for (unsigned i = 0; i < dim2; i++) out << col_names[i] << "\t";
    out << std::endl;
    for (unsigned i = 0; i < dim1; i++) {
        for (unsigned j = 0; j < dim2; j++) {
            out << value[i][j] << "\t";
        }
        out << std::endl;
    }
    out.close();
}

template <typename T> void DenseMatrix<T>::saveToBinaryFile(std::string filename) {
    std::ofstream out(filename, std::ios::out | std::ios::binary);
    if (!out.is_open() || value == NULL) return;
    dense_matrix_file_header dmfh = {
        .id = DENSE_VECTOR_EXPECTED_FILE_ID,
        .num_rows = dim1,
        .num_cols = dim2,
        .type_size = sizeof(T)
    };
    out.write(reinterpret_cast<const char*>(&dmfh), sizeof(dmfh));
    for (unsigned i = 0; i < dim1; i++) {
        out.write(reinterpret_cast<const char*>(value[i]), sizeof(T) * dim2);
    }
    out.close();
}

template <typename T> void DenseMatrix<T>::print() {
    if (value == NULL) return;
    for (unsigned i = 0; i < dim2; i++) std::cout << col_names[i] << "\t";
    std::cout << std::endl;
    for (unsigned i = 0; i < dim1; i++) {
        for (unsigned j = 0; j < dim2; j++) {
            std::cout << value[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

template <typename T> void DenseMatrix<T>::load(std::string filename) {
    
    std::ifstream in(filename);
    if (!in.is_open()) return;
    
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        boost::trim(line);
        lines.push_back(line);
    }
    in.close();
    int p_dim1 = 0;
    int p_dim2 = 0;
    p_dim1 = lines.size() - 1;
    if (p_dim1 < 1) return;
    
    std::vector<std::string> names;
    boost::split(names, lines[0], boost::is_any_of("\t"));
    p_dim2 = names.size();
    if (p_dim2 < 1) return;
    setSize(p_dim1, p_dim2);
    
    for (unsigned m = 0; m < p_dim1; m++) {
        std::vector<std::string> tmp;
        boost::split(tmp, lines[m + 1], boost::is_any_of("\t"));
        assert(tmp.size() == p_dim2);
        for (unsigned n = 0; n < p_dim2; n++) {
            value[m][n] = (T)std::stod(tmp[n]);
        }
    }
    
    col_names.assign(names.begin(), names.end());
}

template <typename T> void DenseMatrix<T>::loadFromBinaryFile(std::string filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (!in.is_open()) return;
    dense_matrix_file_header dmfh;
    in.read(reinterpret_cast<char*>(&dmfh), sizeof(dmfh));
    if (dmfh.id != DENSE_VECTOR_EXPECTED_FILE_ID) return;
    setSize(dmfh.num_rows, dmfh.num_cols);
    for (unsigned i = 0; i < dim1; i++) {
        in.read(reinterpret_cast<char*>(value[i]), sizeof(T) * dim2);
    }
    in.close();
}
