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
    int m_couleur;  //couleur attribuée au point
    int m_distance; //Distance du point de départ
    int m_anteDijk; //antécédant par recherche du chemin le plus court
public:
    Point(int num);
    ~Point();
    int getNum() const;//Getter du numéro du point
    void ajoutAnte(Trajet ante);//Ajout d'un arc comme antécédant
    void ajoutSuiv(Trajet suiv);//Ajout d'un arc comme suivant
    std::vector<Trajet> getAnte() const;//Récupération de tous les arcs précédentes
    std::vector<Trajet> getSuiv() const;//Récupération de tous les arcs suivantes
    void setCouleur(int coul);  //Attribution d'une couleur
    int getCouleur() const;     //Récupération de la couleur actuelle
    void setDistance(int coul); //Attribution d'une distance au point initial
    int getDistance() const;    //Récupération de la distance au point initial
    void setDijk(int Dijk);     //Attribution d'un antécédent par recherche du chemin le plus court
    int getDijk() const;        //Récupération de l'antécédent par recherche du chemin le plus court
    void afficher() const;      //Affichage du point et de ses connexions
};

#endif // POINT_H_INCLUDED
