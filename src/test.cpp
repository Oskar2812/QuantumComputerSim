#include "../include/QComputer.h"
#include "../include/QAlgorithms.h"
#include "../include/Visualiser.h"

int main() {

    QComputer qc1(3,0);

    Visualiser vis(qc1, quantumSearch);

    vis.show();

    return EXIT_SUCCESS;
}