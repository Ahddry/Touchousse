#ifndef STATION_H_INCLUDED
#define STATION_H_INCLUDED

#include "Plan_Pistes.h"
#include "Point.h"
#include <set>

class Station
{
private:
    int m_ordre;        //Ordre (nombre de points) du graphe
    int m_taille;       //Taille (nombre d'arcs) du graphe
    std::vector<Point> m_points;  //tableaux contenant tous les points
    std::vector<Trajet> m_trajets;  //tableaux contenant tous les points
    Plan_Pistes m_plan; //Objet de l'affichage graphique
public:
    Station(Plan_Pistes p);    //Constructeur du graphe prenant le chemin du fichier txt contenant le graphe en paramètres
    ~Station();
    int getOrdre() const;           //Getter pour l'ordre du graphe
    void bfs(int depart, int arrivee);      //Algorithme du BFS pour découverte des chemins d'un point à un autre
    void dijkstra(int depart, int arrivee); //Algorithme de Dijkstra pour déterminer le plus court chemin entre deux points précisés
    void afficher() const;      //Affichage du gaphe
    void resetAttributs();
    void aretes();
    void interactif();
    void arc();

};

#endif // STATION_H_INCLUDED
