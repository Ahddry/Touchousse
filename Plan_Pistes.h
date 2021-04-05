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
    Plan_Pistes(std::string init);//Initialisation du mode graphique avec tout ses pr�requis
    ~Plan_Pistes();
    void setup();           //affichage des �l�ments de base d'une partie
    BITMAP * load_bitmap_check(std::string nomImage);//M�thode du chargement d'une image en v�rifiant son existance, obtenue sur le site de M. Fercoq
    std::vector<std::string> text(std::string str);//M�thode permettant de s�parer une cha�ne de charact�res en 2 aux endroits comprenant un #, recycl�e de notre projet du semestre dernier
    void afficher();        //affichages des �l�ments du buffer

    void point(Point p);            //affichage d'un point sur le buffer
    void trajet(Trajet t, Point depart, Point arrivee, int compteur);   //affichage d'un arc sur le buffer
    void trait(std::string type, int x1, int y1, int x2, int y2, int couleur); //Un trait sophistiqu�

    void emphase(std::string titre, std::string sousTitre);//Message accentu� avec une premi�re chose �crite en gros et une seconde en un peu plus petit en dessous
    int menu(std::string titre, std::vector<std::string> choix, int nbChoix);//menu divers avec choix param�trables, attendant un clic de l'utilisateur sur une des options propos�es
    std::string saisie(std::string titre, std::string texte);//saisie similaire � un "std::getline(std::cin, variable)" de l'utilisateur, pour allegro
    void effacer();         //effacer tous les �l�ments actuellement en buffer
    void erreur(std::string msg);//message d'erreur en rouge au bas de l'�cran
    int menuPrincipal();    //Menu principal du jeu, attendant un clic de l'utilisateur sur une des options propos�es
    void standby();         //attente que l'utilisateur clique sur fermer ou appuie sur [ECHAP]
};


#endif // PLAN_PISTES_H_INCLUDED
