#include "Station.h"
#include <queue>
#include <fstream>
#include <iostream>

Station::Station(std::string fichier)
{
    std::ifstream ifs{fichier};         //Lecture du fichier
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + fichier );
    ifs >> m_ordre;
    for(int i=0; i<m_ordre;i++)         //Création des points
    {
        int n;
        ifs>>n;
        m_points.push_back(Point(n));
    }
    ifs >> m_taille;
    for(int i=0; i<m_taille; i++)       //Creation des arcs
    {
        int d, a, p;
        ifs>>d>>a>>p;
        Trajet arr(d,a,p);
        m_points[d].ajoutSuiv(arr);
        m_points[a].ajoutAnte(arr);
    }
}

Station::~Station(){}

int Station::getOrdre() const    //getter pour l'ordre du graphe
{
    return m_ordre;
}

void Station::dijkstra(int depart, int arrivee)  ///Algorithme de Dijkstra
{
    for(auto& elem:m_points)                   //Mise à blanc des points
    {
        elem.setCouleur(0);
    }
    m_points[depart].setDistance(0);       //Le point de départ a une distance de 0
    m_points[depart].setCouleur(1);        //et est marqué comme repéré
    bool fin = false;
    do
    {
        int numS = -1;                      //numéro de point
        int dist = INT_MAX;                 //Distance la plus courte au point de départ
        for(auto& elem:m_points)
        {
            if(elem.getCouleur()==1 && elem.getDistance()<dist) //Récupération du point non marqué comme finit avec la distance la plus courte au point de départ
            {
                numS = elem.getNum();
                dist = elem.getDistance();
            }
        }
        for(auto& truc:m_points[numS].getSuiv())               //Récupération des points suivant ce dernier
        {
            int z = truc.getArrivee();
            if(m_points[z].getCouleur()==0)                    //S'ils ne sont pas encore marqués
            {
                m_points[z].setDijk(numS);                     //Ajout comme antécédant
                m_points[z].setDistance(truc.getPoids()+dist); //Attribution de la distance au départ
                m_points[z].setCouleur(1);                     //Marquage du point
            }
            else if(m_points[z].getDistance()>(dist+truc.getPoids()))  //S'ils sont marqués mais que leur distance doit être mise à jour
            {
                m_points[z].setDijk(numS);                         //Remplacement de l'antécédant
                m_points[z].setDistance(truc.getPoids()+dist);     //mise à jour de la distance
            }

        }
        m_points[numS].setCouleur(2);                          //Marquage du point actuel comme finit
        fin = true;
        for(const auto& elem:m_points)
        {
            if(elem.getCouleur() != 2) fin = false;             //Vérification de si tous les points ont étés marqués ou non.
        }
    }while(!fin);
    std::cout<<"Resultat du parcourt de la file par algorithme de Dijkstra : "<<std::endl;
    std::cout<<"\nDistance parcourue : "<<m_points[arrivee].getDistance()<<std::endl;
    std::cout<<arrivee;
    int ante = m_points[arrivee].getDijk();
    while(true)
    {
        if(ante!=(-1))                      //Affichage du point d'arrivé et du chemin parcourut
        {
            std::cout<<" <-- "<< ante;
            ante= m_points[ante].getDijk();
        }
        else break;
    }
    std::cout<<std::endl;
}

void Station::afficher() const       //Affichage du graphe
{
    std::cout<<"  Ordre = "<< m_ordre<<std::endl;
    std::cout<<"  Listes d'adjacence :"<<std::endl;
    for(const auto& elem:m_points)
    {
        elem.afficher();
    }
}
