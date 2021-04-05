#ifndef PLAN_PISTES_H_INCLUDED
#define PLAN_PISTES_H_INCLUDED

#include "Point.h"

#include <vector>
#include <allegro.h>
#include <png.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include <time.h>
#include <string>

class Plan_Pistes
{
private:
    BITMAP *m_background;   //image de fond de l'affichage
    BITMAP *m_plan;        //Bitmap servent de buffer pour l'affichage
    BITMAP *m_panneau;     //image du panneau d'affichage

public:
    Plan_Pistes();
    Plan_Pistes(std::string init);//Initialisation du mode graphique avec tout ses prérequis
    ~Plan_Pistes();
    void setup();           //affichage des éléments de base d'une partie
    BITMAP * load_bitmap_check(std::string nomImage);//Méthode du chargement d'une image en vérifiant son existance, obtenue sur le site de M. Fercoq
    std::vector<std::string> text(std::string str);//Méthode permettant de séparer une chaîne de charactères en 2 aux endroits comprenant un #, recyclée de notre projet du semestre dernier
    void afficher();        //affichages des éléments du buffer

    void point(Point p);            //affichage d'un point sur le buffer
    void trajet(Trajet t, Point depart, Point arrivee, int compteur);   //affichage d'un arc sur le buffer
    void trait(std::string type, int x1, int y1, int x2, int y2, int couleur); //Un trait sophistiqué

    void emphase(std::string titre, std::string sousTitre);//Message accentué avec une première chose écrite en gros et une seconde en un peu plus petit en dessous
    int menu(std::string titre, std::vector<std::string> choix, int nbChoix);//menu divers avec choix paramétrables, attendant un clic de l'utilisateur sur une des options proposées
    std::string saisie(std::string titre, std::string texte);//saisie similaire à un "std::getline(std::cin, variable)" de l'utilisateur, pour allegro
    void effacer();         //effacer tous les éléments actuellement en buffer
    void erreur(std::string msg);//message d'erreur en rouge au bas de l'écran
    int menuPrincipal();    //Menu principal du jeu, attendant un clic de l'utilisateur sur une des options proposées
    void standby();         //attente que l'utilisateur clique sur fermer ou appuie sur [ECHAP]
};


#endif // PLAN_PISTES_H_INCLUDED
