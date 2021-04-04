#include "Point.h"
#include <climits>
#include <iostream>

Point::Point(int num) : m_num(num)
{
    m_distance = INT_MAX;   //Initialisation de la distance comme �tant maximale (infinie) au point de d�part
    m_couleur = 0;          //Mise � l'�tat 0 de la couleur
    m_anteDijk = -1;        //Mise � -1 (aucun) de l'ant�c�dant
}

Point::~Point(){}

int Point::getNum() const
{
    return m_num;       //Getter du num�ro du point
}

void Point::ajoutAnte(Trajet ante)
{
    m_antecedant.push_back(ante);//Ajout d'un arc comme ant�c�dant
}

void Point::ajoutSuiv(Trajet suiv)
{
    m_suivant.push_back(suiv);//Ajout d'un arc comme suivant
}

std::vector<Trajet> Point::getAnte() const//R�cup�ration de tous les arcs pr�c�dents
{
    std::vector<Trajet> ante;
    for(const auto& elem:m_antecedant)
    {
        ante.push_back(elem);
    }
    return ante;
}

std::vector<Trajet> Point::getSuiv() const//R�cup�ration de tous les arcs suivants
{
    std::vector<Trajet> suiv;
    for(const auto& elem:m_suivant)
    {
        suiv.push_back(elem);
    }
    return suiv;
}

void Point::setCouleur(int coul)//Attribution d'une couleur
{
    m_couleur = coul;
}

int Point::getCouleur() const//R�cup�ration de la couleur actuelle
{
    return m_couleur;
}

void Point::setDistance(int dist)//Attribution d'une distance au point initial
{
    m_distance = dist;
}

int Point::getDistance() const//R�cup�ration de la distance au point initial
{
    return m_distance;
}

void Point::setDijk(int dijk)//Attribution d'un ant�c�dent par recherche du chemin le plus court
{
    m_anteDijk = dijk;
}

int Point::getDijk() const//R�cup�ration de l'ant�c�dent par recherche du chemin le plus court
{
    return m_anteDijk;
}

void Point::afficher() const//Affichage du point et de ses connexions
{
    std::cout<<"Point "<<m_num<<" : \n"<<"Antecedants : ";
    for(const auto& elem: m_antecedant) std::cout<<elem.getDepart()<<" ";
    std::cout<<"\nSuivants : ";
    for(const auto& elem: m_suivant) std::cout<<elem.getArrivee()<<" ";
    std::cout<<std::endl<<std::endl;
}
