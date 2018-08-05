//
// Created by xianyubo on 2018/8/4.
#include <assert.h>
#include <iostream>
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
    DenseMatrix();
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

template <typename T> void DenseMatrix<T>::save(std::string filename, bool has_header) {

}

template <typename T> void DenseMatrix<T>::saveToBinaryFile(std::string filename) {

}

template <typename T> void DenseMatrix<T>::print() {
    if (value == NULL) return;
    for (unsigned i = 0; i < dim1; i++) std::cout << col_names[i] << "\t";
    std::cout << std::endl;
    for (unsigned i = 0; i < dim1; i++) {
        for (unsigned j = 0; j < dim2; j++) {
            std::cout << value[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}
//

