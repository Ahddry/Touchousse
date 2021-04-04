#include "Trajet.h"


Trajet::Trajet(int depart, int arrivee, int poids) : m_depart(depart), m_arrivee(arrivee), m_poids(poids)
{

}


Trajet::~Trajet(){}

int Trajet::getArrivee() const
{
    return m_arrivee;//Getter du point d'arrivée de l'arc
}
int Trajet::getDepart() const
{
    return m_depart;//Getter du point de départ de l'arc
}
int Trajet::getPoids() const
{
    return m_poids;//Getter du poids de l'arc
}


