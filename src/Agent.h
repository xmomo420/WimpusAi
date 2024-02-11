#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ostream>
#include <unistd.h>
#include <climits>
#include <cmath>
#include <cctype>
#include <unordered_map>

using namespace std;

class Agent {

    /********************************************************
    ************ Structure de données utilitaires ***********
    *********************************************************/

private:

    // Point 2D
    struct Point {
        int rangee;
        int colonne;
    };


    // Case dans le jeu Wimpus
    struct Case {
        bool visitee;
        char contenu;
    };

    /********************************************************
    ****************** Attributs d'instance *****************
    *********************************************************/

    vector<vector<Case>> carte;
    Point pointOr{};
    double score = 0;
    Point pointCourant = {0, 0};
    vector<char> chemin;
    unordered_map<int, Point> parents;

    // Constantes
    const vector<char> ACTIONS_POSSIBLES{'h', 'b', 'd', 'g'};
    const double COUT_TIR = pow(10, 50);
    const double POINTS_OR_TROUVE = pow(10, 100);

public:

    /**
     * Constructeur qui modélise la carte du jeu en se basant sur le
     * fichier texte qui la représente.
     * @param cheminFicher correspond au chemin du fichier représentant
     * la carte du jeu.
     */
    explicit Agent(const string &cheminFicher);

    /**
     *
     */
    void resoudre();

private:

    /*********************************************************
    ****** Déclarations de fonctions d'instance privées ******
    **********************************************************/

    bool outOfBounds(const Agent::Point &point) const;

    bool pointValide(const Agent::Point &point);

    Case &caseAuPoint(const Point &point);

    int calculerDistanceOr(const Agent::Point &point) const;

    vector<Agent::Case> stockerUneLigne(const string &ligne, const int &indice);

    void revenir();

    void calculerScore();

    void afficherChemin() const;

    bool AStar(Point &positionDepart);

    Point meilleurePosition(vector<Point> &frontiere);

    double calculerQualite(Point &position);

    vector<Agent::Point> casesAdjacentes(Point &point);

    void reconstruire(const Point &depart, const Point &arrivee);

    /*********************************************************
    ****** Déclarations de fonctions de classes privées ******
    **********************************************************/

    static void supprimerEspaceBlancs(string &stringAModifier);

    static char actionOpposee(const char &action);

    static bool pointMemePosition(const Point &point, const Point &autrePoint);

    static char trouverAction(const Point &positionOriginal, const Point &pointFutur);


};




