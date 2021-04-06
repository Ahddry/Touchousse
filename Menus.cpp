#include "Menus.h"
#include <fstream>
#include <iostream>

Menus::Menus(Plan_Pistes p) : m_plan(p)
{
}
Menus::~Menus(){}

bool Menus::principal()///Menu principal du jeu
{
    rest(350);
    bool quit = false;
    bool fin = false;
    m_plan.effacer();
    int choix;
    Station s(m_plan);
    choix = m_plan.menuPrincipal();  //Selection du choix de l'utilisateur et affichage du menu
    switch(choix)
    {
    case 1:
    {
        ///Affichage du plan interactif
        m_plan.effacer();
        m_plan.setup();
        s.interactif();
        m_plan.standby();
        break;
    }
    case 2:
    {
        ///O� suis-je
        m_plan.effacer();
        m_plan.setup();
        s.arc();
        m_plan.standby();
        break;
    }
    case 3:
    {
        ///Dijkstra
        m_plan.effacer();
        m_plan.setup();
        int begin, end;
        s.saisieDijkstra(begin, end);
        s.dijkstra(begin,end);
        m_plan.standby();
        break;
    }
    case 4:
    {
        ///Choix du chemin le plus interressant
        ///Sous menu
        while(!fin)
        {
            fin = cheminInterressant();
        }
        break;
    }
    case 5:
    {
        ///Cr�dits
        std::vector<std::string> credits;
        std::ifstream fichierCredits("Credits.txt");//ouverture d'un flux de lecture vers le fichier des cr�dits
        if(fichierCredits)//si le fichier existe
        {
            std::string ligne;
            while(getline(fichierCredits, ligne))//lecture du fichier ligne par ligne
            {
                credits.push_back(ligne);
            }
            m_plan.regles(credits);          //affichage des cr�dits
        }
        else//si le fichier n'existe pas
        {
            m_plan.erreur("Erreur: Le fichier n'existe pas!");
        }
        break;
    }
    case 6:

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

bool Menus::cheminInterressant()    ///Menu du choix des chemins en fonction de crit�res d�finis par l'utilisateur
{
    bool quit = false;
    m_plan.effacer();
    int choix;
    choix = m_plan.menuExtras(); //Selection du choix de l'utilisateur et affichage du menu
    switch(choix)
    {
    case 1:
    {
        ///En utilisant des pr�-selections
        std::string titre = "Veuillez choisir votre pr�selection de chemin :";
        std::vector<std::string> choix;
        choix.push_back("Niveau pioupiou");
        choix.push_back("Je sais skiier");
        choix.push_back("J'aime le risque");
        choix.push_back("Ne pas dechausser");
        int selec = m_plan.menu(titre, choix, 4);
        break;
    }
    case 2:
    {
        ///En choisissant quelles trajets banir
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
