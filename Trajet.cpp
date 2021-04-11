#include "Trajet.h"

Trajet::Trajet(int num, std::string nom, std::string type, int depart, int arrivee, int altDe, int altAr)
: m_numero(num), m_nom(nom), m_type(type), m_depart(depart), m_arrivee(arrivee), m_altDepart(altDe), m_altArrivee(altAr)
{
    ///Calcul du temps de trajet :
    double delta = m_altArrivee - m_altDepart;              //On rentre en dur le poids et la capacité des différents trajets pour appliquer les algorithmes
    if(type=="V")                                           //Piste verte
    {
        m_poids = 5 * (delta/100);
        m_capacite = 1950;
    }
    else if(type=="B")                                      //Piste bleue
    {
        m_poids = 4 * (delta/100);
        m_capacite = 2100;
    }
    else if(type=="R")                                      //Piste rouge
    {
        m_poids = 3 * (delta/100);
        m_capacite = 1800;
    }
    else if(type=="N")                                      //Piste noire
    {
        m_poids = 2 * (delta/100);
        m_capacite = 1200;
    }
    else if(type=="KL")                                     //Piste de kilomètre lancé
    {
        m_poids = (delta/600);
        m_capacite = 600;
    }
    else if(type=="SURF")                                   //Piste Surf
    {
        m_poids = 10 * (delta/100);
        m_capacite = 975;
    }

    else if(type=="TPH")                                    //Téléphérique
    {
        m_poids = 4 + 2 * (delta/100);
        m_capacite = 1200;
    }
    else if(type=="TC")                                     //Télécabine
    {
        m_poids = 2 + 3 * (delta/100);
        m_capacite = 2200;
    }
    else if(type=="TSD")                                    //Télésiège débrayable
    {
        m_poids = 1 + 3 * (delta/100);
        m_capacite = 2500;
    }
    else if(type=="TS")                                     //Télésiège
    {
        m_poids = 1 + 4 * (delta/100);
        m_capacite = 1800;
    }
    else if(type=="TK")                                     //Téléski
    {
        m_poids = 1 + 4 * (delta/100);
        m_capacite = 800;
    }
    else if(type=="BUS")                                    //Bus
    {
        if(m_depart == 7 && m_arrivee == 30) m_poids = 40;
        else if(m_depart == 30 && m_arrivee == 7) m_poids = 40;
        else if(m_depart == 30 && m_arrivee == 36) m_poids = 30;
        else if(m_depart == 36 && m_arrivee == 30) m_poids = 30;
        m_capacite = 300;
    }
    if(m_poids < 0) m_poids *= -1;
    m_selectionable = true;
    m_interet = m_poids;
    m_flux = 0;
}


Trajet::~Trajet(){}

int Trajet::getNum() const          //Getter du numéro de l'arc
{
    return m_numero;
}

std::string Trajet::getNom() const          //Getter du nom de l'arc
{
    return m_nom;
}

std::string Trajet::getType() const         //Getter du type de l'arc
{
    return m_type;
}

int Trajet::getArrivee() const
{
    return m_arrivee;//Getter du point d'arrivée de l'arc
}
int Trajet::getDepart() const
{
    return m_depart;//Getter du point de départ de l'arc
}
double Trajet::getPoids() const
{
    return m_poids;//Getter du poids de l'arc
}


void Trajet::setInteret(bool interet)   //setter de l'intéret de l'arc
{
    if(interet==true)
    {
        m_interet = m_poids;
    }
    else
    {
        m_interet = m_poids*150;
    }
}

double Trajet::getInteret() const   //Getter de l'intéret de l'arc
{
    return m_interet;
}

void Trajet::setSelec(bool selection)  //Setter du critère de selection
{
    m_selectionable = selection;
}
bool Trajet::getSelec() const      //Getter du critère de selection
{
    return m_selectionable;
}
void Trajet::setActive(bool active)  //Setter du critère de bannissement
{
    m_active = active;
}
bool Trajet::getActive() const      //Getter du critère de bannissement
{
    return m_active;
}

int Trajet::getFlux() const         //Getter du flux
{
    return m_flux;
}

void Trajet::setFlux(int flux)          //Setter du flux
{
    m_flux = flux;
}
int Trajet::getCapacite() const         //Getter de la capacité
{
    return m_capacite;
}
