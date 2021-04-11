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
    for(int i=0; i<m_ordre;i++)         //Cr�ation des points
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
        bool active;
        ifs>>num>>nom>>type>>d>>a>>active;
        Trajet trj(num, nom, type, d, a, m_points[d].getAlt(), m_points[a].getAlt());
        if(active)
        {
            m_points[d].ajoutSuiv(trj);
            m_points[a].ajoutAnte(trj);
            trj.setActive(true);
        }
        else
        {
            trj.setSelec(false);
            trj.setActive(false);
        }
        m_trajets.push_back(trj);
    }
    m_testAffichage = true;
    m_affichageBfs = false;
}

Station::~Station(){}

int Station::getOrdre() const    //getter pour l'ordre du graphe
{
    return m_ordre;
}

bool Station::bfs(int depart, int arrivee)      ///Algorithme du BFS
{
    resetBfs();
    if(depart<1) m_plan.erreur("Le numero de point choisit pour commencer est invalide.");
    else
    {
        for(auto& elem: m_points)
        {   ///Code couleur : 0 = blanc, 1 = gris, 2 = noir
            elem.setCouleur(0);             //Colorer tous les points en blanc
        }
        m_points[depart].setCouleur(1);    //Colorer en gris le point de d�part
        std::queue<Point> file;            //file de traitement
        file.push(m_points[depart]);       //ajout du point de d�part � la file
        while(!file.empty())
        {
            Point pointTemp = file.front();        //r�cup�ration de la t�te de file
            for(auto& traj:pointTemp.getSuiv())     //parcourt de ses successeurs
            {
                if(selecArc(traj))
                {
                    int succ = traj.getArrivee();
                    if(m_points[succ].getCouleur()==0) //op�ration uniquement sur ceux blanc (non d�couverts
                    {
                        m_points[succ].setCouleur(1);  //passage du point d�couvert � gris
                        m_points[succ].setBfs(pointTemp.getNum());  //marque de son pr�d�cesseur
                        file.push(m_points[succ]);     //ajout du point d�couvert � la file

                    }
                }
            }
            pointTemp.setCouleur(2);                //passage du point trait� � noir
            file.pop();                     //suppression du point trait� de la file

        }
        for(const auto& elem:m_points)
        {
            int anteBfs = elem.getBfs();
            while(true)
            {
                if(anteBfs!=(-1))
                {
                    anteBfs= m_points[anteBfs].getBfs();
                }
                else break;
            }
        }
        for(auto& elem:m_points)
        {
            if(elem.getBfs()==-1)
            {
                elem.setSelectDijk(false);  //Mise � faux du crit�re de s�lection de point pour Dijkstra si celui-ci n'est pas atteignable
            }

        }
        if(m_affichageBfs)  //Affichage graphique du BFS
        {
            int dest = arrivee;
            int ante = m_points[arrivee].getBfs();
            //m_plan.point(m_points[arrivee]);
            while(ante!=-1)
            {

                std::vector<Trajet> trajets = m_points[dest].getAnte();
                for(auto& t:trajets)                //R�cup�ration des diff�rents trajets
                {
                    if(t.getDepart()==ante)
                    {
                        m_plan.trajet(t, m_points[t.getDepart()], m_points[t.getArrivee()], 0); //Affichages graphiques du BFS
                        m_plan.point(m_points[t.getDepart()]);
                        m_plan.point(m_points[t.getArrivee()]);
                        break;
                    }
                }
                dest = ante;
                ante = m_points[dest].getBfs();
            }
        }
        m_points[depart].setSelectDijk(true);
        if(m_points[arrivee].getBfs()==-1)      //On test si le point d'arriv�e selectionn� est atteignable
        {
            if(m_testAffichage) m_plan.erreur("Le point d'arrivee n'est pas atteignable avec les criteres actuels.");
            return false;
        }
        else
        {
            if(m_affichageBfs) m_plan.emphase("Voici le plus court chemin", "en nombre de trajets jusqu'a "+m_points[arrivee].getLieu());
            return true;
        }
    }
    if(m_testAffichage) m_plan.erreur("Le point d'arrivee n'est pas atteignable avec les criteres actuels.");
    return false;

}

class TestPoids                     ///Classe permetant la comparaison d'une arete � une autre pour la pirority queue en terme de poids
{
public:
    double operator() (const Trajet& t1, const Trajet& t2) //D�finition de la comparaison d'une Ar�te � une autre pour la priority_queue
    {
        if(t1.getPoids()==t2.getPoids())
            return t1.getDepart() > t2.getDepart(); //En cas de m�me poids, tri par ordre croissant du nombre de d�part
        return t1.getInteret() > t2.getInteret();       //Comparaison du poids d'une ar�te � celui d'une autre
    }
};

void Station::dijkstra(int depart, int arrivee)  ///Algorithme de Dijkstra
{
    for(auto& elem:m_points)                   //Mise � blanc des points
    {
        elem.setCouleur(0);
        if(!elem.getSelectDijk())
        {
            elem.setCouleur(2);
        }
    }
    m_points[depart].setDistance(0);       //Le point de d�part a une distance de 0
    m_points[depart].setCouleur(1);        //et est marqu� comme rep�r�
    m_points[0].setCouleur(2);             //on met le points 0 qui est fictif � 2
    bool fin = false;
    int boucle = 1;
    do
    {
        boucle++;
        int numS = -1;                      //num�ro de point
        double dist = INT_MAX;                 //Distance la plus courte au point de d�part
        for(auto& elem:m_points)
        {
            if(elem.getCouleur()==1 && elem.getDistance()<dist) //R�cup�ration du point non marqu� comme finit avec la distance la plus courte au point de d�part
            {
                numS = elem.getNum();
                dist = elem.getDistance();
            }
        }
        for(auto& truc:m_points[numS].getSuiv())               //R�cup�ration des arcs suivant ce dernier
        {
            if(selecArc(truc))
            {
                double poids = truc.getPoids(), cste = 1;
                for(const auto& type: m_ininteret)
                {
                    if(truc.getType() == type) cste = 150.4;            //Si le type de trajet � �viter et le m�me que celui de l'arc alors on lui applique un coefficient pour �viter de le selctionner
                }
                poids *= cste;
                int z = truc.getArrivee();
                if(m_points[z].getCouleur()==0)                    //Si leurs points d'arriv�e ne sont pas encore marqu�s
                {
                    m_points[z].setDijk(numS);//Ajout comme ant�c�dant
                    m_points[z].setDistance(poids+dist); //Attribution de la distance au d�part
                    m_points[z].setCouleur(1);                     //Marquage du point
                }
                else if(m_points[z].getDistance()>(dist+poids))  //S'ils sont marqu�s mais que leur distance doit �tre mise � jour
                {
                    m_points[z].setDijk(numS);    //Remplacement de l'ant�c�dant
                    m_points[z].setDistance(poids+dist);     //mise � jour de la distance

                }
            }
        }
        m_points[numS].setCouleur(2);                          //Marquage du point actuel comme finit
        fin = true;
        for(const auto& elem:m_points)
        {
            if(elem.getCouleur() != 2) fin = false;             //V�rification de si tous les points ont �t�s marqu�s ou non.
        }
    }while(!fin);
    int ante = m_points[arrivee].getDijk();

    for(auto& elem:m_points)            //On met l'�tat des sommets selectionner du dijkstra � faux
    {
        elem.setSelectDijk(false);
    }

    m_points[arrivee].setSelectDijk(true);          //L'�tat du sommet d'arriv�e est mis � vrai
    double duree = 0;
    m_plan.setup();
    int actuel;
    while(true)
    {
        if(ante!=(-1))                      //Affichage du point d'arriv� et du chemin parcourut
        {
            m_points[ante].setSelectDijk(true);
            ante= m_points[ante].getDijk();
        }
        else break;
    }
    ante = arrivee;
    for(auto& elem: m_points)
    {
        if(m_points[ante].getSelectDijk() && ante!=-1)                      //Affichage du point d'arriv� et du chemin parcourut
        {
            m_plan.point(m_points[ante]);
            actuel = ante;
            ante= m_points[ante].getDijk();
            std::vector<Trajet> traj = m_points[actuel].getAnte();

            for(const auto& type: m_ininteret)
            {
                for(auto& trajets: traj)
                {
                    if(trajets.getType() == type)
                        trajets.setInteret(false);
                }
            }

            std::priority_queue<Trajet, std::vector<Trajet>, TestPoids> file; //File de priorit� des ar�tes pond�r�es, tri�es par ordre croissant de point d'arriv�e
            for(const auto& trajets: traj)
            {
                if(trajets.getDepart() == ante)
                    file.push(trajets);
            }
            bool affiche = false;
            if(ante !=-1)
            {
                while(!affiche)
                {
                    Trajet t = file.top();
                    if(selecArc(t))
                    {
                        duree+=t.getPoids();                    //Affichage graphique de la dur�e de parcourt du chemin trouv�
                        m_plan.trajet(t,m_points[t.getDepart()], m_points[t.getArrivee()],0);
                        affiche = true;
                    }
                    else file.pop();
                }
            }
        }
        elem.setBfs(-1);            //Pour �viter un warning sur la non utilisation de la varible "elem" -> sans effet reel.
    }
    m_plan.descripPistes();
    m_plan.afficher();
    m_plan.standby();
    m_plan.emphase("La duree moyenne de ce chemin est de", m_plan.cutDouble(duree)+" minutes");
    m_plan.standby();
}

class Comparaison               ///Classe permetant la comparaison d'une arete � une autre pour la pirority queue en terme de num�ro d'arriv�e
{
public:
    int operator() (const Trajet& t1, const Trajet& t2) //D�finition de la comparaison d'une Ar�te � une autre pour la priority_queue
    {
        if(t1.getArrivee()==t2.getArrivee())
            return t1.getDepart() > t2.getDepart();     //En cas de m�me num�ro d'arriv�e, tri par ordre croissant du nombre de d�part
        return t1.getArrivee() > t2.getArrivee();       //Comparaison du num�ro d'arriv�e d'une ar�te � celui d'une autre
    }
};

void Station::fordFulkerson(int depart, int arrivee)    //Algorithme de Ford-Fulkerson pour d�terminer le flot horaire maximal de skieurs entre deux points
{
    m_plan.setup();
    m_testAffichage = false;
    int flowMax = 0, flowMin = 0;
    int anteBfs, compteur = 0;
    std::priority_queue<Trajet, std::vector<Trajet>, Comparaison> file; //File de priorit� des ar�tes pond�r�es, tri�es par ordre croissant de point d'arriv�e
    std::vector<Trajet> testDoublons;
    std::vector<Trajet> testDoublons2;
    std::vector<Trajet> sature;
    m_plan.point(m_points[depart]);
    while(bfs(depart,arrivee))          //Parcours du trajet par le BFS
    {
        flowMin = INT_MAX;
        for(int i = arrivee; i!=depart; i = anteBfs)
        {
            anteBfs = m_points[i].getBfs();
            if(anteBfs!=(-1))
            {
                for(auto& elem:m_trajets)
                {
                    if(elem.getDepart() == anteBfs && elem.getArrivee() == i && elem.getSelec())
                    {
                        int capres = elem.getCapacite()-elem.getFlux();         //r�cup�ration de la capacit� r�sultante sur un arc
                        flowMin = std::min(flowMin, capres);                    //Recherche du flux maximal possible sur un chemin
                        testDoublons.push_back(elem);       //Test pour l'affichage graphique
                        m_plan.point(m_points[i]);          //Affichage graphique des points parcourus lors du chemin
                    }
                }
            }

        }
        for(int i = arrivee; i!=depart; i = anteBfs)    //ajout du flux trouv� au chemin
        {
            anteBfs = m_points[i].getBfs();
            if(anteBfs!=(-1))
            {
                for(auto& elem:m_trajets)
                {
                    if(elem.getDepart() == anteBfs && elem.getArrivee() == i && elem.getSelec())
                    {
                        elem.setFlux(elem.getFlux()+ flowMin);  //ajut du flux trouv� sur chauqe arc du chemin
                        if(elem.getFlux()>=elem.getCapacite())  //si l'arc atteint sa capacit� maximal de flux
                        {
                            elem.setSelec(false);               //l'enlever des arcs s�lectionnables
                            sature.push_back(elem);             //le marquer comme satur� pour l'affichage graphique
                        }
                    }

                }
            }

        }
        flowMax += flowMin;         //Ajout du flux trouv� au flux total
        resetBfs();
    }
    std::vector<std::pair<int,int>> arrivees;
    for(auto& elem :testDoublons)       //Test pour l'affichage graphique des arcs (pour qu'ils ne se superposent pas)
    {
        bool doublon = false;
        for(auto& traj: testDoublons2)
        {
            if(elem.getDepart()==traj.getDepart() && elem.getArrivee()==traj.getArrivee() && elem.getType()==traj.getType())
                doublon = true;
        }
        if(!doublon)
            testDoublons2.push_back(elem);
    }
    for(auto& elem:testDoublons2)
    {
        file.push(elem);
    }
    while(!file.empty())    //Boucle d'affichage graphique des arcs parcourus par un flux
    {
        compteur = 0;
        Trajet t = file.top();
        std::pair<int, int> connex = {t.getDepart(), t.getArrivee()};
        for(const auto& point:arrivees)
        {
            if(connex.first == point.first && connex.second==point.second) compteur++;
            if(connex.first == point.second && connex.second==point.first) compteur++;
        }
        m_plan.trajet(t, m_points[t.getDepart()], m_points[t.getArrivee()], compteur);

        compteur = 0;
        arrivees.push_back(connex);
        file.pop();
    }
    m_plan.emphase("Le flux horaire maximal est de " + std::to_string(flowMax), "Entre les points " + m_points[depart].getLieu() + " et " + m_points[arrivee].getLieu());
    m_plan.standby();
    m_plan.effacer();
    m_plan.setup();
    for(auto& elem:sature)  //boucle d'affichage des arcs satur�s et des points auquels ils sont reli�s
    {
        elem.setSelec(true);
        m_plan.trajet(elem, m_points[elem.getDepart()], m_points[elem.getArrivee()], 0);
        m_plan.point(m_points[elem.getDepart()]);
        m_plan.point(m_points[elem.getArrivee()]);
    }
    m_plan.emphase("Ces trajets sont satures", "Il faudrait si possible augmenter leur capacite horaire");  //un message
    m_plan.standby();
    m_testAffichage = true;
}


void Station::resetAttributs()
{
    for(auto& elem:m_points)
    {
        elem.setDistance(INT_MAX);  //Initialisation de la distance comme �tant maximale (infinie) au point de d�part
        elem.setCouleur(0);         //Mise � l'�tat 0 de la couleur
        elem.setDijk(-1);           //Mise � -1 (aucun) de l'ant�c�dant
        elem.setBfs(-1);            //Mise � -1 (aucun) de l'ant�c�dant
        elem.setSelectDijk(true);   //Mise � vrai du crit�re de s�lection
    }
    for(auto& elem:m_trajets)
    {
        elem.setSelec(true);        //Mise � vrai du crit�re de s�lection de trajet
        elem.setInteret(true);      //Mise � vrai de l'int�ret pour l'arc
        elem.setFlux(0);            //Mise � 0 du flux sur l'arc
        if(!elem.getActive())
        {
            elem.setSelec(false);
        }
    }
    while(!m_ininteret.empty())
    {
        m_ininteret.pop_back();     //Vidage de la liste des arcs non int�r�ssants
    }
}

void Station::resetBfs()
{
    for(auto& elem:m_points)
    {
        elem.setCouleur(0);         //Mise � l'�tat 0 de la couleur
        elem.setBfs(-1);            //Mise � -1 (aucun) de l'ant�c�dant
    }
}

void Station::interactif()
{
    for(const auto& elem:m_points)          //On cr�er les points sur le plan interactif
    {
        m_plan.point(elem);
    }
    std::vector<std::pair<int,int>> arrivees;
    for(const auto &elem:m_points)
    {
        std::vector<Trajet> traj = elem.getSuiv();

        std::priority_queue<Trajet, std::vector<Trajet>, Comparaison> file; //File de priorit� des ar�tes pond�r�es, tri�es par ordre croissant de point d'arriv�e
        for(const auto& trajets: traj)
        {
            file.push(trajets);     //Remplissage de la file de priorit�
        }
        while(!file.empty())        //Boucle d'affichage des arcs
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
    m_plan.descripPistes();     //Affichage de la l�gende de pistes
    m_plan.afficher();
    Point selection = m_plan.selecPoint(m_points);  //Selection cliquable de point
    m_plan.infoPoint(selection);    //Affichage des informations sur le point s�lectionn�
}

void Station::arc()
{   //Saisie s�curis�e de l'arc et conversion de la saisie en int
    std::string saisie = m_plan.saisie("Ou vous trouvez-vous ?", "Veuillez saisir le numero ou le nom de votre trajet :");
    bool trouve = false;
    if(atoi((saisie).c_str()) == 0)      //Si l'int est =0 alors c'est que l'utilisateur a saisi le nom de l'arc et non son num�ro
    {
        for(const auto& elem:m_trajets) //Parcours des trajets � la recherche de l'arc souhait�
        {
            if(saisie == elem.getNom())
            {
                m_plan.infoTrajet(elem, m_points);      //Affichage graphique des informations sur l'arc s�lectionn�
                trouve = true;
            }
        }
    }
    else        //Si l'int est diff�rent de 0 alors c'est que l'utilisateur a saisi le num�ro de l'arc et non son nom
    {
        for(const auto& elem:m_trajets)//Parcours des trajets � la recherche de l'arc souhait�
        {
            if(atoi(saisie.c_str()) == elem.getNum())       //conversion de la saisie en int
            {
                m_plan.infoTrajet(elem, m_points);        //Affichage graphique des informations sur l'arc s�lectionn�
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

    do          //Saisie s�curis�e du point de d�part
    {
        point1 = atoi(m_plan.saisie("Ou vous trouvez-vous ?", "Veuillez saisir le numero de depart :").c_str());
        if(point1<=0){m_plan.erreur("La location n'existe pas");}
        if(point1>37){m_plan.erreur("La location n'existe pas");}
    }while(point1<=0||point1>37);

    m_plan.effacer();
    m_plan.setup();

    do          //Saisie s�curis�e du point d'arriv�e
    {
        point2 = atoi(m_plan.saisie("Ou voulez allez ?", "Veuillez saisir le numero d'arrivee :").c_str());
        if(point2<=0){m_plan.erreur("La location n'existe pas");}
        if(point2>37){m_plan.erreur("La location n'existe pas");}
    }while(point2<=0||point2>37);
}


bool Station::selecArc(Trajet t)    //Permet de v�rifier si l'arc est s�lectionnable avec les crit�res actuels
{
    for(auto& elem:m_trajets)
    {
        if(t.getDepart() == elem.getDepart() && t.getArrivee() == elem.getArrivee() && t.getPoids() == elem.getPoids() && (elem.getSelec()==true))
        {
            return true;
        }
    }
    return false;
}

void Station::preselec(int presel)
{
    resetAttributs();
    int point1 = 0, point2 = 0;
    switch(presel)
    {
    case 1:
    {
        for(auto& elem:m_trajets)       //On supprime certain types de trajets
        {
            std::string type = elem.getType();
            if(type == "N" || type =="KL" || type =="SURF")
            {
                elem.setSelec(false);
            }
        }
        m_ininteret.push_back("R");         //on fait en sorte que ces trajets ne soient pas prioritaire lors du dijkstra
        saisieDijkstra(point1, point2);
        if(bfs(point1, point2))
        {
            dijkstra(point1, point2);
        }
        break;
    }
    case 2:
        {
            for(auto& elem:m_trajets)       //On supprime certain types de trajets
            {
                std::string type = elem.getType();
                if(type =="KL")
                {
                    elem.setSelec(false);
                }
            }
            m_ininteret.push_back("N");         //on fait en sorte que ces trajets ne soient pas prioritaire lors du dijkstra
            m_ininteret.push_back("SURF");
            saisieDijkstra(point1, point2);
            if(bfs(point1, point2))
            {
                dijkstra(point1, point2);
            }
            break;
        }
    case 3:
        {
            resetAttributs();
            m_ininteret.push_back("B");         //on fait en sorte que ces trajets ne soient pas prioritaire lors du dijkstra
            m_ininteret.push_back("BUS");
            saisieDijkstra(point1, point2);
            if(bfs(point1, point2))
            {
                dijkstra(point1, point2);
            }
            break;
        }
    case 4:
        {

            for(auto& elem:m_trajets)           //On supprime certain types de trajets
            {
                std::string type = elem.getType();
                if(type =="BUS" || type == "TC" || type == "TPH")
                {
                    elem.setSelec(false);
                }
            }
            m_ininteret.push_back("TSD");           //on fait en sorte que ces trajets ne soient pas prioritaire lors du dijkstra
            m_ininteret.push_back("TS");
            m_ininteret.push_back("TK");
            saisieDijkstra(point1, point2);
            if(bfs(point1, point2))
            {
                dijkstra(point1, point2);
            }
            break;
        }
    }
}

void Station::personnalise()
{
    std::vector<std::pair<std::string, int>> resultat;

    int taille = 0;

    std::ifstream ifs{"preferences.txt"};         //Lecture du fichier preferences
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture preferences.txt" );
    m_points.push_back(Point(0, "FakeNum0", 0, -815, -815));
    ifs >> taille;                     //Lecture du nombre de types
    for(int i=0; i<taille;i++)
    {
        std::string type;
        int val;
        ifs >> type >> val;
        resultat.push_back({type, val});
    }

    bool quitter = false;
    while(!quitter)
    {
        quitter = m_plan.personnalise(resultat);    //Attente de la fin de la s�lection de l'utilisateur
    }
    for(auto& trajets: m_trajets)
    {
        std::string type = trajets.getType();
        for(const auto& elem: resultat)
        {
            if(elem.second == 2)        //Si on double clique sur l'�lement alors le trajet ne sera pas emprunt�
            {
                if(type == elem.first)
                {
                    trajets.setSelec(false);
                }
                m_ininteret.push_back(elem.first);
            }
        }
    }
    for(const auto& elem: resultat)
    {
        if(elem.second == 1)            //Si on clique sur l'�lement alors on �vitera d'emprunter ce trajet
        {
            m_ininteret.push_back(elem.first);
        }
    }

    std::ofstream fichier("preferences.txt");
    if(fichier)//ecriture du fichier preferences
    {
        fichier << resultat.size() << std::endl;
        for(auto& elem: resultat)
        {
            fichier << elem.first << "\t" << elem.second << std::endl;
        }
    }
    else
    {
        m_plan.erreur("Erreur ouverture du fichier impossible");
    }


    int point1 = 0, point2 = 0;
    saisieDijkstra(point1, point2);
    if(bfs(point1, point2))
    {
        dijkstra(point1, point2);   //Parcourt d'un chemin et calcul du temps de ce dernier pour les crit�res s�lectionn�s
    }
}

void Station::adminPanel(bool simple)
{
    lectureFichiers();          //Re-lecture des derni�res valeurs dans les fichiers
    bool fin = false;

    if(simple)
    {
        std::vector<std::pair<std::string, bool>> resultat;
        bool b = false, r = false, n = false, kl = false, surf = false;
        bool tph = false, tc = false, tsd = false, ts = false, tk = false, bus = false;
        for(const auto& elem:m_trajets)         //Permet de savoir si les types de trajets sont ouverts ou ferm�s
        {
            if(elem.getType()=="B")
            {
                if(elem.getActive()) b = true;
            }
            if(elem.getType()=="R")
            {
                if(elem.getActive()) r = true;
            }
            if(elem.getType()=="N")
            {
                if(elem.getActive()) n = true;
            }
            if(elem.getType()=="KL")
            {
                if(elem.getActive()) kl = true;
            }
            if(elem.getType()=="SURF")
            {
                if(elem.getActive()) surf = true;
            }
            if(elem.getType()=="TPH")
            {
                if(elem.getActive()) tph = true;
            }
            if(elem.getType()=="TC")
            {
                if(elem.getActive()) tc = true;
            }
            if(elem.getType()=="TSD")
            {
                if(elem.getActive()) tsd = true;
            }
            if(elem.getType()=="TS")
            {
                if(elem.getActive()) ts = true;
            }
            if(elem.getType()=="TK")
            {
                if(elem.getActive()) tk = true;
            }
            if(elem.getType()=="BUS")
            {
                if(elem.getActive()) bus = true;
            }
        }
        resultat.push_back({"B",b});
        resultat.push_back({"R",r});
        resultat.push_back({"N",n});
        resultat.push_back({"KL",kl});
        resultat.push_back({"SURF",surf});
        resultat.push_back({"TPH",tph});
        resultat.push_back({"TC",tc});
        resultat.push_back({"TSD",tsd});
        resultat.push_back({"TS",ts});
        resultat.push_back({"TK",tk});
        resultat.push_back({"BUS",bus});
        while(!fin)
        {
            fin = m_plan.pannelSimple(resultat);    //Attente de la s�lection de l'administrateur pour l'ouverture et fermeture de certains types de trajets
        }
        for(auto& elem: m_trajets)          //Permet d'ouvrir ou ferm� certains types de trajets
        {
            if(elem.getType()=="B")
            {
                elem.setActive(resultat[0].second);
            }
            if(elem.getType()=="R")
            {
                elem.setActive(resultat[1].second);
            }
            if(elem.getType()=="N")
            {
                elem.setActive(resultat[2].second);
            }
            if(elem.getType()=="KL")
            {
                elem.setActive(resultat[3].second);
            }
            if(elem.getType()=="SURF")
            {
                elem.setActive(resultat[4].second);
            }
            if(elem.getType()=="TPH")
            {
                elem.setActive(resultat[5].second);
            }
            if(elem.getType()=="TC")
            {
                elem.setActive(resultat[6].second);
            }
            if(elem.getType()=="TSD")
            {
                elem.setActive(resultat[7].second);
            }
            if(elem.getType()=="TS")
            {
                elem.setActive(resultat[8].second);
            }
            if(elem.getType()=="TK")
            {
                elem.setActive(resultat[9].second);
            }
            if(elem.getType()=="BUS")
            {
                elem.setActive(resultat[10].second);
            }
        }
    }
    else
    {
        int page = 1;
        while(!fin)
        {
            fin = m_plan.pannelAdvance(m_trajets, page);    //Attente de la s�lection individuelle par trajet de l'administrateur pour leur ouverture/fermeture
        }
    }
    reecritureFichiers();           //Sauvegarde sur les fichiers des nouvelles valeurs
}

void Station::lectureFichiers()
{
    std::string fichier = "data_arcs.txt";
    while(!m_trajets.empty())
    {
        m_trajets.pop_back();
    }
    while(!m_points.empty())
    {
        m_points.pop_back();
    }
    while(!m_ininteret.empty())
    {
        m_ininteret.pop_back();
    }
    std::ifstream ifs{fichier};         //Lecture du fichier
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + fichier );
    m_points.push_back(Point(0, "FakeNum0", 0, -815, -815));
    ifs >> m_ordre;                     //Lecture du nombre de points
    for(int i=0; i<m_ordre;i++)         //Cr�ation des points
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
        bool active;
        ifs>>num>>nom>>type>>d>>a>>active;
        Trajet trj(num, nom, type, d, a, m_points[d].getAlt(), m_points[a].getAlt());
        if(active)
        {
            m_points[d].ajoutSuiv(trj);
            m_points[a].ajoutAnte(trj);
            trj.setActive(true);
        }
        else                            //Activation ou d�sactivation des trajets
        {
            trj.setSelec(false);
            trj.setActive(false);
        }
        m_trajets.push_back(trj);
    }
    m_testAffichage = true;
    m_affichageBfs = false;         //Variable de test d'affichage graphique
}

void Station::reecritureFichiers()
{
    std::ofstream fichier("data_arcs.txt");
    if(fichier)//ecriture du fichier data_arcs
    {
        fichier << m_points.size()-1<< std::endl;//ligne 1 : ordre du graphe
        for(unsigned int i = 1; i<m_points.size(); i++)
        {   //Puis chaque point
            fichier << m_points[i].getNum() << "\t" << m_points[i].getLieu() << "\t" << m_points[i].getAlt() << "\t" << m_points[i].x() << "\t" << m_points[i].y() << std::endl;
        }
        fichier << m_trajets.size() << std::endl;   //puis la taille du graphe
        for(unsigned int i = 0; i<m_trajets.size(); i++)
        {   //Puis chaque arc
            fichier << m_trajets[i].getNum() << "\t" << m_trajets[i].getNom() << "\t" << m_trajets[i].getType() << "\t" << m_trajets[i].getDepart() << "\t" << m_trajets[i].getArrivee() << "\t" << m_trajets[i].getActive() << std::endl;
        }
    }
    else
    {
        m_plan.erreur("Erreur ouverture du fichier impossible");
    }
}

void Station::setAffBfs(bool aff)
{
    m_affichageBfs = aff;       //Condition d'affichage graphique ou non du BFS
}
