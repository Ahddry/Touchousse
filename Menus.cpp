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
        ///Où suis-je
        m_plan.effacer();
        m_plan.setup();
        s.arc();
        m_plan.standby();
        break;
    }
    case 3:
    {
        ///Dijkstra
        s.resetAttributs();
        m_plan.effacer();
        m_plan.setup();
        int point1, point2;
        s.saisieDijkstra(point1, point2);
        if(s.bfs(point1, point2))
        {
            s.dijkstra(point1, point2);
        }
        s.resetAttributs();
        break;
    }
    case 4:
    {
        ///Choix du chemin le plus interressant
        ///Sous menu
        while(!fin)
        {
            fin = cheminInterressant(s);
        }
        break;
    }
    case 5:
    {
        ///Algorithme de flow (Ford-Fulkerson)
        s.resetAttributs();
        m_plan.effacer();
        m_plan.setup();
        int point1, point2;
        s.saisieDijkstra(point1, point2);
        if(s.bfs(point1, point2))
        {
            s.fordFulkerson(point1, point2);
        }
        s.resetAttributs();
        break;

    }
    case 6:
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

bool Menus::cheminInterressant(Station s)    ///Menu du choix des chemins en fonction de critères définis par l'utilisateur
{
    bool quit = false;
    m_plan.effacer();
    int choix;
    choix = m_plan.menuExtras(); //Selection du choix de l'utilisateur et affichage du menu
    switch(choix)
    {
    case 1:
    {
        s.resetAttributs();
        ///En utilisant des pré-selections
        std::string titre = "Veuillez choisir votre preselection de chemin";
        std::vector<std::string> choix;
        choix.push_back("Niveau pioupiou");
        choix.push_back("Je sais skiier");
        choix.push_back("J'aime le risque");
        choix.push_back("Ne pas dechausser");
        int selec = m_plan.menu(titre, choix, 4);
        s.preselec(selec);
        s.resetAttributs();
        break;
    }
    case 2:
    {
        s.resetAttributs();
        ///En choisissant quels trajets banir
        m_plan.setup();
        m_plan.emphase("Trajet personnalise", "Choississez quels types de trajets vous desirez");
        rest(1200);
        s.personnalise();
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
