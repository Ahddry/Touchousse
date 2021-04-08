#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include "Administrateurs.h"
#include "Station.h"

class Menus
{
private:
    Plan_Pistes m_plan;

public:
    Menus(Plan_Pistes p);
    ~Menus();
    bool principal();       //menu principal
    bool cheminInterressant(); //menu du choix des chemins en fonction de critères définis par l'utilisateur
    bool extras(); //menu des extensions
    bool fonctionsAdmin(Station s, Administrateurs adm); //menu des fonctions Admin
};

#endif // MENUS_H_INCLUDED
