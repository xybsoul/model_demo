#include <iostream>
#include "common/matrix.tcc"
int main() {
//    std::cout << "Hello, World!" << " " << DVECTOR_EXPECTED_FILE_ID << std::endl;
    DenseMatrix<double> *m = new DenseMatrix<double>(3, 5);
//    m->init(100);
//    m->load("test.txt");
//    m->saveToBinaryFile("test.bin");
    m->loadFromBinaryFile("test.bin");
    m->print();
    return 0;
}