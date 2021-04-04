#ifndef TRAJET_H_INCLUDED
#define TRAJET_H_INCLUDED

class Trajet
{
private:
    int m_depart;       //Point de d�part de l'arc
    int m_arrivee;      //Point d'arriv�e de l'arc
    int m_poids;        //Poids/longueur de l'arc
public:
    Trajet(int depart, int arrivee, int poids);
    ~Trajet();
    int getDepart() const;  //Getter du sommet de d�part de l'arc
    int getArrivee() const; //Getter du sommet d'arriv�e de l'arc
    int getPoids() const;   //Getter du poids de l'arc
};

#endif // TRAJET_H_INCLUDED
