#include "Administrateurs.h"
#include <fstream>
#include <string>
#include <iostream>

Administrateurs::Administrateurs(){}
Administrateurs::Administrateurs(Plan_Pistes p) : m_plan(p)///constructeur d'un administrateur prenant en enter le pseudo de l'administrateur deja connecté
{
    m_plan.effacer();
    m_quitter=false;
    m_connection =false;
    bool fin = false;
    while(fin==false)
    {
        std::vector<std::string> propos;    //proposition à afficher à l'utilisateur souhaitant accéder à un compte
        propos.push_back("Se connecter");
        propos.push_back("Quitter");
        int rep = m_plan.menu("Menu Connexion", propos, 2);//choix de l'administrateur
        switch(rep)
        {
        case 1:///pour choix se connecter
            {
                std::string pseudo = "";
                m_plan.effacer();
                m_nom = m_plan.saisie("Connexion", "Nom d'utilisateur :");//recuperation du pseudo
                if(m_nom!=pseudo)//verification si le administrateur n'est pas deja connecté
                {
                    m_fichier= "Administrateurs/"+m_nom+".txt";//recuperation du chemin d'accès du fichier administrateur
                    std::ifstream fichierAdministrateurs(m_fichier);//ouverture d'un flux de lecture vers le fichier administrateur
                    if(fichierAdministrateurs)//si le fichier existe
                    {
                        m_plan.effacer();
                        m_mdp = m_plan.saisie("Connexion", "Mot de passe :");//récupération du mot de passe de l'administrateur
                        std::string ligne;
                        while(getline(fichierAdministrateurs, ligne))//lecture du fichier et enregistrement dans le tableau d'info administrateur
                        {
                            m_info.push_back(ligne);
                        }
                        if(m_info[1]==m_mdp)//si le mot de passe est bon
                        {
                            m_connection =true;
                            fin = true;
                        }
                        else//si le mot de passe est incorrect
                        {
                            m_plan.erreur("Mot de passe incorrect!");
                        }
                    }
                    else//si le nom d'utilisateur n'existe pas
                    {
                        m_plan.erreur("Erreur: Le nom d'utilisateur n'existe pas!");
                    }
                }
                else//si le administrateur est deja connecte
                {
                    m_plan.erreur("Cet administrateur est deja connecte!");
                }
                break;
            }
        case 2:///si choix est quitter
            {
                m_quitter=true;
                m_connection=false;
                fin = true;
                break;
            }

        }
    }
    if(m_quitter==true)//si choix de quitter
    {
        m_plan.effacer();
        m_plan.setup();
        m_plan.emphase("", "Bonne journee.");
        rest(400);
    }
    else
    {
        m_plan.setup();
        m_plan.emphase("Bienvenue", m_nom + " !");
        rest(600);
    }
}
Administrateurs::Administrateurs(std::string pseudo, Plan_Pistes p) : m_plan(p)
{
    //m_plan.effacer();
    m_quitter=false;
    m_connection =false;
    bool fin = false;
    while(fin==false)
    {
        std::vector<std::string> propos;    //proposition à afficher à l'utilisateur souhaitant accéder à un compte
        propos.push_back("Creer un nouveau compte ADMIN");
        propos.push_back("Quitter");
        int rep = m_plan.menu("Menu Creation de compte", propos, 2);//choix de l'administrateur
        switch(rep)
        {
        case 1:///pour choix créer un nouvel admin
            {
                m_plan.effacer();
                bool nomPris=false;
                while(nomPris==false)
                {
                    m_nom = m_plan.saisie("Creation de compte", "Nom d'utilisateur :");//récupération du choix du administrateur;
                    m_fichier= "Administrateurs/"+m_nom+".txt";//récupération du chemin d'accès du fichier du administrateur
                    std::ifstream fichierAdministrateurs(m_fichier.c_str());
                    if(fichierAdministrateurs)//si le fichier existe
                    {
                        m_plan.erreur("Erreur: Le nom d'utilisateur existe deja!");
                        nomPris=false;
                    }
                    else
                    {
                        nomPris=true;
                    }
                }
                m_mdp = m_plan.saisie("Creation de compte", "Mot de passe :");//récupération du mot de passe choisi
                m_fichier="Administrateurs/"+m_nom+".txt";
                std::ifstream testFichierAdministrateurs(m_fichier.c_str());
                if(testFichierAdministrateurs)//vérification si le fichier n'existe pas déja
                {
                    m_plan.erreur("Le fichier existe deja");
                }
                else
                {
                    std::ofstream fichierAdministrateurs(m_fichier.c_str());
                    if(fichierAdministrateurs)//ecriture du fichier administrateur
                    {
                        fichierAdministrateurs << m_nom<< std::endl;//ligne 1 pseudo
                        fichierAdministrateurs << m_mdp<< std::endl;//ligne 2 mot de passe
                    }
                    else
                    {
                        m_plan.erreur("Erreur ouverture du fichier impossible");
                    }
                }
                m_connection = true;//administrateur connecté
                fin = true;
                break;
            }
        case 2:///si choix est quitter
            {
                m_quitter=true;
                m_connection=false;
                fin = true;
                break;
            }

        }
    }
    if(m_quitter==true)//si choix de quitter
    {
        m_plan.effacer();
        m_plan.setup();
        m_plan.emphase("", "Bonne journee.");
        rest(400);
    }
    else
    {
        m_plan.setup();
        m_plan.emphase("Bienvenue", m_nom + " !");
        rest(600);
    }
}



Administrateurs::~Administrateurs(){}///Destructeur Administrateurs

bool Administrateurs::getConnection() const      //getter du statut de connection de l'administrateur
{
    return m_connection;
}

std::string Administrateurs::getNom() const//retourne le pseudo de l'administrateur sous forme de chaine de caractère
{
    return m_nom;
}
bool Administrateurs::getQuitter() const//retourne si le administrateur veux quitter sous forme d'un bool
{
    return m_quitter;
}
