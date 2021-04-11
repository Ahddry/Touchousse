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
    std::vector<std::string> m_ininteret; //Liste des types de connexion de peu d'int�r�t pour le skieur
    Plan_Pistes m_plan; //Objet de l'affichage graphique
    bool m_testAffichage;       //Permet de savoir si l'affichage c'est correctement r�alis�
    bool m_affichageBfs;        //Permet de savoir si le BFS c'est correctement r�alis�
public:
    Station(Plan_Pistes p);    //Constructeur du graphe prenant le chemin du fichier txt contenant le graphe en param�tres
    ~Station();
    int getOrdre() const;           //Getter pour l'ordre du graphe
    bool bfs(int depart, int arrivee);      //Algorithme du BFS pour d�couverte des chemins d'un point � un autre
    void dijkstra(int depart, int arrivee); //Algorithme de Dijkstra pour d�terminer le plus court chemin entre deux points pr�cis�s
    void fordFulkerson(int depart, int arrivee);//Algorithme de Ford-Fulkerson pour d�terminer le flot horaire maximal de skieurs entre deux points
    void resetAttributs();                      //Remet � l'�tat initial tout les attributs pour pouvoir appliquer un nouvel algorithme
    void resetBfs();                            //Remet � l'�tat initial uniquement les attributs n�cessaire au BFS
    void interactif();                          //Permet de g�n�rer la carte interractive
    void arc();                                 //Permet de localiser un arc et de connaitre ces informations
    void saisieDijkstra(int& point1, int& point2);          //Saisie du point de d�part et d'arriv�e pour le dijkstra
    bool selecArc(Trajet t);
    void preselec(int presel);                  //M�thode pour les trajets pr�-d�finis
    void personnalise();                        //M�thode pour personaliser son types de trajet
    void adminPanel(bool simple);               //M�thode pour g�rer le panneau administrateur
    void lectureFichiers();                     //Ouvre et lis le fichier des points et trajets de la station
    void reecritureFichiers();                  //Modifie le fichier des points et trajets de la station
    void setAffBfs(bool aff);                   //Condition d'affichage graphique ou non du BFS

};

#endif // STATION_H_INCLUDED
