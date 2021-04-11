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
    BITMAP *m_background;  //image de fond de l'affichage
    BITMAP *m_plan;        //Bitmap servent de buffer pour l'affichage

public:
    Plan_Pistes();
    Plan_Pistes(std::string init);//Initialisation du mode graphique avec tout ses prérequis
    ~Plan_Pistes();
    void setup();           //affichage des éléments du panneau interactif
    BITMAP * load_bitmap_check(std::string nomImage);//Méthode du chargement d'une image en vérifiant son existance, obtenue sur le site de M. Fercoq
    std::vector<std::string> text(std::string str);//Méthode permettant de séparer une chaîne de charactères en 2 aux endroits comprenant un #, recyclée de notre projet du semestre dernier
    void afficher();        //affichages des éléments du buffer

    void descripPistes();   //Affichage des légendes des pistes

    void point(Point p);            //affichage d'un point sur le buffer
    void trajet(Trajet t, Point depart, Point arrivee, int compteur);   //affichage d'un arc sur le buffer
    void trait(std::string type, int x1, int y1, int x2, int y2, int couleur); //Un trait sophistiqué

    Point selecPoint(std::vector<Point> ensemble) const;        //Permet de selectionner un point quand on clique dessus
    void infoPoint(Point point);                                //Affiche les information du point selectionné
    void infoTrajet(Trajet traj, std::vector<Point> points);    //Affiche les informations sur le trajet rentré
    void entreDeux(int depart, int arrivee, std::string type, int x, int y, bool point);//Affichage plus complet d'un trajet avec son type et ses points de départ et d'arrivée ou non, employé pour certains menus d'inforamtions
    std::string longType(std::string type) const;               //renvoie la version longue du nom du type de trajet envoyé en paramètres
    std::string cutDouble(double nombre);                       //Permet de simplifier les variable de type double pour les affichages
    bool personnalise(std::vector<std::pair<std::string,int>>& connex); //Permet à l'utilisateur de faire une sélection des types de trajets qu'il désire emprunter
    bool pannelSimple(std::vector<std::pair<std::string,bool>>& connex);//Panneau de sélection de l'administrateur pour l'ouverture ou la fermeture des trajets par types
    bool pannelAdvance(std::vector<Trajet>& trajets, int& page);        //Panneau de sélection de l'administrateur pour l'ouverture ou la fermeture des trajets individuellement

    void emphase(std::string titre, std::string sousTitre);//Message accentué avec une première chose écrite en gros et une seconde en un peu plus petit en dessous
    int menu(std::string titre, std::vector<std::string> choix, int nbChoix);//menu divers avec choix paramétrables, attendant un clic de l'utilisateur sur une des options proposées
    std::string saisie(std::string titre, std::string texte);//saisie similaire à un "std::getline(std::cin, variable)" de l'utilisateur, pour allegro
    void effacer();         //effacer tous les éléments actuellement en buffer
    void erreur(std::string msg);//message d'erreur en rouge au bas de l'écran
    void regles(std::vector<std::string> regles);
    int menuPrincipal();    //Menu principal de l'application, attendant un clic de l'utilisateur sur une des options proposées
    int menuExtras();       //Menu des extensions de l'application, attendant un clic de l'utilisateur sur une des options proposées
    int menuPersonalisation();//Menu de la sélection de l'utilisateur pour le choix de presets ou de faire une sélection personnatisée, attendant un clic de l'utilisateur sur une des options proposées
    void standby();         //attente que l'utilisateur clique sur fermer ou appuie sur [ECHAP]
};


#endif // PLAN_PISTES_H_INCLUDED
