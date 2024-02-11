#include "Agent.h"

// Constructeur
Agent::Agent(const string &cheminFichier) {
    ifstream fichier(cheminFichier); // Déclarer un flux pour l'ouverture du fichier
    if (fichier.fail()) { // Vérifier que le fichier existe
        cerr << "Erreur lors de l'ouverture du fichier \"" << cheminFichier
             << "\"." << endl;
        exit(EXIT_FAILURE);
    }
    carte = vector<vector<Case>>();
    vector<Case> rangee = vector<Case>(); // Rangée de cases dans la carte Wimpus
    string ligne; // Ligne du fichier texte
    int i = 0; // Correspond à l'indice de la ligne du fichier texte sur lequel on va itérerer
    while (!fichier.eof()) {
        getline(fichier, ligne);
        supprimerEspaceBlancs(ligne);
        if (!ligne.empty()) { // Au cas s'il y a des lignes vides dans le fichier
            rangee = stockerUneLigne(ligne, i);
            carte.push_back(rangee);
            i++;
        }
    }
    fichier.close();
    pointCourant = Point{(int)carte.size() - 1, 0}; // Indiquer la position initiale de l'agent
    carte[carte.size() - 1][0].visitee = true; // Marquer la case initiale comme visitée
    chemin = vector<char>();
}

vector<Agent::Case> Agent::stockerUneLigne(const string &ligne, const int &indice) {
    vector<Case> cases = vector<Case>();
    for (int i = 0; i < ligne.length(); ++i) {
        if (ligne[i] == 'O') // L'or est représenté par 'O' dans le fichier texte
            pointOr = Point{indice,i}; // Indiquer les coordonnées du point où se trouve l'or
        // Ajouter la case dans le vector de 'Case'.
        cases.push_back(Case{false, ligne[i]});
    }
    return cases;
}

int Agent::calculerDistanceOr(const Agent::Point &point) const {
    return abs(pointOr.rangee - point.rangee) + abs(pointOr.colonne - point.colonne);
}

void Agent::supprimerEspaceBlancs(string &stringAModifier) {
    for (int i = 0; i < stringAModifier.length(); i++) // Itérer sur le string
        if (isspace(stringAModifier[i])) // Supprimer si c'est un espace blanc
            stringAModifier.erase(i);
}

bool Agent::outOfBounds(const Agent::Point &point) const {
    return point.rangee >= carte.size() || point.colonne >= carte[0].size() ||
           point.rangee < 0 || point.colonne < 0;
}

bool Agent::pointMemePosition(const Point &point, const Point &autrePoint) {
    return point.rangee == autrePoint.rangee && point.colonne == autrePoint.colonne;
}

Agent::Case & Agent::caseAuPoint(const Point &point) {
    return carte[point.rangee][point.colonne];
}

bool Agent::pointValide(const Agent::Point &point) {
    return !outOfBounds(point)
        && caseAuPoint(point).contenu != 'P'
            && !caseAuPoint(point).visitee;
}

char Agent::actionOpposee(const char &action) {
    switch (action) {
        case 'h':
            return 'b';
        case 'b':
            return 'h';
        case 'd':
            return 'g';
        case 'g':
            return 'd';
        default:
            cerr << "Erreur actionOpposee(const char &action), action invalide" << endl;
            exit(EXIT_FAILURE);
    }
}

void Agent::revenir() {
    chemin.push_back('s'); // Saisir or
    score += POINTS_OR_TROUVE;
    for (int i = (int)chemin.size() - 2; i >= 0; --i)
        if (islower(chemin[i]))  // Pour vérifier si c'est un déplacement et non un tir
            chemin.push_back(actionOpposee(chemin[i]));
    chemin.push_back('q'); // Quitter depuis la case départ
    calculerScore(); // Calculer score final
}

void Agent::resoudre() {
    if (AStar(pointCourant)) {
        for (char &action : chemin)
            cout << action;
        cout << endl;
    } else
        cout << "Or pas trouvé\n";
}

void Agent::calculerScore() {
    for (char &action : chemin) {
        if (islower(action))
            score -= 1;
        else
            score -= COUT_TIR;
    }
}

bool Agent::AStar(Agent::Point &positionDepart) {
    vector<Point> frontiere = {positionDepart};
    Point position{};
    while (!frontiere.empty()) {
        position = meilleurePosition(frontiere);
        if (caseAuPoint(position).contenu == 'O') {
            reconstruire(positionDepart, position);
            return true;
        }
        caseAuPoint(position).visitee = true;
        for (Point &pointFils : casesAdjacentes(position))
            if (!caseAuPoint(pointFils).visitee) {
                frontiere.push_back(pointFils);
                parents[pointFils.rangee * (int)carte.size() + pointFils.colonne] = position;
            }
    }
    return false;
}

Agent::Point Agent::meilleurePosition(vector<Point> &frontiere) {
    Point meilleurPoint{};
    double estimationQualite = POINTS_OR_TROUVE; // Nombre très grand
    int indiceMeilleure = 0;
    for (int i = 0; i < frontiere.size(); i++) {
        if (calculerQualite(frontiere[i]) < estimationQualite) {
            estimationQualite = calculerQualite(frontiere[i]);
            meilleurPoint = frontiere[i];
            indiceMeilleure = i;
        }
    }
    frontiere.erase(frontiere.begin() + indiceMeilleure);
    return meilleurPoint;
}

double Agent::calculerQualite(Agent::Point &position) {
    int distanceOr = calculerDistanceOr(position);
    return caseAuPoint(position).contenu == 'W'
           ? distanceOr + 1 + COUT_TIR : distanceOr + 1;
}

vector<Agent::Point> Agent::casesAdjacentes(Agent::Point &point) {
    vector<Point> cases = {};
    Point pointAdjacent{};
    pointAdjacent = Point{point.rangee - 1, point.colonne}; // En haut
    if (pointValide(pointAdjacent))
        cases.push_back(pointAdjacent);
    pointAdjacent = Point{point.rangee + 1, point.colonne}; // En bas
    if (pointValide(pointAdjacent))
        cases.push_back(pointAdjacent);
    pointAdjacent = Point{point.rangee, point.colonne - 1}; // À gauche
    if (pointValide(pointAdjacent))
        cases.push_back(pointAdjacent);
    pointAdjacent = Point{point.rangee, point.colonne + 1}; // À droite
    if (pointValide(pointAdjacent))
        cases.push_back(pointAdjacent);
    return cases;
}

void Agent::reconstruire(const Agent::Point &depart, const Agent::Point &arrivee) {
    vector<Point> positions;
    Point positionCourrante = arrivee;
    while (!pointMemePosition(positionCourrante, depart)) {
        positions.push_back(positionCourrante);
        positionCourrante =
                parents[positionCourrante.rangee * (int)carte.size() + positionCourrante.colonne];
    }
    positions.push_back(depart);
    reverse(positions.begin(), positions.end());
    char action;
    for (int i = 0; i < positions.size() - 1; i++) {
        action = trouverAction(positions[i], positions[i + 1]);
        if (caseAuPoint(positions[i + 1]).contenu == 'W')
            chemin.push_back((char)toupper(action));
        chemin.push_back(action);
    }
    revenir();
}

char Agent::trouverAction(const Agent::Point &positionOriginal,const Agent::Point &pointFutur) {
    if (pointFutur.rangee - positionOriginal.rangee == -1) // Vers le haut
        return 'h';
    else if (pointFutur.rangee - positionOriginal.rangee == 1) // Vers le bas
        return 'b';
    else if (pointFutur.colonne - positionOriginal.colonne == 1) // Vers la droite
        return 'd';
    else
        return 'g'; // Vers la gauche
}

