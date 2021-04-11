#include <iostream>
#include "Plan_Pistes.h"
#include "Menus.h"

int main(int argc, char *argv[])
{
    Plan_Pistes p("");          //Création d'un objet Plan_Pistes permmetant les affichages graphiques
    Menus m(p);                 //Ouverture des menus
    bool fin = false;
    while(!fin)
    {
        fin = m.principal();    //On reste dans le menu tant que l'utilisateur ne veut pas quitter
    }
    return 0;

}
END_OF_MAIN();                  //Instruction Allegro de fin de programme
