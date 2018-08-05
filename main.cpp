#include <iostream>
#include "common/matrix.h"
int main() {
//    std::cout << "Hello, World!" << " " << DVECTOR_EXPECTED_FILE_ID << std::endl;
    DenseMatrix<int> *m = new DenseMatrix<int>(3, 5);
    m->init(100);
    m->print();
    return 0;
}