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
    for(int i=0; i<m_ordre;i++)         //Cr�ation des points
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
    for(auto& elem:m_points)                   //Mise � blanc des points
    {
        elem.setCouleur(0);
    }
    m_points[depart].setDistance(0);       //Le point de d�part a une distance de 0
    m_points[depart].setCouleur(1);        //et est marqu� comme rep�r�
    bool fin = false;
    do
    {
        int numS = -1;                      //num�ro de point
        int dist = INT_MAX;                 //Distance la plus courte au point de d�part
        for(auto& elem:m_points)
        {
            if(elem.getCouleur()==1 && elem.getDistance()<dist) //R�cup�ration du point non marqu� comme finit avec la distance la plus courte au point de d�part
            {
                numS = elem.getNum();
                dist = elem.getDistance();
            }
        }
        for(auto& truc:m_points[numS].getSuiv())               //R�cup�ration des points suivant ce dernier
        {
            int z = truc.getArrivee();
            if(m_points[z].getCouleur()==0)                    //S'ils ne sont pas encore marqu�s
            {
                m_points[z].setDijk(numS);                     //Ajout comme ant�c�dant
                m_points[z].setDistance(truc.getPoids()+dist); //Attribution de la distance au d�part
                m_points[z].setCouleur(1);                     //Marquage du point
            }
            else if(m_points[z].getDistance()>(dist+truc.getPoids()))  //S'ils sont marqu�s mais que leur distance doit �tre mise � jour
            {
                m_points[z].setDijk(numS);                         //Remplacement de l'ant�c�dant
                m_points[z].setDistance(truc.getPoids()+dist);     //mise � jour de la distance
            }

        }
        m_points[numS].setCouleur(2);                          //Marquage du point actuel comme finit
        fin = true;
        for(const auto& elem:m_points)
        {
            if(elem.getCouleur() != 2) fin = false;             //V�rification de si tous les points ont �t�s marqu�s ou non.
        }
    }while(!fin);
    std::cout<<"Resultat du parcourt de la file par algorithme de Dijkstra : "<<std::endl;
    std::cout<<"\nDistance parcourue : "<<m_points[arrivee].getDistance()<<std::endl;
    std::cout<<arrivee;
    int ante = m_points[arrivee].getDijk();
    while(true)
    {
        if(ante!=(-1))                      //Affichage du point d'arriv� et du chemin parcourut
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
