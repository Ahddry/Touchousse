#include "Plan_Pistes.h"
#include "time.h"
#include <ctime>
#include "math.h"
#include <iostream>

#define ROUGE makecol(255,0,0)
#define VERT makecol(0,255,0)
#define BLEU makecol(0,0,255)
#define NOIR makecol(0,0,0)

#define ROUGE_DOUX makecol(255,50,50)
#define ROUGE_FONCE makecol(162,0,24)
#define VERT_FONCE makecol(14, 209, 69)
#define SOUS_TITRE makecol(112,146,190)
#define MONTAGNE makecol(200,210,220)


Plan_Pistes::Plan_Pistes(){}

Plan_Pistes::Plan_Pistes(std::string init)///Initialisation du mode graphique avec tout ses pr�requis
{
    ///Initialisation


    /// Lancer allegro et le mode graphique
    allegro_init();
    install_keyboard();
    install_mouse();
    register_png_file_type();
    jpgalleg_init();
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,1600,900,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    ///chargement des images
    m_background = load_bitmap_check("Graphics/Background.jpg");
    m_panneau = load_bitmap_check("Graphics/Panneau2.jpg");

    ///Lancement de la musique d'arri�re plan
    //m_musique = load_midi("Musique.mid");
    //play_midi(m_musique, true);

    /// Allocation dynamique d'une structure BITMAP de 1600 par 900
    m_plan=create_bitmap(1600,900);
    blit(m_background,m_plan,0,0,0,0,SCREEN_W,SCREEN_H);
}

Plan_Pistes::~Plan_Pistes(){}

void Plan_Pistes::setup()///affichage des �l�ments de base d'une partie
{
    blit(m_background,m_plan,0,0,0,0,SCREEN_W,SCREEN_H);
    //blit(m_panneau,m_plan,0,0,40,0,SCREEN_W,SCREEN_H);

    //FONT *Font1 = load_font("Fonts/Font1.pcx",NULL,NULL);
    //FONT *old = load_font("Fonts/Old.pcx",NULL,NULL);
    rectfill(m_plan, 20, 20, SCREEN_W-20, SCREEN_H-20, NOIR); //rectangle horizontal
    rectfill(m_plan, 30, 10, SCREEN_W-30, SCREEN_H-10, NOIR); //rectangle vertical
    circlefill(m_plan, SCREEN_W-35, 25, 15, NOIR);
    circlefill(m_plan, 35, SCREEN_H-25, 15, NOIR);
    circlefill(m_plan, SCREEN_W-35, SCREEN_H-25, 15, NOIR);
    circlefill(m_plan, 35, 25, 15, NOIR);
    floodfill(m_plan, SCREEN_W/2, 23, NOIR);
    rectfill(m_plan, 27.5, 25, SCREEN_W-27.5, SCREEN_H-25, MONTAGNE);
    rectfill(m_plan, SCREEN_W/2-80, SCREEN_H-15, SCREEN_W/2+80, SCREEN_H, NOIR);
    afficher();
}


BITMAP * Plan_Pistes::load_bitmap_check(std::string nomImage)///M�thode du chargement d'une image en v�rifiant son existance, obtenue sur le site de M. Fercoq
{
    BITMAP *bmp;
    bmp=load_bitmap(nomImage.c_str(),NULL);
    if (!bmp)
    {
        std::string msg = "pas pu trouver "+nomImage;
        allegro_message(msg.c_str());
        exit(EXIT_FAILURE);
    }
    return bmp;
}

std::vector<std::string> Plan_Pistes::text(std::string str)/// http://www.cplusplus.com/reference/string/string/substr/
{                                                      ///M�thode permettant de s�parer une cha�ne de charact�res en 2 aux endroits comprenant un #, recycl�e de notre projet pr�c�dent
    std::size_t pos = str.find("# ");      // position de "# " dans str, soit l'emplacement ou l'on coupe en deux notre cha�ne de charact�res
    std::string str1 = str.substr(0, pos);
    std::string str2 = str.substr(pos+1);
    std::vector<std::string> vec = {str1, str2};
    return vec;
}

void Plan_Pistes::afficher()///affichages des �l�ments du buffer
{                       ///obtenue sur le site de M. Fercoq
    int x,y;
    x=(SCREEN_W-m_plan->w)/2;
    y=(SCREEN_H-m_plan->h)/2;

    install_mouse();
    show_mouse(screen);

    // Affichage de la Bitmap sur l'�cran � ces coordonn�es
    blit(m_plan, screen, 0,0, x, y, m_plan->w, m_plan->h);
}

void Plan_Pistes::descripPistes()
{
    std::vector<Trajet> traj;
    traj.push_back(Trajet(0, "", "V", 0, 0, 0, 0));
    traj.push_back(Trajet(0, "", "B", 0, 0, 0, 0));
    traj.push_back(Trajet(0, "", "R", 0, 0, 0, 0));
    traj.push_back(Trajet(0, "", "N", 0, 0, 0, 0));
    traj.push_back(Trajet(0, "", "KL", 0, 0, 0, 0));
    traj.push_back(Trajet(0, "", "SURF", 0, 0, 0, 0));

    int x = SCREEN_W-275, y = 55;
    for(const auto& elem:traj)
    {
        entreDeux(elem.getDepart(), elem.getArrivee(), elem.getType(), x, y, false);
        y+=30;
    }
    while(!traj.empty())
    {
        traj.pop_back();
    }
    traj.push_back(Trajet(0, "", "TPH", 0, 0, 0, 0));
    traj.push_back(Trajet(0, "", "TC", 0, 0, 0, 0));
    traj.push_back(Trajet(0, "", "TSD", 0, 0, 0, 0));
    traj.push_back(Trajet(0, "", "TS", 0, 0, 0, 0));
    traj.push_back(Trajet(0, "", "TK", 0, 0, 0, 0));
    traj.push_back(Trajet(0, "", "BUS", 0, 0, 0, 0));
    y = 700;
    for(const auto& elem:traj)
    {
        entreDeux(elem.getDepart(), elem.getArrivee(), elem.getType(), x, y, false);
        y+=32;
    }
}

void Plan_Pistes::point(Point p)            //affichage d'un point sur le buffer
{
    int x = p.x(), y = p.y();
    FONT *old = load_font("Fonts/regles.pcx",NULL,NULL);
    circle(m_plan, x, y, 25, NOIR);
    textout_centre_ex(m_plan, old, std::to_string(p.getNum()).c_str(), x, y-15, NOIR, -1);
    //afficher();
}

void Plan_Pistes::trajet(Trajet t, Point depart, Point arrivee, int compteur)   //affichage d'un arc sur le buffer
{
    int couleur = 0;
    std::string type = t.getType();
    if(type=="V") couleur = VERT;
    else if(type=="B") couleur = BLEU;
    else if(type=="R") couleur = ROUGE;
    else if(type=="N") couleur = NOIR;
    else if(type=="KL") couleur = makecol(255, 0, 255);
    else if(type=="SURF") couleur = makecol(0, 255, 255);
    else if(type=="TPH") couleur = makecol(0, 128, 25);
    else if(type=="TC") couleur = makecol(165, 73, 164);
    else if(type=="TSD") couleur = NOIR;
    else if(type=="TS") couleur = makecol(255, 125, 0);
    else if(type=="TK") couleur = makecol(185, 122, 87);
    else if(type=="BUS") couleur = makecol(255, 255, 0);

    int x1 = depart.x();
    int y1 = depart.y();
    int x2 = arrivee.x();
    int y2 = arrivee.y();


    int delta = 25;

    int polaireX1 = delta * cos(compteur*M_PI/6);
    int polaireY1 = delta * sin(compteur*M_PI/6);
    int polaireX2 = delta * cos((compteur-1)*M_PI/4);
    int polaireY2 = delta * sin((compteur-1)*M_PI/4);

    if(t.getSelec())
    {
        if(x1>x2 && y1>y2)
            //line(m_plan, x1-delta-compteur, y1-delta-compteur, x2+delta-compteur, y2+delta-compteur, couleur);
            trait(type, x1-polaireX1, y1-polaireY1, x2+polaireX2, y2+polaireY2, couleur);
        else if(x1<x2 && y1>y2)
            //line(m_plan, x1+delta-compteur, y1-delta-compteur, x2-delta-compteur, y2+delta-compteur, couleur);
            trait(type, x1+polaireX1, y1-polaireY1, x2-polaireX2, y2-polaireY2, couleur);
        else if(x1<x2 && y1<y2)
            //line(m_plan, x1+delta-compteur, y1+delta-compteur, x2-delta-compteur, y2-delta-compteur, couleur);
            trait(type, x1+polaireX1, y1-polaireY1, x2-polaireX2, y2+polaireY2, couleur);
        else if(x1>x2 && y1<y2)
            //line(m_plan, x1-delta+compteur, y1+delta+compteur, x2+delta+compteur, y2-delta+compteur, couleur);
            trait(type, x1-polaireX1, y1+polaireY1, x2+polaireX2, y2+polaireY2, couleur);
    }



}

void Plan_Pistes::trait(std::string type, int x1, int y1, int x2, int y2, int couleur)
{
    if(type=="V"||type=="B"||type=="R"||type=="N"||type=="KL"||type=="SURF"||type=="BUS")
    {
        line(m_plan, x1, y1, x2, y2, couleur);
        rectfill(m_plan, x2-2, y2-2, x2+2, y2+2, couleur);
    }
    else if(type=="TSD"||type=="TS" || type=="TPH" || type=="TC" || type=="TK")
    {
        line(m_plan, x1, y1, x2, y2, couleur);
        for(int i=15; i>0;i--)
        {
            circlefill(m_plan, x1+i*(x2-x1)/16,y1+i*(y2-y1)/16, 2, couleur);
        }
        rectfill(m_plan, x2-2, y2-2, x2+2, y2+2, makecol(0, 195, 195));
    }
}


Point Plan_Pistes::selecPoint(std::vector<Point> ensemble) const
{
    std::vector<std::pair<int, int>> coords;
    for(const auto& elem:ensemble)
    {
        coords.push_back({elem.x(), elem.y()});
    }
    //rect(m_plan, x-25, y-25, x+25, y+25, ROUGE);

    int x = 0;
    int y = 0;

    int numero = 0;
    bool clic = false, fin = false;
    while(!fin)
    {
        clic = false;
        while(!clic)
        {
            if(mouse_b&1)
            {
                x = mouse_x;
                y = mouse_y;
                clic = true;
            }
        }
        for(unsigned int i = 0; i<coords.size(); i++)
        {
            if (x>=coords[i].first-25 && y>=coords[i].second-25 && x<=coords[i].first+25 && y<=coords[i].second+25)
            {
                numero = i;
                rect(m_plan, coords[i].first-25, coords[i].second-25, coords[i].first+25, coords[i].second+25, ROUGE);
                fin = true;
                break;
            }
        }
    }

    return ensemble[numero];
}

void Plan_Pistes::infoPoint(Point point)
{
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);
    FONT *old = load_font("Fonts/regles.pcx",NULL,NULL);
    effacer();
    setup();
    emphase("Informations sur le point", point.getLieu());
    rest(1200);
    setup();
    std::string lePoint = point.getLieu() + " - Altitude : " + std::to_string(point.getAlt()) + "m";
    //Titre centr�
    textout_centre_ex(m_plan, MiddleTitle,"Informations sur le point", SCREEN_W/2,40, ROUGE_DOUX,-1);
    textout_centre_ex(m_plan, SubTitle,lePoint.c_str(), SCREEN_W/2,120, SOUS_TITRE,-1);

    circle(m_plan, 125, 130, 50, NOIR);
    textout_centre_ex(m_plan, MiddleTitle, std::to_string(point.getNum()).c_str(), 125, 90, NOIR, -1);

    int x = 125;
    int y = 250;
    for(const auto& elem:point.getAnte())
    {
        entreDeux(elem.getDepart(), elem.getArrivee(), elem.getType(), x, y, true); ///TRIER LES ARETES SI YA LE TEMPS
        std::string poids = cutDouble(elem.getPoids());
        textout_ex(m_plan, old,(elem.getNom() + " : " + poids + "mins").c_str(), x+285, y-17, SOUS_TITRE,-1);
        y+=75;
    }
    x = SCREEN_W-375;
    y = 250;
    for(const auto& elem:point.getSuiv())
    {
        entreDeux(elem.getDepart(), elem.getArrivee(), elem.getType(), x, y, true);
        std::string poids = cutDouble(elem.getPoids());
        textout_right_ex(m_plan, old,(poids + "mins : " + elem.getNom()).c_str(), x-35, y-17, SOUS_TITRE,-1);
        y+=75;
    }

    afficher();
}

void Plan_Pistes::infoTrajet(Trajet traj, std::vector<Point> points)
{
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);
    effacer();
    setup();
    emphase("Informations sur le trajet", traj.getNom());
    rest(1200);
    setup();
    //Titre centr�
    std::string poids = cutDouble(traj.getPoids());
    std::string sousTitre = std::to_string(traj.getNum()) + " : " + traj.getNom();
    textout_centre_ex(m_plan, MiddleTitle,"Informations sur le trajet", SCREEN_W/2,40, ROUGE_DOUX,-1);
    textout_centre_ex(m_plan, SubTitle,sousTitre.c_str(), SCREEN_W/2,120, SOUS_TITRE,-1);
    entreDeux(traj.getDepart(), traj.getArrivee(), traj.getType(), 125, 250, true);
    std::string info1 = "Vous etes sur le trajet " + std::to_string(traj.getNum()) + " : " + traj.getNom();
    std::string info2 = "C'est un(e) " + longType(traj.getType()) + " qui relie les points " + points[traj.getDepart()].getLieu() + " et " + points[traj.getArrivee()].getLieu() + ".";
    std::string info3 = "La duree moyenne de ce trajet est de " + poids + " minutes.";
    textout_centre_ex(m_plan, SubTitle,info1.c_str(), SCREEN_W/2, SCREEN_H/2-50, SOUS_TITRE,-1);
    textout_centre_ex(m_plan, SubTitle,info2.c_str(), SCREEN_W/2, SCREEN_H/2, SOUS_TITRE,-1);
    textout_centre_ex(m_plan, SubTitle,info3.c_str(), SCREEN_W/2, SCREEN_H/2+50, SOUS_TITRE,-1);
}

void Plan_Pistes::entreDeux(int depart, int arrivee, std::string type, int x, int y, bool point)
{
    FONT *old = load_font("Fonts/regles.pcx",NULL,NULL);
    int couleur = 0;
    if(type=="V") couleur = VERT;
    else if(type=="B") couleur = BLEU;
    else if(type=="R") couleur = ROUGE;
    else if(type=="N") couleur = NOIR;
    else if(type=="KL") couleur = makecol(255, 0, 255);
    else if(type=="SURF") couleur = makecol(0, 255, 255);
    else if(type=="TPH") couleur = makecol(0, 128, 25);
    else if(type=="TC") couleur = makecol(165, 73, 164);
    else if(type=="TSD") couleur = NOIR;
    else if(type=="TS") couleur = makecol(255, 125, 0);
    else if(type=="TK") couleur = makecol(185, 122, 87);
    else if(type=="BUS") couleur = makecol(255, 255, 0);
    std::string typeLong = longType(type);

    if(point)
    {
        circle(m_plan, x, y, 25, NOIR);
        textout_centre_ex(m_plan, old, std::to_string(depart).c_str(), x, y-15, NOIR, -1);
        circle(m_plan, x+250, y, 25, NOIR);
        textout_centre_ex(m_plan, old, std::to_string(arrivee).c_str(), x+250, y-15, NOIR, -1);
    }
    trait(type, x+25, y, x+225, y, couleur);
    textout_centre_ex(m_plan, old, typeLong.c_str(), x+125, y-35, couleur, -1);
}

std::string Plan_Pistes::longType(std::string type) const
{
    std::string typeLong;
    if(type=="V") typeLong = "Piste Verte";
    else if(type=="B") typeLong = "Piste Bleue";
    else if(type=="R") typeLong = "Piste Rouge";
    else if(type=="N") typeLong = "Piste Noire";
    else if(type=="KL") typeLong = "Piste de km lance";
    else if(type=="SURF") typeLong = "Snow Park";
    else if(type=="TPH") typeLong = "Telepherique";
    else if(type=="TC") typeLong = "Tele-cabine";
    else if(type=="TSD") typeLong = "TS Debrayable";
    else if(type=="TS") typeLong = "Telesiege";
    else if(type=="TK") typeLong = "Teleski";
    else if(type=="BUS") typeLong = "BUS";
    return typeLong;
}

std::string Plan_Pistes::cutDouble(double nombre)
{
    std::string poids = std::to_string(nombre);
    poids.erase(poids.find_last_not_of('0')+1, std::string::npos); //https://stackoverflow.com/questions/13686482/c11-stdto-stringdouble-no-trailing-zeros
    poids.erase(poids.find_last_not_of('.')+1, std::string::npos); //https://stackoverflow.com/questions/13686482/c11-stdto-stringdouble-no-trailing-zeros
    return poids;
}


bool Plan_Pistes::personnalise(std::vector<std::pair<std::string,int>>& connex)
{
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);
    FONT *old = load_font("Fonts/Normal.pcx",NULL,NULL);
    effacer();
    setup();
    //Titre centr�
    textout_centre_ex(m_plan, MiddleTitle,"Selection personnalisee", SCREEN_W/2,40, ROUGE_DOUX,-1);
    textout_centre_ex(m_plan, SubTitle,"de votre trajet preferentiel", SCREEN_W/2,120, SOUS_TITRE,-1);

    std::string explication = "Cliquez sur les cases correspondantes aux types de trajets pour changer leurs presences dans votre trajet preferentiel.";
    textout_centre_ex(m_plan, old,explication.c_str(), SCREEN_W/2,200, SOUS_TITRE,-1);
    std::vector<std::pair<int, int>> coords;
    int couleur = 0;
    std::string texte = "";
    int x = 120, y = 320;
    unsigned int i = 0;
    for(const auto& elem:connex)
    {
        if(i==connex.size()/2)
        {
            x+=810;
            y=320;
        }
        entreDeux(1, 2, elem.first, x, y, true);
        int x2 = x+400;
        if(elem.second == 0)
        {
            couleur = VERT_FONCE;
            texte = "AVEC";
        }
        else if(elem.second == 1)
        {
            couleur = makecol(255, 127, 39);
            texte = "EVITER";
        }
        else if(elem.second == 2)
        {
            couleur = ROUGE;
            texte = "SANS";
        }
        rectfill(m_plan, x2-50, y-25, x2+150, y+25, couleur);
        textout_centre_ex(m_plan, old,texte.c_str(), x2+50, y-15, makecol(48,48,48),-1);
        coords.push_back({x2+50, y});
        y+=70;
        i++;
    }

    rectfill(m_plan, SCREEN_W/2-50, SCREEN_H-45, SCREEN_W/2+50, SCREEN_H, VERT_FONCE);
    textout_centre_ex(m_plan, old,"Confirmer", SCREEN_W/2,SCREEN_H-40, NOIR,-1);
    afficher();

    int xSouris = 0, ySouris = 0;
    bool clic = false, fin = false, quitter = false;
    while(!fin)
    {
        clic = false;
        while(!clic)
        {
            if(mouse_b&1)
            {
                xSouris = mouse_x;
                ySouris = mouse_y;
                clic = true;
            }
        }
        for(unsigned int i = 0; i<coords.size(); i++)
        {
            if (xSouris>=coords[i].first-100 && ySouris>=coords[i].second-25 && xSouris<=coords[i].first+100 && ySouris<=coords[i].second+25)
            {
                rect(m_plan, coords[i].first-25, coords[i].second-25, coords[i].first+25, coords[i].second+25, ROUGE);
                connex[i].second++;
                if(connex[i].second == 3)
                    connex[i].second = 0;
                fin = true;
                break;
            }
            if ( mouse_b&1 && mouse_x>=SCREEN_W/2-50 && mouse_y>=SCREEN_H-50 && mouse_x<=SCREEN_W/2+50 &&mouse_y<=SCREEN_H && !fin)
            {
                fin=true;
                quitter = true;
                break;
            }
        }
    }

    return quitter;
}

bool Plan_Pistes::pannelSimple(std::vector<std::pair<std::string,bool>>& connex)
{
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);
    FONT *old = load_font("Fonts/Normal.pcx",NULL,NULL);
    effacer();
    setup();
    //Titre centr�
    textout_centre_ex(m_plan, MiddleTitle,"Ouverture/Fermeture des pistes", SCREEN_W/2,40, ROUGE_DOUX,-1);
    textout_centre_ex(m_plan, SubTitle,"panneau d'administration general", SCREEN_W/2,120, SOUS_TITRE,-1);
    std::string explication = "Cliquez sur les cases correspondantes aux types de trajets pour toutes les ouvrir ou fermer au public.";
    textout_centre_ex(m_plan, old,explication.c_str(), SCREEN_W/2,200, SOUS_TITRE,-1);
    std::vector<std::pair<int, int>> coords;
    int couleur = 0;
    std::string texte = "";
    int x = 120, y = 320;
    unsigned int i = 0;
    for(const auto& elem:connex)
    {
        if(i==connex.size()/2)
        {
            x+=810;
            y=320;
        }
        entreDeux(1, 2, elem.first, x, y, true);
        int x2 = x+400;
        if(elem.second == true)
        {
            couleur = VERT_FONCE;
            texte = "OUVERTES";
        }
        else if(elem.second == false)
        {
            couleur = ROUGE;
            texte = "FERMEES";
        }
        rectfill(m_plan, x2-50, y-25, x2+150, y+25, couleur);
        textout_centre_ex(m_plan, old,texte.c_str(), x2+50, y-15, makecol(48,48,48),-1);
        coords.push_back({x2+50, y});
        y+=70;
        i++;
    }

    rectfill(m_plan, SCREEN_W/2-50, SCREEN_H-45, SCREEN_W/2+50, SCREEN_H, VERT_FONCE);
    textout_centre_ex(m_plan, old,"Confirmer", SCREEN_W/2,SCREEN_H-40, NOIR,-1);
    afficher();

    int xSouris = 0, ySouris = 0;
    bool clic = false, fin = false, quitter = false;
    while(!fin)
    {
        clic = false;
        while(!clic)
        {
            if(mouse_b&1)
            {
                xSouris = mouse_x;
                ySouris = mouse_y;
                clic = true;
            }
        }
        for(unsigned int i = 0; i<coords.size(); i++)
        {
            if (xSouris>=coords[i].first-100 && ySouris>=coords[i].second-25 && xSouris<=coords[i].first+100 && ySouris<=coords[i].second+25)
            {
                rect(m_plan, coords[i].first-25, coords[i].second-25, coords[i].first+25, coords[i].second+25, ROUGE);
                if(connex[i].second == true)
                    connex[i].second = false;
                else connex[i].second = true;
                fin = true;
                break;
            }
            if ( mouse_b&1 && mouse_x>=SCREEN_W/2-50 && mouse_y>=SCREEN_H-50 && mouse_x<=SCREEN_W/2+50 &&mouse_y<=SCREEN_H && !fin)
            {
                fin=true;
                quitter = true;
                break;
            }
        }
    }

    return quitter;
}

bool Plan_Pistes::pannelAdvance(std::vector<Trajet>& trajets, int& page)
{
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);
    FONT *old = load_font("Fonts/Normal.pcx",NULL,NULL);
    effacer();
    setup();
    //Titre centr�
    textout_centre_ex(m_plan, MiddleTitle,"Ouverture/Fermeture des pistes", SCREEN_W/2,40, ROUGE_DOUX,-1);
    textout_centre_ex(m_plan, SubTitle,"panneau d'administration general", SCREEN_W/2,120, SOUS_TITRE,-1);
    std::string explication = "Cliquez sur les cases correspondantes aux types de trajets pour toutes les ouvrir ou fermer au public.";
    textout_centre_ex(m_plan, old,explication.c_str(), SCREEN_W/2,200, SOUS_TITRE,-1);
    std::vector<std::pair<int, int>> coords;
    int couleur = 0;
    std::string texte = "";
    int x = 120 - 1600*(page-1), y = 320;
    unsigned int i = 0;
    for(const auto& elem:trajets)
    {
        if(i%7==0 && i!=0)
        {
            x+=800;
            y=320;
        }
        textout_right_ex(m_plan, old,(std::to_string(elem.getNum()) + " :").c_str(), x-35, y-17, SOUS_TITRE,-1);
        entreDeux(elem.getDepart(), elem.getArrivee(), elem.getType(), x, y, true);
        std::cout<<"Affichage : "<<elem.getDepart()<<"->"<<elem.getArrivee()<<" "<<elem.getType()<<std::endl;
        int x2 = x+400;
        if(elem.getActive() == true)
        {
            couleur = VERT_FONCE;
            texte = "OUVERTES";
        }
        else if(elem.getActive() == false)
        {
            couleur = ROUGE;
            texte = "FERMEES";
        }
        rectfill(m_plan, x2-50, y-25, x2+150, y+25, couleur);
        textout_centre_ex(m_plan, old,texte.c_str(), x2+50, y-15, makecol(48,48,48),-1);
        coords.push_back({x2+50, y});
        y+=70;
        i++;
    }

    rectfill(m_plan, SCREEN_W/2-50, SCREEN_H-45, SCREEN_W/2+50, SCREEN_H, VERT_FONCE);
    textout_centre_ex(m_plan, old,"Confirmer", SCREEN_W/2,SCREEN_H-40, NOIR,-1);
    if(page>1)
    {
        rectfill(m_plan, 27.5, SCREEN_H-70, 185, SCREEN_H-25, makecol(255, 127, 39));
        textout_ex(m_plan, old,"Page precedente", 32,SCREEN_H-65, NOIR,-1);
    }
    if(page<7)
    {
        rectfill(m_plan, SCREEN_W-160, SCREEN_H-70, SCREEN_W-27.5, SCREEN_H-25, makecol(255, 127, 39));
        textout_right_ex(m_plan, old,"Page suivante", SCREEN_W-32,SCREEN_H-65, NOIR,-1);
    }
    afficher();

    int xSouris = 0, ySouris = 0;
    bool clic = false, fin = false, quitter = false;
    while(!fin)
    {
        clic = false;
        while(!clic)
        {
            if(mouse_b&1)
            {
                xSouris = mouse_x;
                ySouris = mouse_y;
                clic = true;
            }
        }
        for(unsigned int i = 0; i<coords.size(); i++)
        {
            if (xSouris>=coords[i].first-100 && ySouris>=coords[i].second-25 && xSouris<=coords[i].first+100 && ySouris<=coords[i].second+25)
            {
                rect(m_plan, coords[i].first-25, coords[i].second-25, coords[i].first+25, coords[i].second+25, ROUGE);
                if(trajets[i].getActive() == true)
                    trajets[i].setActive(false);
                else trajets[i].setActive(true);
                fin = true;
                break;
            }
            if ( mouse_b&1 && mouse_x>=27.5 && mouse_y>=SCREEN_H-70 && mouse_x<=185 &&mouse_y<=SCREEN_H-25 && !fin && page>1)
            {
                fin=true;
                page--;
                break;
            }
            if ( mouse_b&1 && mouse_x>=SCREEN_W-160 && mouse_y>=SCREEN_H-70 && mouse_x<=SCREEN_W-27.5 &&mouse_y<=SCREEN_H-25 && !fin && page<7)
            {
                fin=true;
                page++;
                break;
            }
            if ( mouse_b&1 && mouse_x>=SCREEN_W/2-50 && mouse_y>=SCREEN_H-50 && mouse_x<=SCREEN_W/2+50 &&mouse_y<=SCREEN_H && !fin)
            {
                fin=true;
                quitter = true;
                break;
            }
        }
    }

    return quitter;
}


void Plan_Pistes::emphase(std::string titre, std::string sousTitre)
{   ///Message accentu� avec une premi�re chose �crite en gros et une seconde en un peu plus petit en dessous
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);

    // Texte centr�
    textout_centre_ex(m_plan, MiddleTitle,titre.c_str(), SCREEN_W/2,SCREEN_H/2-100, ROUGE_DOUX,-1);
    textout_centre_ex(m_plan, SubTitle,sousTitre.c_str(), SCREEN_W/2,SCREEN_H/2-20, SOUS_TITRE,-1);

    afficher();
}

int Plan_Pistes::menu(std::string titre, std::vector<std::string> propositions, int nbChoix)///Menu divers avec choix param�trables, attendant un clic de l'utilisateur sur une des options propos�es
{
    //Polices utilis�es
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);

    setup();

    //Texte centr�
    textout_centre_ex(m_plan, MiddleTitle,titre.c_str(), SCREEN_W/2,SCREEN_H/4, ROUGE_DOUX,-1);
    int j=100;
    for(const auto& elem: propositions)
    {
        textout_centre_ex(m_plan, SubTitle,elem.c_str(), SCREEN_W/2,SCREEN_H/4+j, SOUS_TITRE,-1);
        j+=75;
    }

    // pour disposer de la souris
    install_mouse();

    // pour voir le pointeur de la souris
    show_mouse(m_plan);

    int choix = 0;

    bool fin = false;
    afficher();
    if(nbChoix>0)//Si il y a un chois � faire
    {
        while (!fin)    //Attente que l'utilisateur fasse son choix
        {
            if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+90 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+160)
            {
                choix = 1;//Premier choix
                fin = true;
            }
            if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+165 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+235)
            {
                choix = 2;//Deuxi�me choix
                fin = true;
            }
            if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+245 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+315 && nbChoix>2)
            {
                choix = 3;//Troisi�me choix
                fin = true;
            }
            if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+320 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+395 && nbChoix>3)
            {
                choix = 4;//Quatri�me choix
                fin = true;
            }
            if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+400 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+475 && nbChoix>4)
            {
                choix = 5;//Cinqui�me choix
                fin = true;
            }
        }
    }
    return choix;
}

std::string Plan_Pistes::saisie(std::string titre, std::string texte)///saisie similaire � un "std::getline(std::cin, variable)" de l'utilisateur, pour allegro
{                                                                ///obtenu en partie sur le site de M. Fercoq
    effacer();
    setup();
    std::string saisie;
    int poscarac=0; //position du charact�re pour son affichage
    bool fin = false;
    int touche;
    //Polices utilis�es
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);
    //Ce que l'on demande � l'utilisateur
    textout_centre_ex(m_plan, MiddleTitle,titre.c_str(), SCREEN_W/2,SCREEN_H/4, ROUGE_DOUX,-1);
    textout_centre_ex(m_plan, SubTitle,texte.c_str(), SCREEN_W/2,SCREEN_H/4+80, ROUGE_FONCE,-1);
    afficher();
    set_keyboard_rate(500,200); //pour �viter qu'un appui de touche fasse appara�tre plus de charact�res que d�sir�
    touche=readkey();
    for(unsigned int i=0; i<100; i++)
    {
        if(keypressed())
        {
            touche = readkey();
            saisie +=(char) touche;
            saisie = "";
        }
    }

    rest(50);
    char carac;                 //Charact�re saisi
    while (!fin)
    {

        // keypressed() : d�tection de touche appuy�e
        if (keypressed())
        {
            effacer();
            setup();
            textout_centre_ex(m_plan, MiddleTitle,titre.c_str(), SCREEN_W/2,SCREEN_H/4, ROUGE_DOUX,-1);
            textout_centre_ex(m_plan, SubTitle,texte.c_str(), SCREEN_W/2,SCREEN_H/4+80, ROUGE_FONCE,-1);
            // r�cup�rer la touche avec readkey()
            touche=readkey();
            carac = (char) touche;
            if(carac=='\r'||key[KEY_BACKSPACE]) //si l'utilisateur appuie sur un charact�re non support� ou veut effacer le dernier en appuyant sur la touche [BACKSPACE]
            {
                std::size_t pos = saisie.size(); //On enl�ve le dernier charact�re de la saisie
                saisie = saisie.substr(0, pos-1);
            }
            else
            {
                // a partir de l'info de touche on obtient le caract�re en castant en char
                saisie +=(char) touche;
            }
            textout_centre_ex(m_plan, SubTitle,saisie.c_str(), SCREEN_W/2,SCREEN_H/2, ROUGE_FONCE,-1);
            poscarac=poscarac+20;
            afficher();

        }
        if (key[KEY_ENTER]) //si l'utilisateur confirme sa saisie avec Entr�e
                fin=true;
    }
    effacer();
    return saisie;
}

void Plan_Pistes::effacer()///effacer tous les �l�ments actuellement en buffer
{                      ///Obtenu sur le site de M. Fercoq
    // Effacer contenu �ventuel qui traine en m�moire
    // -> tous les pixels sont mis � noir
    clear_bitmap(m_plan);
    blit(m_background,m_plan,0,0,0,0,SCREEN_W,SCREEN_H);
    //afficher();
}

void Plan_Pistes::erreur(std::string msg)///Message d'erreur en rouge au bas de l'�cran
{
    //Police
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);
    //Message
    rectfill(m_plan, SCREEN_W/2-700, SCREEN_H-220, SCREEN_W/2+700, SCREEN_H-160, MONTAGNE);
    textout_centre_ex(m_plan, SubTitle,msg.c_str(), SCREEN_W/2,SCREEN_H/4*3, ROUGE_DOUX,-1);
    afficher();
    rest(1500);
}

void Plan_Pistes::regles(std::vector<std::string> regles)//affichage des r�gles ou des cr�dits sur un panneau au dessus du menu principal
{
    effacer();
    //Chargement des polices
    FONT *old = load_font("Fonts/regles.pcx",NULL,NULL);
    FONT *MainTitle = load_font("Fonts/MainTitle.pcx",NULL,NULL);
    setup();
    //Affichage du titre
    textout_centre_ex(m_plan, MainTitle,"Touchousse", SCREEN_W/2,SCREEN_H/4-75, ROUGE_DOUX,-1);
    int j = 25, w = j;
    for(const auto& elem:regles)//Affichage du texte donn�
    {
        textout_centre_ex(m_plan, old,elem.c_str(), SCREEN_W/2, SCREEN_H/4+20+w, ROUGE_FONCE,-1);
        w+=j;
    }
    //Boutton retour
    rectfill(m_plan, 25, SCREEN_H-80, 125, SCREEN_H-25, ROUGE_FONCE);
    textout_centre_ex(m_plan, old,"Retour", 75,SCREEN_H-75, MONTAGNE,-1);
    bool fin = false;
    afficher();
    while(!fin)//Attends que l'utilisateur appuie sur retour ou sur sa touche [ECHAP]
    {
        if ( mouse_b&1 && mouse_x>=25 && mouse_y>=SCREEN_H-80 && mouse_x<=125 &&mouse_y<=SCREEN_H-25 && !fin)
        {
            fin=true;
            break;
        }
        if (key[KEY_ESC])
        {
            fin=true;
            break;
        }
    }
}


int Plan_Pistes::menuPrincipal()///Menu principal du jeu, attendant un clic de l'utilisateur sur une des options propos�es
{
    effacer();
    //Chargement des images
    BITMAP* baniere = load_bitmap_check("Graphics/Menu.bmp");
    BITMAP* arrow1 = load_bitmap_check("Graphics/arrow1.bmp");
    BITMAP* arrow2 = load_bitmap_check("Graphics/arrow2.bmp");
    blit(m_background,m_plan,0,0,0,0,SCREEN_W,SCREEN_H);
    masked_blit(baniere,m_plan,0,0,0,0,SCREEN_W,SCREEN_H);

    //Chargement des polices
    FONT *MainTitle = load_font("Fonts/MainTitle.pcx",NULL,NULL);
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);

    //Affichage du titre du jeu
    textout_centre_ex(m_plan, MainTitle,"TOUCHOUSSE", SCREEN_W/2,SCREEN_H/4-75, ROUGE_DOUX,-1);

    std::vector<std::string> propositions;//Ajout des choix � afficher
    propositions.push_back("Plan interactif");
    propositions.push_back("Infos Trajets");
    propositions.push_back("Plus court chemin");
    propositions.push_back("Choix chemin");
    propositions.push_back("Flux");
    propositions.push_back("Extras");
    propositions.push_back("Quitter");
    int j=50, choix;
    for(const auto& elem: propositions)//Affichage des choix
    {
        textout_centre_ex(m_plan, MiddleTitle,elem.c_str(), SCREEN_W/2,SCREEN_H/4+j, ROUGE_DOUX,-1);
        j+=85;
    }
    bool fin = false;
    afficher();
    while (!fin)//Attente que l'utilisateur fasse son choix
    {

        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+50 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+140)
        {
            choix = 1;//Lancer le plan interractif
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+55,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+55,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }

        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+145 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+225)
        {
            choix = 2;//Affichage infos trajets
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+140,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+140,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }

        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+230 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+310)
        {
            choix = 3;//Lance un Dijkstra
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+225,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+225,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }

        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+315 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+395)
        {
            choix = 4;//?????????????
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+310,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+310,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }

        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+400 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+480)
        {
            choix = 5;//Affichage des cr�dits
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+395,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+395,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }

        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+485 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+565)
        {
            choix = 6;//Les extras
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+480,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+480,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }

        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+570 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+650)
        {
            choix = 7;//Quitter
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+560,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+560,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
    }
    return choix;
}
int Plan_Pistes::menuExtras()///Menu des extensions de l'application, attendant un clic de l'utilisateur sur une des options propos�es
{
    effacer();
    //Chargement des images
    BITMAP* baniere = load_bitmap_check("Graphics/Menu.bmp");
    BITMAP* arrow1 = load_bitmap_check("Graphics/arrow1.bmp");
    BITMAP* arrow2 = load_bitmap_check("Graphics/arrow2.bmp");
    blit(m_background,m_plan,0,0,0,0,SCREEN_W,SCREEN_H);
    masked_blit(baniere,m_plan,0,0,0,0,SCREEN_W,SCREEN_H);

    //Chargement des polices
    FONT *MainTitle = load_font("Fonts/MainTitle.pcx",NULL,NULL);
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);

    //Affichage du titre du jeu
    textout_centre_ex(m_plan, MainTitle,"TOUCHOUSSE", SCREEN_W/2,SCREEN_H/4-75, ROUGE_DOUX,-1);

    std::vector<std::string> propositions;//Ajout des choix � afficher
    propositions.push_back("Fonctions Admin");
    propositions.push_back("Credits");
    propositions.push_back("Retour");
    int j=50, choix;
    for(const auto& elem: propositions)//Affichage des choix
    {
        textout_centre_ex(m_plan, MiddleTitle,elem.c_str(), SCREEN_W/2,SCREEN_H/4+j, ROUGE_DOUX,-1);
        j+=85;
    }
    bool fin = false;
    afficher();
    while (!fin)//Attente que l'utilisateur fasse son choix
    {

        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+50 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+140)
        {
            choix = 1;//Sous-menu de cr�ateur de carted
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+55,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+55,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+145 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+225)
        {
            choix = 2;//Affichage des 7 meilleurs joueurs du jeu
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+140,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+140,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+230 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+310)
        {
            choix = 3;//retour au menu principal
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+225,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+225,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
    }
    return choix;
}

int Plan_Pistes::menuPersonalisation()///Menu de personalisation des trajets propos�s, attendant un clic de l'utilisateur sur une des options propos�es
{
    effacer();
    //Chargement des images
    BITMAP* baniere = load_bitmap_check("Graphics/Menu.bmp");
    BITMAP* arrow1 = load_bitmap_check("Graphics/arrow1.bmp");
    BITMAP* arrow2 = load_bitmap_check("Graphics/arrow2.bmp");
    blit(m_background,m_plan,0,0,0,0,SCREEN_W,SCREEN_H);
    masked_blit(baniere,m_plan,0,0,0,0,SCREEN_W,SCREEN_H);

    //Chargement des polices
    FONT *MainTitle = load_font("Fonts/MainTitle.pcx",NULL,NULL);
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);

    //Affichage du titre du jeu
    textout_centre_ex(m_plan, MainTitle,"TOUCHOUSSE", SCREEN_W/2,SCREEN_H/4-75, ROUGE_DOUX,-1);

    std::vector<std::string> propositions;//Ajout des choix � afficher
    propositions.push_back("Pre-definis");
    propositions.push_back("Personnalises");
    propositions.push_back("Retour");
    int j=50, choix;
    for(const auto& elem: propositions)//Affichage des choix
    {
        textout_centre_ex(m_plan, MiddleTitle,elem.c_str(), SCREEN_W/2,SCREEN_H/4+j, ROUGE_DOUX,-1);
        j+=85;
    }
    bool fin = false;
    afficher();
    while (!fin)//Attente que l'utilisateur fasse son choix
    {

        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+50 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+140)
        {
            choix = 1;//Sous-menu de cr�ateur de carted
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+55,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+55,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+145 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+225)
        {
            choix = 2;//Affichage des 7 meilleurs joueurs du jeu
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+140,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+140,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+230 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+310)
        {
            choix = 3;//retour au menu principal
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-400,SCREEN_H/4+225,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+320,SCREEN_H/4+225,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
    }
    return choix;
}


void Plan_Pistes::standby()///attente que l'utilisateur clique sur fermer ou appuie sur [ECHAP]
{
    //Police
    FONT *old = load_font("Fonts/regles.pcx",NULL,NULL);
    //Boutton retour
    rectfill(m_plan, SCREEN_W/2-50, SCREEN_H-45, SCREEN_W/2+50, SCREEN_H, MONTAGNE);
    textout_centre_ex(m_plan, old,"Retour", SCREEN_W/2,SCREEN_H-40, ROUGE_DOUX,-1);

    bool fin = false;
    afficher();
    rest(250);
    while(!fin)//attente que l'utilisateur clique sur fermer ou appuie sur [ECHAP]
    {
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-50 && mouse_y>=SCREEN_H-50 && mouse_x<=SCREEN_W/2+50 &&mouse_y<=SCREEN_H && !fin)
        {
            fin=true;
            break;
        }
        if (key[KEY_ESC])
        {
            fin=true;
            break;
        }
    }
}
