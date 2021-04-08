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
    double m_poids;         //Poids/durée de parcours de l'arc
    double m_interet;         //Poids/durée de parcours de l'arc * une constante d'inintéressement
    int m_flux;          //Flux circulant actuelement sur l'arc
    int m_capacite;          //Capacité horaire de skieurs sur l'arc
    int m_altDepart;    //Altitude du point de départ
    int m_altArrivee;   //Altitude du point d'arrivée
    bool m_selectionable;    //Choix de si l'arc est selectionnable ou non vis-à-vis des paramètres actuels de l'utilisateur
    bool m_active;         //Choix de si l'arc est interdit à l'ouverture du fichier

public:
    Trajet(int num, std::string nom, std::string type, int depart, int arrivee, int altDe, int altAr);
    ~Trajet();
    int getNum() const;     //Getter du numéro de l'arc
    std::string getNom() const; //Getter du nom de l'arc
    std::string getType() const;    //Getter du type de l'arc
    int getDepart() const;  //Getter du point de départ de l'arc
    int getArrivee() const; //Getter du point d'arrivée de l'arc
    double getPoids() const;   //Getter du poids de l'arc
    void setInteret(bool interet);   //setter de l'intéret de l'arc
    double getInteret() const;   //Getter de l'intéret de l'arc
    void setSelec(bool selection);  //Setter du critère de selection
    bool getSelec() const;      //Getter du critère de selection
    void setActive(bool active);  //Setter du critère de bannissement
    bool getActive() const;      //Getter du critère de bannissement
    int getFlux() const;
    void setFlux(int flux);
    int getCapacite() const;

};

#endif // TRAJET_H_INCLUDED
