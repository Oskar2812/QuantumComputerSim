#include "QComputer.h"
#include "QAlgorithms.h"

#ifndef VISUALISER_H
#define VISUALISER_H

class Visualiser{
    public:
        typedef void (*QAlgo)(QComputer&, int);

        /// @brief Initilaises a Visualiser object to create an interactive representaton of the simulated quantum computer
        /// @param qc The computer to visualise
        Visualiser(QComputer& qc, QAlgo alg);

        /// @brief Displayes the interactive representation of the quantum computer
        void show();

    private:
        QComputer& qComp;
        QAlgo algo;

};

#endif