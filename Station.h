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
    std::vector<std::string> m_ininteret; //Liste des types de connexion de peu d'intérêt pour le skieur
    Plan_Pistes m_plan; //Objet de l'affichage graphique
    bool m_testAffichage;
public:
    Station(Plan_Pistes p);    //Constructeur du graphe prenant le chemin du fichier txt contenant le graphe en paramètres
    ~Station();
    int getOrdre() const;           //Getter pour l'ordre du graphe
    bool bfs(int depart, int arrivee);      //Algorithme du BFS pour découverte des chemins d'un point à un autre
    void dijkstra(int depart, int arrivee); //Algorithme de Dijkstra pour déterminer le plus court chemin entre deux points précisés
    void fordFulkerson(int depart, int arrivee);//Algorithme de Ford-Fulkerson pour déterminer le flot horaire maximal de skieurs entre deux points
    void resetAttributs();
    void resetBfs();
    void interactif();
    void arc();
    void saisieDijkstra(int& point1, int& point2);
    bool selecArc(Trajet t);
    void preselec(int presel);
    void personnalise();
    void adminPanel(bool simple);
    void lectureFichiers();
    void reecritureFichiers();

};

#endif // STATION_H_INCLUDED
