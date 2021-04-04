#include "Station.h"
#include <queue>
#include <fstream>
#include <iostream>

Station::Station(std::string fichier)
{
    std::ifstream ifs{fichier};         //Lecture du fichier
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + fichier );
    m_points.push_back(Point(0, "FakeNum0", 0));
    ifs >> m_ordre;                     //Lecture du nombre de points
    for(int i=0; i<m_ordre;i++)         //Création des points
    {
        int n, alt;
        std::string lieu;
        ifs>>n>>lieu>>alt;
        m_points.push_back(Point(n, lieu, alt));
    }
    ifs >> m_taille;                    //Lecture du nombre de trajets existants
    for(int i=0; i<m_taille; i++)       //Creation des arcs
    {
        int num, d, a;
        std::string nom, type;
        ifs>>num>>nom>>type>>d>>a;      ///CALCUL DES DISTANCES NECESSAIRE
        Trajet trj(num, nom, type, d, a, m_points[d].getAlt(), m_points[a].getAlt());
        m_points[d].ajoutSuiv(trj);
        m_points[a].ajoutAnte(trj);
    }
}

Station::~Station(){}

int Station::getOrdre() const    //getter pour l'ordre du graphe
{
    return m_ordre;
}

void Station::bfs(int depart, int arrivee)      ///Algorithme du BFS
{
    if(depart>m_ordre-1) std::cout<<"Le numero de point choisit pour commencer est invalide."<<std::endl;
    else
    {
        for(auto& elem: m_points)
        {   ///Code couleur : 0 = blanc, 1 = gris, 2 = noir
            elem.setCouleur(0);             //Colorer tous les points en blanc
        }
        m_points[depart].setCouleur(1);    //Colorer en gris le point de départ
        std::queue<Point> file;            //file de traitement
        file.push(m_points[depart]);       //ajout du point de départ à la file
        while(!file.empty())
        {
            Point pointTemp = file.front();        //récupération de la tête de file
            for(auto& traj:pointTemp.getSuiv())     //parcourt de ses successeurs
            {
                int succ = traj.getArrivee();
                if(m_points[succ].getCouleur()==0) //opération uniquement sur ceux blanc (non découverts
                {
                    m_points[succ].setCouleur(1);  //passage du point découvert à gris
                    m_points[succ].setBfs(pointTemp.getNum());  //marque de son prédécesseur
                    file.push(m_points[succ]);     //ajout du point découvert à la file
                }
            }
            pointTemp.setCouleur(2);                //passage du point traité à noir
            file.pop();                     //suppression du point traité de la file

        }
        std::cout<<"----------------------------"<<std::endl;
        std::cout<<"Resultat du parcourt de la file par BFS : "<<std::endl;
        for(const auto& elem:m_points)
        {
            std::cout<<elem.getNum();
            int anteBfs = elem.getBfs();
            while(true)
            {
                if(anteBfs!=(-1))
                {
                    std::cout<<" <-- "<< anteBfs;
                    anteBfs= m_points[anteBfs].getBfs();
                }
                else break;
            }
            std::cout<<std::endl;
        }
    }
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
        bool dist = INT_MAX;                 //Distance la plus courte au point de départ
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
    std::cout<<"Ordre = "<< m_ordre<<std::endl;
    std::cout<<"Listes d'adjacence :"<<std::endl;
    for(unsigned int i=1; i<m_points.size(); i++)
    {
        std::string lieu = m_points[i].getLieu();
        int a = atoi(lieu.c_str());
        if(a==0) std::cout<<lieu<<" : "<<std::endl;
        else std::cout<<"Point "<<m_points[i].getNum()<<" : "<<std::endl;
        std::cout<<"Antecedants : ";
        for(const auto& elem: m_points[i].getAnte()) std::cout<<elem.getDepart()<<" : "<<m_points[elem.getDepart()].getLieu()<<" | ";
        std::cout<<"\nSuivants : ";
        for(const auto& elem: m_points[i].getSuiv()) std::cout<<elem.getArrivee()<<" : "<<m_points[elem.getArrivee()].getLieu()<<" | ";
        std::cout<<"\nAltitude : "<<m_points[i].getAlt();
        std::cout<<std::endl<<std::endl;
    }
}

void Station::resetAttributs()
{
    for(auto& elem:m_points)
    {
        elem.setDistance(INT_MAX);  //Initialisation de la distance comme étant maximale (infinie) au point de départ
        elem.setCouleur(0);         //Mise à l'état 0 de la couleur
        elem.setDijk(-1);           //Mise à -1 (aucun) de l'antécédant
        elem.setBfs(-1);            //Mise à -1 (aucun) de l'antécédant
    }
}

void Station::aretes()
{
    for(auto& elem:m_points)
    {
        for(auto& traj: elem.getSuiv())
        {
            std::cout<<traj.getNum()<<"\t"<<traj.getNom()<<"\t"<<traj.getType()<<"\t"<<traj.getDepart()<<"\t"<<traj.getArrivee()<<"\t|"<<traj.getPoids()<<std::endl;
        }
    }
}
