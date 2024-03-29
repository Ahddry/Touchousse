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

    choix = m_plan.menuPrincipal();  //Selection du choix de l'utilisateur et affichage du menu
    switch(choix)
    {
    case 1:
    {
        ///Affichage du plan interactif
        Station s(m_plan);
        m_plan.effacer();
        m_plan.setup();
        s.interactif();
        m_plan.standby();
        break;
    }
    case 2:
    {
        ///O� suis-je
        Station s(m_plan);
        m_plan.effacer();
        m_plan.setup();
        s.arc();
        m_plan.standby();
        break;
    }
    case 3:
    {
        ///Dijkstra ou BFS
        Station s(m_plan);
        s.resetAttributs();
        std::vector<std::string> propos;
        propos.push_back("En nombre de pistes");
        propos.push_back("En duree de trajet");
        int choix = m_plan.menu("Comment determiner le plus court chemin ?", propos, 2);
        m_plan.effacer();
        m_plan.setup();
        int point1, point2;
        s.saisieDijkstra(point1, point2);
        if(choix == 1)
        {
            s.setAffBfs(true);
            m_plan.setup();
            s.bfs(point1, point2);
            m_plan.standby();
            s.setAffBfs(false);
        }
        else
        {
            if(s.bfs(point1, point2))
            {
                s.dijkstra(point1, point2);
            }
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
            fin = cheminInterressant();
        }
        break;
    }
    case 5:
    {
        ///Algorithme de flow (Ford-Fulkerson)
        Station s(m_plan);
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
        ///Extras
        ///Sous menu
        while(!fin)
        {
            fin = extras();
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

bool Menus::cheminInterressant()    ///Menu du choix des chemins en fonction de crit�res d�finis par l'utilisateur
{
    Station s(m_plan);
    bool quit = false;
    m_plan.effacer();
    int choix;
    choix = m_plan.menuPersonalisation(); //Selection du choix de l'utilisateur et affichage du menu
    switch(choix)
    {
    case 1:
    {
        s.resetAttributs();
        ///En utilisant des pr�-selections
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


bool Menus::extras()    ///Menu des extensions et des fonction d'administration
{
    bool quit = false;
    m_plan.effacer();
    int choix;
    choix = m_plan.menuExtras(); //Selection du choix de l'utilisateur et affichage du menu
    switch(choix)
    {
    case 1:
    {
        Administrateurs admin(m_plan);
        if(admin.getConnection())
        {
            Station s(m_plan);
            bool fin = false;
            while(!fin)
            {
                fin = fonctionsAdmin(s, admin);
            }
        }
        break;
    }
    case 2:
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

bool Menus::fonctionsAdmin(Station s, Administrateurs adm) ///Menu des fonctions Admin
{
    m_plan.effacer();
    std::string titre = "Veuillez choisir votre preselection de chemin";
    std::vector<std::string> choix;
    choix.push_back("Ouvrir/Fermer des trajets");
    choix.push_back("Ouvrir/Fermer des types de trajets");
    choix.push_back("Creer un nouveau compte administrateur");
    choix.push_back("Quitter");
    int selec = m_plan.menu(titre, choix, 4);
    bool quit = false;
    switch(selec)
    {
    case 1:     //Supprimer des trajets un � un
    {
        s.adminPanel(false);
        break;
    }
    case 2:     //Supprimer des types de trajets
    {
        s.adminPanel(true);
        break;
    }
    case 3:     //Ajouter un administrateur
    {
        m_plan.effacer();
        Administrateurs newAdmin(adm.getNom(), m_plan);
        if(newAdmin.getConnection())
        {
            m_plan.setup();
            m_plan.emphase(newAdmin.getNom(), "a bien ete ajoute a la liste des administrateurs");
            rest(1200);
        }
        break;
    }
    case 4 :
    {
        ///Quitter
        rest(400);
        std::vector<std::string> confirm;
        confirm.push_back("Oui");
        confirm.push_back("Non");
        int laConfirmation = m_plan.menu("Voulez-vous vraiment vous deconnecter ?", confirm, 2);
        if(laConfirmation == 2) quit = false;
        else quit = true;
        break;
    }
    default:
        m_plan.erreur("Choix non existant.");
        break;
    }
    return quit;
}
