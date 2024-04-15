#include "../include/QAlgorithms.h"

#include <cmath>
#include <random>
#include <iostream>
#include <bitset>

using namespace Eigen;

void quantumSearch(QComputer& qc, int target){

    std::bitset<32> binary(target);

    std::string str = binary.to_string();
    str = str.substr(str.size() - qc.getNQubits(), qc.getNQubits());

    VectorXcd tarVec(1 << qc.getNQubits());
    tarVec[0] = 0;
    tarVec[target] = 1;

    //Solving the problem
    int dim = (1 << qc.getNQubits());

    qc.actAll(&QComputer::actH, "INIT");

    MatrixXcd eps = tarVec * tarVec.transpose();
    MatrixXcd U1 = MatrixXcd::Identity(dim, dim) - 2* eps;

    MatrixXcd psi = qc.getState() * qc.getState().transpose();
    MatrixXcd U2 = MatrixXcd::Identity(dim, dim) - 2 * psi;

    qc.actGate(U1, "U1");
    qc.actGate(U2, "U2");
}