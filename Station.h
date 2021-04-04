#ifndef STATION_H_INCLUDED
#define STATION_H_INCLUDED

#include "Point.h"
#include <vector>
#include <string>
#include <set>

class Station
{
private:
    int m_ordre;        //Ordre (nombre de points) du graphe
    int m_taille;       //Taille (nombre d'arcs) du graphe
    std::vector<Point> m_points;  //tableaux contenant tous les points
public:
    Station(std::string fichier);    //Constructeur du graphe prenant le chemin du fichier txt contenant le graphe en paramètres
    ~Station();
    int getOrdre() const;           //Getter pour l'ordre du graphe
    void dijkstra(int depart, int arrivee);//Algorithme de Dijkstra pour déterminer le plus court chemin entre deux points précisés
    void afficher() const;      //Affichage du gaphe

};

#endif // STATION_H_INCLUDED
