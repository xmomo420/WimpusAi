#include "src/Agent.h"

int main(int argc, char *argv[]) {
    if (argc == 2) {
        Agent agent(argv[1]);
        agent.resoudre();
        exit(EXIT_SUCCESS);
    } else {
        cerr << "Erreur, nombre d'argument invalide\nVoir le fichier \"README.md\"\n";
        exit(EXIT_FAILURE);
    }
}
