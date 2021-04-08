#ifndef TRAJET_H_INCLUDED
#define TRAJET_H_INCLUDED
#include <string>

class Trajet
{
private:
    int m_numero;       //Num�ro de l'arc du trajet
    std::string m_nom;  //Nom du trajet
    std::string m_type; //Type du trajet
    int m_depart;       //Point de d�part de l'arc
    int m_arrivee;      //Point d'arriv�e de l'arc
    double m_poids;         //Poids/dur�e de parcours de l'arc
    double m_interet;         //Poids/dur�e de parcours de l'arc * une constante d'inint�ressement
    int m_flux;          //Flux circulant actuelement sur l'arc
    int m_capacite;          //Capacit� horaire de skieurs sur l'arc
    int m_altDepart;    //Altitude du point de d�part
    int m_altArrivee;   //Altitude du point d'arriv�e
    bool m_selectionable;    //Choix de si l'arc est selectionnable ou non vis-�-vis des param�tres actuels de l'utilisateur
    bool m_active;         //Choix de si l'arc est interdit � l'ouverture du fichier

public:
    Trajet(int num, std::string nom, std::string type, int depart, int arrivee, int altDe, int altAr);
    ~Trajet();
    int getNum() const;     //Getter du num�ro de l'arc
    std::string getNom() const; //Getter du nom de l'arc
    std::string getType() const;    //Getter du type de l'arc
    int getDepart() const;  //Getter du point de d�part de l'arc
    int getArrivee() const; //Getter du point d'arriv�e de l'arc
    double getPoids() const;   //Getter du poids de l'arc
    void setInteret(bool interet);   //setter de l'int�ret de l'arc
    double getInteret() const;   //Getter de l'int�ret de l'arc
    void setSelec(bool selection);  //Setter du crit�re de selection
    bool getSelec() const;      //Getter du crit�re de selection
    void setActive(bool active);  //Setter du crit�re de bannissement
    bool getActive() const;      //Getter du crit�re de bannissement
    int getFlux() const;
    void setFlux(int flux);
    int getCapacite() const;

};

#endif // TRAJET_H_INCLUDED
