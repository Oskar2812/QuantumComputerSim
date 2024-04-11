#include "../include/QComputer.h"

#include <iostream>
#include <bitset>
#include <string>

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

    std::cout << "Number of qubits: " << nQubits << std::endl;
    for( int ii = 0; ii < state.size(); ii++){
        std::bitset<32> binary(ii);

        std::string str = binary.to_string();
        str = str.substr(str.size() - nQubits, nQubits);
        std::cout << state[ii] << "      |" << str << ">\n"; 
    }
}
void QComputer::act(Matrix2d gate, int index){
    if(index < 0 || nQubits <= index){
        throw std::invalid_argument("Invalid qubit index: Index must be between 0 and " + std::to_string(nQubits - 1));
    }

    MatrixXd result = MatrixXd::Identity(1,1);

    for(int ii = 0; ii < nQubits; ii++){
        if(ii == index){
            result = kroneckerProduct(result, gate).eval();
        } else {
            result = kroneckerProduct(result, MatrixXd::Identity(2,2)).eval();
        }
    }

    state = result * state;
}

void QComputer::actX(int nn){

    Matrix2d X;
    X << 0, 1,
         1, 0;      

    act(X, nn);
    
}