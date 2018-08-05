//
// Created by xianyubo on 2018/8/4.
//

#ifndef MODEL_DEMO_MATRIX_H
#define MODEL_DEMO_MATRIX_H

#include <vector>
#include <string>
#include <fstream>
#include "../util/type.h"

const uint DVECTOR_EXPECTED_FILE_ID = 1;
const uint FMATRIX_EXPECTED_FILE_ID = 2;
const uint DMATRIX_EXPECTED_FILE_ID = 1001;

struct dense_matrix_file_header {
    uint id;
    uint type_size;
    uint num_rows;
    uint num_cols;
};

struct sparse_matrix_file_header {
    uint id;
    uint float_size;
    uint64 num_values;
    uint num_rows;
    uint num_cols;
};

template <typename T> struct sparse_entry {
    uint id;
    T value;
};

template <typename T> struct sparse_row {
    sparse_entry<T>* data;
    uint size;
};

template <typename T> class DenseMatrix {
public:
    DenseMatrix();
    ~DenseMatrix();
    DenseMatrix(uint p_dim1, uint p_dim2);
    
    void assign(DenseMatrix<T>& v);
    void init(T v);
    void setSize(uint p_dim1, uint p_dim2);
    
    T get(uint x, uint y);
    T& operator() (unsigned x, unsigned y);
    T operator() (unsigned x, unsigned y) const;
    T* operator() (unsigned x) const;
    
    void save(std::string filename, bool has_header = false);
    void saveToBinaryFile(std::string filename);
    void print();
    
    T** value;
    std::vector<std::string> col_names;
    uint dim1, dim2;
    
};

template <typename T> class DenseVector {
public:
    DenseVector();
    DenseVector(uint p_dim);
    ~DenseVector();

    void setSize(uint p_dim);
    
    T get(uint x);
    T& operator() (unsigned x);
    T operator() (unsigned x) const;
    
    void init(T v);
    void assign(T* v);
    void assign(DenseVector<T>& v);
    
    void save(std::string filename);
    void saveToBinaryFile(std::string filename);
    
    void load(std::string filename);
    void loadFromBinaryFile(std::string filename);
    
    T* value;
    uint dim;

};

class DenseVectorDouble : public DenseVector<double> {
public:
    void init_normal(double mean, double stdev);
};

class DenseMatrixDouble : public DenseMatrix<double> {
public:
    void init(double mean, double stdev);
    void init_column(double mean, double stdev, int column);
};

template <typename T> class LargeSparseMatrix {
public:
    virtual void begin() = 0;
    virtual bool end() = 0;
    virtual void next() = 0;
    virtual sparse_row<T>& getRow() = 0;
    virtual uint getRowIndex() = 0;
    virtual uint getNumRows() = 0;
    virtual uint getNumCols() = 0;
    virtual uint64 getNumValues() = 0;
    
    void saveToBinaryFile(std::string filename);
    
    void saveToTextFile(std::string filename);
    
};

template <typename T> class LargeSparseMatrixHD : public LargeSparseMatrix<T> {
public:
    LargeSparseMatrixHD(std::string filename, uint64 cache_size);
    
    virtual uint getNumRow();
    virtual uint getNumCols();
    virtual uint64 getNumValues();
    
    virtual void next();
    virtual void begin();
    virtual bool end();
    
    virtual sparse_row<T>& getRow();
    virtual uint getRowIndex();

protected:
    void readCache();
    
    DenseVector<sparse_row<T>> data;
    DenseVector<sparse_entry<T>> cache;
    std::string filename;
    
    std::ifstream in;
    
    uint64 position_in_data_cache;
    uint number_of_valid_rows_in_cache;
    uint64 number_of_valid_entries_in_cache;
    
    uint num_cols;
    uint64 num_values;
    uint num_rows;

};

template <typename T> class LargeSparseMatrixMemory : public LargeSparseMatrix<T> {
public:
    virtual void begin();
    virtual bool end();
    virtual void next();
    virtual sparse_row<T>& getRow();
    virtual uint getRowIndex();
    virtual uint getNumRows();
    virtual uint getNumCols();
    virtual uint64 getNumValues();
    
    DenseVector<sparse_entry<T>> data;
    uint num_cols;
    uint64 num_values;

protected:
    uint index;
};

#endif //MODEL_DEMO_MATRIX_H
