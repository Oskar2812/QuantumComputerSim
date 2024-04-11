#include "../include/QComputer.h"

#include <iostream>

//Eigen includes
#include <unsupported/Eigen/KroneckerProduct>

using namespace Eigen;

int QComputer::getNQubits() const{
    return nQubits;
}

Vector2d QComputer::initQubit(int initState) const{
    if(initState < 0 || initState > 1){
        throw std::invalid_argument("Error: State must be 0 or 1");
    }

    if(initState == 0){
        Vector2d v(1,0);
        return v;
    } else {
        Vector2d v(0,1);
        return v;
    }
}

QComputer::QComputer(int nQ, int initState) : nQubits(nQ){
    state.resize(1 << nQubits);

    VectorXd temp;
    temp = initQubit(initState);

    for(int ii = 1; ii < nQubits; ii++){
        temp = kroneckerProduct(temp, initQubit(initState)).eval();
    }

    state = temp;
}

void QComputer::showState() const{
    std::cout << "Number of qubits: " << nQubits << "\n" << state << std::endl;
}