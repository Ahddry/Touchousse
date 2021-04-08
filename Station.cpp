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
        bool active;
        ifs>>num>>nom>>type>>d>>a>>active;
        Trajet trj(num, nom, type, d, a, m_points[d].getAlt(), m_points[a].getAlt());
        ///std::cout<<num<<":"<<active<<std::endl;
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
        m_points[depart].setCouleur(1);    //Colorer en gris le point de départ
        std::queue<Point> file;            //file de traitement
        file.push(m_points[depart]);       //ajout du point de départ à la file
        while(!file.empty())
        {
            Point pointTemp = file.front();        //récupération de la tête de file
            for(auto& traj:pointTemp.getSuiv())     //parcourt de ses successeurs
            {
                if(selecArc(traj))
                {
                    int succ = traj.getArrivee();
                    if(m_points[succ].getCouleur()==0) //opération uniquement sur ceux blanc (non découverts
                    {
                        m_points[succ].setCouleur(1);  //passage du point découvert à gris
                        m_points[succ].setBfs(pointTemp.getNum());  //marque de son prédécesseur
                        file.push(m_points[succ]);     //ajout du point découvert à la file

                    }
                }
            }
            pointTemp.setCouleur(2);                //passage du point traité à noir
            file.pop();                     //suppression du point traité de la file

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
                elem.setSelectDijk(false);
                //std::cout<<"Mise a faux du point : "<<elem.getNum()<<std::endl;
            }

        }
        m_points[depart].setSelectDijk(true);
        if(m_points[arrivee].getBfs()==-1)
        {
            if(m_testAffichage) m_plan.erreur("Le point d'arrivee n'est pas atteignable avec les criteres actuels.");
            return false;
        }
        else
        {
            return true;
        }
    }
    if(m_testAffichage) m_plan.erreur("Le point d'arrivee n'est pas atteignable avec les criteres actuels.");
    return false;

}

class TestPoids
{
public:
    double operator() (const Trajet& t1, const Trajet& t2) //Définition de la comparaison d'une Arête à une autre pour la priority_queue
    {
        if(t1.getPoids()==t2.getPoids())
            return t1.getDepart() > t2.getDepart(); //En cas de même poids, tri par ordre croissant du nombre de départ
        return t1.getInteret() > t2.getInteret();       //Comparaison du poids d'une arête à celui d'une autre
    }
};

void Station::dijkstra(int depart, int arrivee)  ///Algorithme de Dijkstra
{
    for(auto& elem:m_points)                   //Mise à blanc des points
    {
        elem.setCouleur(0);
        if(!elem.getSelectDijk())
        {
            elem.setCouleur(2);
            //std::cout<<"Mise a NOIR du point : "<<elem.getNum()<<std::endl;
        }
    }
    m_points[depart].setDistance(0);       //Le point de départ a une distance de 0
    m_points[depart].setCouleur(1);        //et est marqué comme repéré
    m_points[0].setCouleur(2);             //on met le points 0 qui est fictif à 2
    bool fin = false;
    int boucle = 1;
    do
    {
        boucle++;
        int numS = -1;                      //numéro de point
        double dist = INT_MAX;                 //Distance la plus courte au point de départ
        for(auto& elem:m_points)
        {
            if(elem.getCouleur()==1 && elem.getDistance()<dist) //Récupération du point non marqué comme finit avec la distance la plus courte au point de départ
            {
                numS = elem.getNum();
                dist = elem.getDistance();
            }
        }
        for(auto& truc:m_points[numS].getSuiv())               //Récupération des arcs suivant ce dernier
        {
            if(selecArc(truc))
            {
                double poids = truc.getPoids(), cste = 1;
                for(const auto& type: m_ininteret)
                {
                    if(truc.getType() == type) cste = 150.4;
                }
                poids *= cste;
                int z = truc.getArrivee();
                if(m_points[z].getCouleur()==0)                    //Si leurs points d'arrivée ne sont pas encore marqués
                {
                    m_points[z].setDijk(numS);//Ajout comme antécédant
                    m_points[z].setDistance(poids+dist); //Attribution de la distance au départ
                    m_points[z].setCouleur(1);                     //Marquage du point
                }
                else if(m_points[z].getDistance()>(dist+poids))  //S'ils sont marqués mais que leur distance doit être mise à jour
                {
                    m_points[z].setDijk(numS);    //Remplacement de l'antécédant
                    m_points[z].setDistance(poids+dist);     //mise à jour de la distance

                }
            }
        }
        m_points[numS].setCouleur(2);                          //Marquage du point actuel comme finit
        fin = true;
        for(const auto& elem:m_points)
        {
            if(elem.getCouleur() != 2) fin = false;             //Vérification de si tous les points ont étés marqués ou non.
        }
    }while(!fin);
    int ante = m_points[arrivee].getDijk();

    for(auto& elem:m_points)
    {
        elem.setSelectDijk(false);
    }

    m_points[arrivee].setSelectDijk(true);
    double duree = 0;
    m_plan.setup();
    int actuel;
    while(true)
    {
        if(ante!=(-1))                      //Affichage du point d'arrivé et du chemin parcourut
        {
            m_points[ante].setSelectDijk(true);
            ante= m_points[ante].getDijk();
        }
        else break;
    }
    ante = arrivee;
    for(auto& elem: m_points)
    {
        if(m_points[ante].getSelectDijk() && ante!=-1)                      //Affichage du point d'arrivé et du chemin parcourut
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

            std::priority_queue<Trajet, std::vector<Trajet>, TestPoids> file; //File de priorité des arêtes pondérées, triées par ordre croissant de point d'arrivée
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

                        duree+=t.getPoids();
                        m_plan.trajet(t,m_points[t.getDepart()], m_points[t.getArrivee()],0);
                        affiche = true;
                    }
                    else file.pop();
                }
            }
        }
        elem.setBfs(-1);            //Pour éviter un warning sur la non utilisation de la varible "elem" -> sans effet reel.
    }
    std::cout<<"Duree poids : " + std::to_string(m_points[arrivee].getDistance())<<std::endl;
    std::cout<<"Duree reele : " + std::to_string(duree)<<std::endl;
    m_plan.descripPistes();
    m_plan.afficher();
    m_plan.standby();
    m_plan.emphase("La duree moyenne de ce chemin est de", m_plan.cutDouble(duree)+" minutes");
    m_plan.standby();
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

void Station::fordFulkerson(int depart, int arrivee)//Algorithme de Ford-Fulkerson pour déterminer le flot horaire maximal de skieurs entre deux points
{
    m_plan.setup();
    m_testAffichage = false;
    int flowMax = 0, flowMin = 0;
    int anteBfs, compteur = 0;
    std::priority_queue<Trajet, std::vector<Trajet>, Comparaison> file; //File de priorité des arêtes pondérées, triées par ordre croissant de point d'arrivée
    std::vector<Trajet> testDoublons;
    std::vector<Trajet> testDoublons2;
    std::vector<Trajet> sature;
    m_plan.point(m_points[depart]);
    while(bfs(depart,arrivee))
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
                        int capres = elem.getCapacite()-elem.getFlux();
                        flowMin = std::min(flowMin, capres);
                        testDoublons.push_back(elem);
                        m_plan.point(m_points[i]);
                    }
                }
            }

        }
        for(int i = arrivee; i!=depart; i = anteBfs)
        {
            anteBfs = m_points[i].getBfs();
            if(anteBfs!=(-1))
            {
                for(auto& elem:m_trajets)
                {
                    if(elem.getDepart() == anteBfs && elem.getArrivee() == i && elem.getSelec())
                    {
                        elem.setFlux(elem.getFlux()+ flowMin);
                        if(elem.getFlux()>=elem.getCapacite())
                        {
                            elem.setSelec(false);
                            sature.push_back(elem);
                        }
                    }

                }
            }

        }
        flowMax += flowMin;
        resetBfs();
    }
    std::vector<std::pair<int,int>> arrivees;
    for(auto& elem :testDoublons)
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
    while(!file.empty())
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
    for(auto& elem:sature)
    {
        elem.setSelec(true);
        m_plan.trajet(elem, m_points[elem.getDepart()], m_points[elem.getArrivee()], 0);
        m_plan.point(m_points[elem.getDepart()]);
        m_plan.point(m_points[elem.getArrivee()]);
    }
    m_plan.emphase("Ces trajets sont satures", "Il faudrait si possible augmenter leur capacite horaire");
    m_plan.standby();
    m_testAffichage = true;
}


void Station::resetAttributs()
{
    for(auto& elem:m_points)
    {
        elem.setDistance(INT_MAX);  //Initialisation de la distance comme étant maximale (infinie) au point de départ
        elem.setCouleur(0);         //Mise à l'état 0 de la couleur
        elem.setDijk(-1);           //Mise à -1 (aucun) de l'antécédant
        elem.setBfs(-1);            //Mise à -1 (aucun) de l'antécédant
        elem.setSelectDijk(true);   //Mise à vrai du critère de sélection
    }
    for(auto& elem:m_trajets)
    {
        elem.setSelec(true);
        elem.setInteret(true);
        elem.setFlux(0);
        if(!elem.getActive())
        {
            elem.setSelec(false);
        }
    }
    while(!m_ininteret.empty())
    {
        m_ininteret.pop_back();
    }
}

void Station::resetBfs()
{
    for(auto& elem:m_points)
    {
        elem.setCouleur(0);         //Mise à l'état 0 de la couleur
        elem.setBfs(-1);            //Mise à -1 (aucun) de l'antécédant
    }
}

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


bool Station::selecArc(Trajet t)
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
        for(auto& elem:m_trajets)
        {
            std::string type = elem.getType();
            if(type == "N" || type =="KL" || type =="SURF")
            {
                elem.setSelec(false);
            }
        }
        m_ininteret.push_back("R");
        saisieDijkstra(point1, point2);
        if(bfs(point1, point2))
        {
            dijkstra(point1, point2);
        }
        break;
    }
    case 2:
        {
            for(auto& elem:m_trajets)
            {
                std::string type = elem.getType();
                if(type =="KL")
                {
                    elem.setSelec(false);
                }
            }
            m_ininteret.push_back("N");
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
            m_ininteret.push_back("B");
            m_ininteret.push_back("BUS");
            saisieDijkstra(point1, point2);
            if(bfs(point1, point2))
            {
                dijkstra(point1, point2);
            }
            break;
        }
    case 4:
        {///CA CRASH POUR L'INSTANT

            for(auto& elem:m_trajets)
            {
                std::string type = elem.getType();
                if(type =="BUS" || type == "TC" || type == "TPH")
                {
                    elem.setSelec(false);
                }
            }
            m_ininteret.push_back("TSD");
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
    resultat.push_back({"V",0});
    resultat.push_back({"B",0});
    resultat.push_back({"R",0});
    resultat.push_back({"N",0});
    resultat.push_back({"KL",0});
    resultat.push_back({"SURF",0});
    resultat.push_back({"TPH",0});
    resultat.push_back({"TC",0});
    resultat.push_back({"TSD",0});
    resultat.push_back({"TS",0});
    resultat.push_back({"TK",0});
    resultat.push_back({"BUS",0});
    bool quitter = false;
    while(!quitter)
    {
        quitter = m_plan.personnalise(resultat);
    }
    for(auto& trajets: m_trajets)
    {
        std::string type = trajets.getType();
        for(const auto& elem: resultat)
        {
            if(elem.second == 2)
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
        //std::cout<<elem.first<<" : "<<elem.second<<std::endl;
        if(elem.second == 1)
        {
            m_ininteret.push_back(elem.first);
        }
    }
    int point1 = 0, point2 = 0;
    saisieDijkstra(point1, point2);
    if(bfs(point1, point2))
    {
        dijkstra(point1, point2);
    }
}

void Station::adminPanel(bool simple)
{
    lectureFichiers();
    bool fin = false;

    if(simple)
    {
        std::vector<std::pair<std::string, bool>> resultat;
        bool b = false, r = false, n = false, kl = false, surf = false;
        bool tph = false, tc = false, tsd = false, ts = false, tk = false, bus = false;
        for(const auto& elem:m_trajets)
        {
            if(elem.getType()=="B")
            {
                if(elem.getActive()) b = true;
                std::cout<<elem.getActive()<<std::endl;
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
            fin = m_plan.pannelSimple(resultat);
        }

        for(const auto& elem:resultat)
            std::cout<<elem.first<<" : "<<elem.second<<std::endl;

        for(auto& elem: m_trajets)
        {
            if(elem.getType()=="B")
            {
                std::cout<<elem.getActive()<<"\t";
                elem.setActive(resultat[0].second);
                std::cout<<elem.getActive()<<std::endl;
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
        for(const auto& elem:m_trajets)
        {
                if(elem.getType()=="B")
                {
                    std::cout<<elem.getActive()<<std::endl;
                }
        }
    }
    else
    {
        int page = 1;
        while(!fin)
        {
            fin = m_plan.pannelAdvance(m_trajets, page);
        }
    }
    reecritureFichiers();
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
}

void Station::reecritureFichiers()
{
    std::ofstream fichier("data_arcs.txt");
    if(fichier)//ecriture du fichier data_arcs
    {
        fichier << m_points.size()-1<< std::endl;//ligne 1 pseudo
        for(unsigned int i = 1; i<m_points.size(); i++)
        {
            fichier << m_points[i].getNum() << "\t" << m_points[i].getLieu() << "\t" << m_points[i].getAlt() << "\t" << m_points[i].x() << "\t" << m_points[i].y() << std::endl;
        }
        fichier << m_trajets.size() << std::endl;
        for(unsigned int i = 0; i<m_trajets.size(); i++)
        {
            fichier << m_trajets[i].getNum() << "\t" << m_trajets[i].getNom() << "\t" << m_trajets[i].getType() << "\t" << m_trajets[i].getDepart() << "\t" << m_trajets[i].getArrivee() << "\t" << m_trajets[i].getActive() << std::endl;
        }
    }
    else
    {
        m_plan.erreur("Erreur ouverture du fichier impossible");
    }
}

