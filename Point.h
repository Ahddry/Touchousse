#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED
#include "Trajet.h"
#include <vector>

class Point
{
private:
    int m_num;      //Num�ro du point
    std::vector<Trajet> m_antecedant;//arcs reliant le point � ses ant�c�dants
    std::vector<Trajet> m_suivant;//arcs reliant le point � ses suivants
    std::string m_lieu; //Nom du point
    int m_altitude; //Altitude du point
    int m_couleur;  //couleur attribu�e au point

    int m_X;        //Coordon�e X du point pour l'affichage graphique
    int m_Y;        //Coordon�e Y du point pour l'affichage graphique

    double m_distance; //Distance du point de d�part
    int m_anteDijk; //ant�c�dant par recherche du chemin le plus court
    int m_anteBfs;  //ant�c�dant par recherche des chemins BFS
public:
    Point(int num, std::string nom, int alt, int x, int y);
    ~Point();
    int getNum() const;         //Getter du num�ro du point
    std::string getLieu() const;        //Getter du nom du point
    int getAlt() const;         //Getter de l'altitude du point
    int x() const;              //Getter pour la coordon�e X du point pour l'affichage graphique
    int y() const;              //Getter pour la coordon�e Y du point pour l'affichage graphique
    void ajoutAnte(Trajet ante);//Ajout d'un arc comme ant�c�dant
    void ajoutSuiv(Trajet suiv);//Ajout d'un arc comme suivant
    std::vector<Trajet> getAnte() const;//R�cup�ration de tous les arcs pr�c�dentes
    std::vector<Trajet> getSuiv() const;//R�cup�ration de tous les arcs suivantes
    void setCouleur(int coul);  //Attribution d'une couleur
    int getCouleur() const;     //R�cup�ration de la couleur actuelle
    void setDistance(double dist); //Attribution d'une distance au point initial
    double getDistance() const;    //R�cup�ration de la distance au point initial
    void setDijk(int dijk);     //Attribution d'un ant�c�dent par recherche du chemin le plus court
    int getDijk() const;        //R�cup�ration de l'ant�c�dent par recherche du chemin le plus court
    void setBfs(int bfs);       //Attribution d'un ant�c�dent par recherche des chemins BFS
    int getBfs() const;         //R�cup�ration de l'ant�c�dent par recherche des chemins BFS
};

#endif // POINT_H_INCLUDED
