#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include "Plan_Pistes.h"
#include "Station.h"

class Menus
{
private:
    Plan_Pistes m_plan;

public:
    Menus(Plan_Pistes p);
    ~Menus();
    bool principal();       //menu principal
    bool cheminInterressant(); //menu des extensions
};

#endif // MENUS_H_INCLUDED
