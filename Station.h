#ifndef STATION_H_INCLUDED
#define STATION_H_INCLUDED

#include "Point.h"
#include <set>

class Station
{
private:
    int m_ordre;        //Ordre (nombre de points) du graphe
    int m_taille;       //Taille (nombre d'arcs) du graphe
    std::vector<Point> m_points;  //tableaux contenant tous les points
public:
    Station(std::string fichier);    //Constructeur du graphe prenant le chemin du fichier txt contenant le graphe en param�tres
    ~Station();
    int getOrdre() const;           //Getter pour l'ordre du graphe
    void bfs(int depart, int arrivee);      //Algorithme du BFS pour d�couverte des chemins d'un point � un autre
    void dijkstra(int depart, int arrivee); //Algorithme de Dijkstra pour d�terminer le plus court chemin entre deux points pr�cis�s
    void afficher() const;      //Affichage du gaphe
    void resetAttributs();
    void aretes();

};

#endif // STATION_H_INCLUDED
