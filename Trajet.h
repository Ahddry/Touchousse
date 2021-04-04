#ifndef TRAJET_H_INCLUDED
#define TRAJET_H_INCLUDED
#include <string>

class Trajet
{
private:
    int m_numero;       //Numéro de l'arc du trajet
    std::string m_nom;  //Nom du trajet
    std::string m_type; //Type du trajet
    int m_depart;       //Point de départ de l'arc
    int m_arrivee;      //Point d'arrivée de l'arc
    double m_poids;        //Poids/durée de parcours de l'arc
    int m_altDepart;    //Altitude du point de départ
    int m_altArrivee;   //Altitude du point d'arrivée
public:
    Trajet(int num, std::string nom, std::string type, int depart, int arrivee, int altDe, int altAr);
    ~Trajet();
    int getNum() const;     //Getter du numéro de l'arc
    std::string getNom() const; //Getter du nom de l'arc
    std::string getType() const;    //Getter du type de l'arc
    int getDepart() const;  //Getter du point de départ de l'arc
    int getArrivee() const; //Getter du point d'arrivée de l'arc
    double getPoids() const;   //Getter du poids de l'arc
};

#endif // TRAJET_H_INCLUDED
