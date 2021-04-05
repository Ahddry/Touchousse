#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED
#include "Trajet.h"
#include <vector>

class Point
{
private:
    int m_num;      //Numéro du point
    std::vector<Trajet> m_antecedant;//arcs reliant le point à ses antécédants
    std::vector<Trajet> m_suivant;//arcs reliant le point à ses suivants
    std::string m_lieu; //Nom du point
    int m_altitude; //Altitude du point
    int m_couleur;  //couleur attribuée au point

    int m_X;        //Coordonée X du point pour l'affichage graphique
    int m_Y;        //Coordonée Y du point pour l'affichage graphique

    double m_distance; //Distance du point de départ
    int m_anteDijk; //antécédant par recherche du chemin le plus court
    int m_anteBfs;  //antécédant par recherche des chemins BFS
public:
    Point(int num, std::string nom, int alt, int x, int y);
    ~Point();
    int getNum() const;         //Getter du numéro du point
    std::string getLieu() const;        //Getter du nom du point
    int getAlt() const;         //Getter de l'altitude du point
    int x() const;              //Getter pour la coordonée X du point pour l'affichage graphique
    int y() const;              //Getter pour la coordonée Y du point pour l'affichage graphique
    void ajoutAnte(Trajet ante);//Ajout d'un arc comme antécédant
    void ajoutSuiv(Trajet suiv);//Ajout d'un arc comme suivant
    std::vector<Trajet> getAnte() const;//Récupération de tous les arcs précédentes
    std::vector<Trajet> getSuiv() const;//Récupération de tous les arcs suivantes
    void setCouleur(int coul);  //Attribution d'une couleur
    int getCouleur() const;     //Récupération de la couleur actuelle
    void setDistance(double dist); //Attribution d'une distance au point initial
    double getDistance() const;    //Récupération de la distance au point initial
    void setDijk(int dijk);     //Attribution d'un antécédent par recherche du chemin le plus court
    int getDijk() const;        //Récupération de l'antécédent par recherche du chemin le plus court
    void setBfs(int bfs);       //Attribution d'un antécédent par recherche des chemins BFS
    int getBfs() const;         //Récupération de l'antécédent par recherche des chemins BFS
};

#endif // POINT_H_INCLUDED
