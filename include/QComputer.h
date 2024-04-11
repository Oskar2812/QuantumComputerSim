/*
Defining the Quantum computer class

Author: Oskar Grewe

*/

#include <Eigen/Dense>

#ifndef QCOMPUTER
#define QCOMPUTER

/// @brief Quantum computer containing the state and (temporarily) the logic gates to act on them 
class QComputer{
    public:

        /// @brief Initilaises the quantum computer
        /// @param nQ number of qubits
        /// @param initState initial state of the qubits (0 or 1)
        QComputer(int nQ, int initState);

        /// @brief nQubits getter function
        /// @return nQubits
        int getNQubits() const; 

        /// @brief Prints the current state vector
        void showState() const;

    private:
        /// @brief Number of qubits in state, TODO: Add a register class and stick it inside there
        int nQubits;
        /// @brief Current sate of the computer
        Eigen::VectorXd state;

        /// @brief Initialises a qubit
        /// @return A 2d vector representing the qubit
        Eigen::Vector2d initQubit(int initState) const;


};

#endif