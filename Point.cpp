#include "Point.h"
#include <climits>
#include <iostream>

Point::Point(int num, std::string nom, int alt, int x, int y) : m_num(num), m_lieu(nom), m_altitude(alt), m_X(x), m_Y(y)
{
    m_distance = INT_MAX;   //Initialisation de la distance comme étant maximale (infinie) au point de départ
    m_couleur = 0;          //Mise à l'état 0 de la couleur
    m_anteDijk = -1;        //Mise à -1 (aucun) de l'antécédant
    m_anteBfs = -1;        //Mise à -1 (aucun) de l'antécédant
    m_selectDijk = true;
}

Point::~Point(){}

int Point::getNum() const
{
    return m_num;       //Getter du numéro du point
}

std::string Point::getLieu() const
{
    return m_lieu;       //Getter du nom du point
}

int Point::getAlt() const
{
    return m_altitude;  //Getter de l'altitude du point
}

int Point::x() const              //Getter pour la coordonée X du point pour l'affichage graphique
{
    return m_X;
}

int Point::y() const              //Getter pour la coordonée Y du point pour l'affichage graphique
{
    return m_Y;
}

void Point::ajoutAnte(Trajet ante)
{
    m_antecedant.push_back(ante);//Ajout d'un arc comme antécédant
}

void Point::ajoutSuiv(Trajet suiv)
{
    m_suivant.push_back(suiv);//Ajout d'un arc comme suivant
}

std::vector<Trajet> Point::getAnte() const//Récupération de tous les arcs précédents
{
    std::vector<Trajet> ante;
    for(const auto& elem:m_antecedant)
    {
        ante.push_back(elem);
    }
    return ante;
}

std::vector<Trajet> Point::getSuiv() const//Récupération de tous les arcs suivants
{
    std::vector<Trajet> suiv;
    for(const auto& elem:m_suivant)
    {
        if(elem.getSelec())
            suiv.push_back(elem);
    }
    return suiv;
}

void Point::setCouleur(int coul)//Attribution d'une couleur
{
    m_couleur = coul;
}

int Point::getCouleur() const//Récupération de la couleur actuelle
{
    return m_couleur;
}

void Point::setDistance(double dist)//Attribution d'une distance au point initial
{
    m_distance = dist;
}

double Point::getDistance() const//Récupération de la distance au point initial
{
    return m_distance;
}

void Point::setDijk(int dijk)//Attribution d'un antécédent par recherche du chemin le plus court
{
    m_anteDijk = dijk;
}

int Point::getDijk() const//Récupération de l'antécédent par recherche du chemin le plus court
{
    return m_anteDijk;
}

void Point::setBfs(int bfs)       //Attribution d'un antécédent par recherche des chemins BFS
{
    m_anteBfs = bfs;
}

int Point::getBfs() const         //Récupération de l'antécédent par recherche des chemins BFS
{
    return m_anteBfs;
}

bool Point::getSelectDijk() const           //Récupération de l'état de selection du sommet
{
    return m_selectDijk;
}

void Point::setSelectDijk(bool selectDijk)          //Attribution de l'existence du sommet ou non
{
    m_selectDijk = selectDijk;
}
