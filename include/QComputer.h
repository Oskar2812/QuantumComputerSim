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
        void showState();

        /// @brief Acts a X gate on the idex'th qubit 
        /// @param qubit The qubit to act on
        void actX(int index);

        /// @brief Acts a Y gate on the idex'th qubit 
        /// @param qubit The qubit to act on
        void actY(int index);

        /// @brief Acts a Z gate on the idex'th qubit 
        /// @param qubit The qubit to act on
        void actZ(int index);

        /// @brief Acts a Hadamard gate on the idex'th qubit 
        /// @param qubit The qubit to act on
        void actH(int index);

        /// @brief Acts a certain gate on all qubits
        /// @param actGate the gate to act
        void actAll(void (QComputer::*actGate)(int));

        /// @brief Measures the state 
        /// @return The index of the measured state with probability |state[index]|^2
        int measure();

        /// @brief Measures and prints it in base-10 and binary
        void showMeasure();

    private:
        /// @brief Number of qubits in state, TODO: Add a register class and stick it inside there
        int nQubits;
        /// @brief Current sate of the computer
        Eigen::VectorXcd state;

        /// @brief Initialises a qubit
        /// @return A 2d vector representing the qubit
        Eigen::Vector2cd initQubit(int initState) const;

        /// @brief The method actually building the matrix that cts on the final state
        /// @param gate The matrix to be used
        void act(Eigen::Matrix2cd gate, int index);

        /// @brief Normalises the state vector
        void normaliseState();


};

#endif