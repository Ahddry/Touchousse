#include "Trajet.h"

Trajet::Trajet(int num, std::string nom, std::string type, int depart, int arrivee, int altDe, int altAr)
: m_numero(num), m_nom(nom), m_type(type), m_depart(depart), m_arrivee(arrivee), m_altDepart(altDe), m_altArrivee(altAr)
{
    ///Calcul du temps de trajet :
    double delta = m_altArrivee - m_altDepart;
    if(type=="V") m_poids = 5 * (delta/100);
    else if(type=="B") m_poids = 4 * (delta/100);
    else if(type=="R") m_poids = 3 * (delta/100);
    else if(type=="N") m_poids = 2 * (delta/100);
    else if(type=="KL") m_poids = (delta/600);
    else if(type=="SURF") m_poids = 10 * (delta/100);

    else if(type=="TPH") m_poids = 4 + 2 * (delta/100);
    else if(type=="TC") m_poids = 2 + 3 * (delta/100);
    else if(type=="TSD") m_poids = 1 + 3 * (delta/100);
    else if(type=="TS") m_poids = 1 + 4 * (delta/100);
    else if(type=="TK") m_poids = 1 + 4 * (delta/100);
    else if(type=="BUS")
    {
        if(m_depart == 7 && m_arrivee == 30) m_poids = 40;
        else if(m_depart == 30 && m_arrivee == 7) m_poids = 40;
        else if(m_depart == 30 && m_arrivee == 36) m_poids = 30;
        else if(m_depart == 36 && m_arrivee == 30) m_poids = 30;
    }
    if(m_poids < 0) m_poids *= -1;
    m_selectionable = true;
}


Trajet::~Trajet(){}

int Trajet::getNum() const
{
    return m_numero;
}

std::string Trajet::getNom() const
{
    return m_nom;
}

std::string Trajet::getType() const
{
    return m_type;
}

int Trajet::getArrivee() const
{
    return m_arrivee;//Getter du point d'arriv�e de l'arc
}
int Trajet::getDepart() const
{
    return m_depart;//Getter du point de d�part de l'arc
}
double Trajet::getPoids() const
{
    return m_poids;//Getter du poids de l'arc
}

void Trajet::setSelec(bool selection)  //Setter du crit�re de selection
{
    m_selectionable = selection;
}
bool Trajet::getSelec() const      //Getter du crit�re de selection
{
    return m_selectionable;
}
