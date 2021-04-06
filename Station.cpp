#include "Station.h"
#include <queue>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <utility>

Station::Station(Plan_Pistes p) : m_plan(p)
{
    std::string fichier = "data_arcs.txt";
    std::ifstream ifs{fichier};         //Lecture du fichier
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + fichier );
    m_points.push_back(Point(0, "FakeNum0", 0, -815, -815));
    ifs >> m_ordre;                     //Lecture du nombre de points
    for(int i=0; i<m_ordre;i++)         //Création des points
    {
        int n, alt, x, y;
        std::string lieu;
        ifs>>n>>lieu>>alt>>x>>y;
        m_points.push_back(Point(n, lieu, alt, x, y));
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
        m_trajets.push_back(trj);
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

class TestPoids
{
public:
    int operator() (const Trajet& t1, const Trajet& t2) //Définition de la comparaison d'une Arête à une autre pour la priority_queue
    {
        if(t1.getPoids()==t2.getPoids())
            return t1.getDepart() > t2.getDepart(); //En cas de même poids, tri par ordre croissant du nombre de départ
        return t1.getPoids() > t2.getPoids();       //Comparaison du poids d'une arête à celui d'une autre
    }
};

void Station::dijkstra(int depart, int arrivee)  ///Algorithme de Dijkstra
{
    for(auto& elem:m_points)                   //Mise à blanc des points
    {
        elem.setCouleur(0);
    }
    m_points[depart].setDistance(0);       //Le point de départ a une distance de 0
    m_points[depart].setCouleur(1);        //et est marqué comme repéré
    m_points[0].setCouleur(2);             //on met le points 0 qui est fictif à 2
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

    for(auto& elem:m_points)
    {
        elem.setSelectDijk(false);
    }

    m_points[arrivee].setSelectDijk(true);

    m_plan.setup();
        ///A DEBUGGER
    while(true)
    {
        if(ante!=(-1))                      //Affichage du point d'arrivé et du chemin parcourut
        {
            std::cout<<" <-- "<< ante;
            m_points[ante].setSelectDijk(true);
            m_plan.point(m_points[ante]);
            //ante= m_points[ante].getDijk();
            std::vector<Trajet> traj = m_points[ante].getAnte();
            std::priority_queue<Trajet, std::vector<Trajet>, TestPoids> file; //File de priorité des arêtes pondérées, triées par ordre croissant de point d'arrivée
            ante= m_points[ante].getDijk();
            for(const auto& trajets: traj)
            {
                if(trajets.getDepart()==ante)
                    file.push(trajets);
            }
            Trajet t = file.top();
            m_plan.trajet(t,m_points[t.getDepart()], m_points[t.getArrivee()],0);
        }
        else break;
    }

    m_plan.afficher();
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


class Comparaison
{
public:
    int operator() (const Trajet& t1, const Trajet& t2) //Définition de la comparaison d'une Arête à une autre pour la priority_queue
    {
        if(t1.getArrivee()==t2.getArrivee())
            return t1.getDepart() > t2.getDepart(); //En cas de même poids, tri par ordre croissant du nombre de départ
        return t1.getArrivee() > t2.getArrivee();       //Comparaison du poids d'une arête à celui d'une autre
    }
};

void Station::interactif()
{
    for(const auto& elem:m_points)
    {
        m_plan.point(elem);
    }
    std::vector<std::pair<int,int>> arrivees;
    for(const auto &elem:m_points)
    {
        std::vector<Trajet> traj = elem.getSuiv();

        std::priority_queue<Trajet, std::vector<Trajet>, Comparaison> file; //File de priorité des arêtes pondérées, triées par ordre croissant de point d'arrivée
        for(const auto& trajets: traj)
        {
            file.push(trajets);
        }
        while(!file.empty())
        {
            int compteur = 0;
            Trajet t = file.top();
            std::pair<int, int> connex = {t.getDepart(), t.getArrivee()};
            for(const auto& point:arrivees)
            {
                if(connex.first == point.first && connex.second==point.second) compteur++;
                if(connex.first == point.second && connex.second==point.first) compteur++;
            }
            m_plan.trajet(t, elem, m_points[t.getArrivee()], compteur);

            compteur = 0;
            arrivees.push_back(connex);
            file.pop();
        }

    }
    m_plan.descripPistes();
    m_plan.afficher();
    Point selection = m_plan.selecPoint(m_points);
    m_plan.infoPoint(selection);
}

void Station::arc()
{
    std::string saisie = m_plan.saisie("Ou vous trouvez-vous ?", "Veuillez saisir le numero ou le nom de votre trajet :");
    bool trouve = false;
    if(atoi((saisie).c_str()) == 0)
    {
        for(const auto& elem:m_trajets)
        {
            if(saisie == elem.getNom())
            {
                m_plan.infoTrajet(elem, m_points);
                trouve = true;
            }
        }
    }
    else
    {
        for(const auto& elem:m_trajets)
        {
            if(atoi(saisie.c_str()) == elem.getNum())
            {
                m_plan.infoTrajet(elem, m_points);
                trouve = true;
            }
        }
    }
    if(!trouve) m_plan.erreur("Le trajet saisi n'existe pas dans la station !");
}

void Station::saisieDijkstra(int& point1, int& point2)
{
    m_plan.effacer();
    m_plan.setup();

    do
    {
        point1 = atoi(m_plan.saisie("Ou vous trouvez-vous ?", "Veuillez saisir le numero de depart :").c_str());
        if(point1<=0){m_plan.erreur("La location n'existe pas");}
        if(point1>37){m_plan.erreur("La location n'existe pas");}
    }while(point1<=0||point1>37);

    m_plan.effacer();
    m_plan.setup();

    do
    {
        point2 = atoi(m_plan.saisie("Ou voulez allez ?", "Veuillez saisir le numero d'arrivee :").c_str());
        if(point2<=0){m_plan.erreur("La location n'existe pas");}
        if(point2>37){m_plan.erreur("La location n'existe pas");}
    }while(point2<=0||point2>37);
}





