#include "../include/QComputer.h"

#include <iostream>
#include <bitset>
#include <random>
#include <complex>
#include <cmath>

//Eigen includes
#include <unsupported/Eigen/KroneckerProduct>

using namespace Eigen;

int QComputer::getNQubits() const{
    return nQubits;
}

Vector2cd QComputer::initQubit(int initState) const{
    if(initState < 0 || initState > 1){
        throw std::invalid_argument("Error: State must be 0 or 1");
    }

    if(initState == 0){
        Vector2cd v(1,0);
        return v;
    } else {
        Vector2cd v(0,1);
        return v;
    }
}

QComputer::QComputer(int nQ, int initState) : nQubits(nQ){
    state.resize(1 << nQubits);

    VectorXcd temp;
    temp = initQubit(initState);

    for(int ii = 1; ii < nQubits; ii++){
        temp = kroneckerProduct(temp, initQubit(initState)).eval();
    }

    state = temp;
}

void QComputer::showState(){
    normaliseState();

    std::cout << "Number of qubits: " << nQubits << std::endl;
    for( int ii = 0; ii < state.size(); ii++){
        std::bitset<32> binary(ii);

        std::string str = binary.to_string();
        str = str.substr(str.size() - nQubits, nQubits);
        std::cout << state[ii] << "      |" << str << ">\n"; 
    }
}
void QComputer::act(Matrix2cd gate, int index, std::string label, std::string tag){

    if(tag == "INIT" && isInit == false){
        initialise();
    } else if(tag == "INIT" && isInit == true){
        return;
    }
    if(index < 0 || nQubits <= index){
        throw std::invalid_argument("Invalid qubit index: Index must be between 0 and " + std::to_string(nQubits - 1));
    }

    MatrixXcd result = MatrixXcd::Identity(1,1);

    for(int ii = 0; ii < nQubits; ii++){
        if(ii == index){
            result = kroneckerProduct(result, gate).eval();
        } else {
            result = kroneckerProduct(result, MatrixXcd::Identity(2,2)).eval();
        }
    }

    state = result * state;
}

void QComputer::normaliseState(){
    state.normalize();
}

int QComputer::measure(){
    state.normalize();

    std::random_device rd;  // Non-deterministic random number generator
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_real_distribution<> dis(0.0, 1.0);

    VectorXd probabilities = state.cwiseAbs2();

    VectorXd cumilative(state.size());
    cumilative[0] = probabilities[0];
    for(int ii = 1 ; ii < state.size(); ii++){
        cumilative[ii] = probabilities[ii] + cumilative[ii - 1];
    } 

    double randomVal = dis(gen);

    for(int ii = 0; ii < cumilative.size(); ii++){
        if(randomVal <= cumilative[ii]){
            return ii;
        }
    }

    return cumilative.size() - 1;
}

void QComputer::showMeasure(){
    int result = measure();

    std::bitset<32> binary(result);

    std::string str = binary.to_string();
    str = str.substr(str.size() - nQubits, nQubits);

    std::cout << "Measurement result: " << result << "  <=>  |" << str << ">" << std::endl;
}

VectorXcd QComputer::getState(){
    return state;
}

void QComputer::initialise(){
    isInit = true;
}

/*GATES*/

void QComputer::actX(int nn, std::string tag){
    Matrix2cd X;
    X << 0, 1,
         1, 0;      

    act(X, nn, "X", tag);   
}

void QComputer::actY(int nn, std::string tag){
    Matrix2cd Y;
    Y << std::complex<double>(0,0), std::complex<double>(0,-1), 
         std::complex<double>(0,1), std::complex<double>(0,0);      

    act(Y, nn, "Y", tag);   
}

void QComputer::actZ(int nn, std::string tag){
    Matrix2cd Z;
    Z << 1, 0,
         0, -1;      

    act(Z, nn, "Z", tag);   
}

void QComputer::actH(int nn, std::string tag){
    Matrix2cd H;
    H << 1, 1,
         1, -1;      

    H = (1/sqrt(2)) * H;
    act(H, nn, "H", tag);   
}

void QComputer::actAll(void (QComputer::*actGate)(int, std::string), std::string tag){
    if(tag == "INIT" && isInit == true){
        return;
    }else if(tag == "INIT" && isInit == false){
        initialise();
        std::cout << "Here" << std::endl;
    }

    for(int ii = 0; ii < nQubits; ii++){
        (this->*actGate)(ii, "STANDARD");
    }
}

void QComputer::actGate(MatrixXcd gate, std::string label, std::string tag){
    if(gate.rows() != (1 << nQubits) || gate.cols() != (1 << nQubits)){
        throw std::invalid_argument("Error: Gate must have dimensions: " + (1 << nQubits));
    }

    state = gate * state;
}