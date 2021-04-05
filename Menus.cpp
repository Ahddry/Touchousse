#include "Menus.h"
#include <fstream>

Menus::Menus(Plan_Pistes p) : m_plan(p)
{
}
Menus::~Menus(){}

bool Menus::principal()///Menu principal du jeu
{
    bool quit = false;
    bool fin = false;
    m_plan.effacer();
    int choix;
    choix = m_plan.menuPrincipal();  //Selection du choix de l'utilisateur et affichage du menu
    switch(choix)
    {
    case 1:
    {
        m_plan.effacer();
        m_plan.setup();
        Station s(m_plan);
        s.interactif();
        m_plan.standby();
        break;
    }
    case 2:
    {
        ///Où suis-je

        break;
    }
    case 3:
    {
        ///On sait pas
        break;
    }
    case 4:
    {
        ///On sait pas
        break;
    }
    case 5:
    {
        ///Crédits
        std::vector<std::string> credits;
        std::ifstream fichierCredits("Credits.txt");//ouverture d'un flux de lecture vers le fichier des crédits
        if(fichierCredits)//si le fichier existe
        {
            std::string ligne;
            while(getline(fichierCredits, ligne))//lecture du fichier ligne par ligne
            {
                credits.push_back(ligne);
            }
            m_plan.regles(credits);          //affichage des crédits
        }
        else//si le fichier n'existe pas
        {
            m_plan.erreur("Erreur: Le fichier n'existe pas!");
        }
        break;
    }
    case 6:
        ///Sous menu extras
        while(!fin)
        {
            fin = extras();
        }
        break;
    case 7 :
        ///Quitter
        quit = true;
        break;
    default:
        m_plan.erreur("Choix non existant.");
        break;
    }
    return quit;
}

bool Menus::extras()///Menu des extensions
{
    bool quit = false;
    m_plan.effacer();
    int choix;
    choix = 3; //m_plan.menuExtras();//Selection du choix de l'utilisateur et affichage du menu
    switch(choix)
    {
    case 1:
        ///On sait pas
        break;
    case 2:
    {
        ///On sait pas
        break;
    }
    case 3 :
        ///Quitter
        quit = true;
        break;
    default:
        m_plan.erreur("Choix non existant.");
        break;
    }
    return quit;
}
