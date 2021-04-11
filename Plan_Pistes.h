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
    Plan_Pistes(std::string init);//Initialisation du mode graphique avec tout ses pr�requis
    ~Plan_Pistes();
    void setup();           //affichage des �l�ments du panneau interactif
    BITMAP * load_bitmap_check(std::string nomImage);//M�thode du chargement d'une image en v�rifiant son existance, obtenue sur le site de M. Fercoq
    std::vector<std::string> text(std::string str);//M�thode permettant de s�parer une cha�ne de charact�res en 2 aux endroits comprenant un #, recycl�e de notre projet du semestre dernier
    void afficher();        //affichages des �l�ments du buffer

    void descripPistes();   //Affichage des l�gendes des pistes

    void point(Point p);            //affichage d'un point sur le buffer
    void trajet(Trajet t, Point depart, Point arrivee, int compteur);   //affichage d'un arc sur le buffer
    void trait(std::string type, int x1, int y1, int x2, int y2, int couleur); //Un trait sophistiqu�

    Point selecPoint(std::vector<Point> ensemble) const;        //Permet de selectionner un point quand on clique dessus
    void infoPoint(Point point);                                //Affiche les information du point selectionn�
    void infoTrajet(Trajet traj, std::vector<Point> points);    //Affiche les informations sur le trajet rentr�
    void entreDeux(int depart, int arrivee, std::string type, int x, int y, bool point);//Affichage plus complet d'un trajet avec son type et ses points de d�part et d'arriv�e ou non, employ� pour certains menus d'inforamtions
    std::string longType(std::string type) const;               //renvoie la version longue du nom du type de trajet envoy� en param�tres
    std::string cutDouble(double nombre);                       //Permet de simplifier les variable de type double pour les affichages
    bool personnalise(std::vector<std::pair<std::string,int>>& connex); //Permet � l'utilisateur de faire une s�lection des types de trajets qu'il d�sire emprunter
    bool pannelSimple(std::vector<std::pair<std::string,bool>>& connex);//Panneau de s�lection de l'administrateur pour l'ouverture ou la fermeture des trajets par types
    bool pannelAdvance(std::vector<Trajet>& trajets, int& page);        //Panneau de s�lection de l'administrateur pour l'ouverture ou la fermeture des trajets individuellement

    void emphase(std::string titre, std::string sousTitre);//Message accentu� avec une premi�re chose �crite en gros et une seconde en un peu plus petit en dessous
    int menu(std::string titre, std::vector<std::string> choix, int nbChoix);//menu divers avec choix param�trables, attendant un clic de l'utilisateur sur une des options propos�es
    std::string saisie(std::string titre, std::string texte);//saisie similaire � un "std::getline(std::cin, variable)" de l'utilisateur, pour allegro
    void effacer();         //effacer tous les �l�ments actuellement en buffer
    void erreur(std::string msg);//message d'erreur en rouge au bas de l'�cran
    void regles(std::vector<std::string> regles);
    int menuPrincipal();    //Menu principal de l'application, attendant un clic de l'utilisateur sur une des options propos�es
    int menuExtras();       //Menu des extensions de l'application, attendant un clic de l'utilisateur sur une des options propos�es
    int menuPersonalisation();//Menu de la s�lection de l'utilisateur pour le choix de presets ou de faire une s�lection personnatis�e, attendant un clic de l'utilisateur sur une des options propos�es
    void standby();         //attente que l'utilisateur clique sur fermer ou appuie sur [ECHAP]
};


#endif // PLAN_PISTES_H_INCLUDED
