#ifndef ADMINISTRATEURS_H_INCLUDED
#define ADMINISTRATEURS_H_INCLUDED

#include "Plan_Pistes.h"
#include <vector>

class Administrateurs
{
private:
    std::string m_nom;       //nom de l'administrateur
    std::string m_mdp;          //mot de passe de l'administrateur
    std::string m_fichier;      //chemin d'accès du fichier joueur
    std::vector<std::string> m_info;//information de l'administrateur
    bool m_quitter;             //verification si l'administrateur veut quitter
    bool m_connection;             //verification si l'administrateur veut quitter
    Plan_Pistes m_plan;
public:
    Administrateurs();
    Administrateurs(Plan_Pistes p);
    Administrateurs(std::string pseudo, Plan_Pistes p);
    ~Administrateurs();
    bool getConnection() const;      //getter du statut de connection de l'administrateur
    std::string getNom() const;  //getter nom de l'administrateur
    bool getQuitter() const;        //getter si l'administrateur veut quitter
};

#endif // ADMINISTRATEURS_H_INCLUDED
