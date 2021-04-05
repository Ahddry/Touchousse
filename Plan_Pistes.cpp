#include "Plan_Pistes.h"
#include "time.h"
#include "math.h"
#include <iostream>

#define ROUGE makecol(255,0,0)
#define VERT makecol(0,255,0)
#define BLEU makecol(0,0,255)
#define NOIR makecol(0,0,0)

#define ROUGE_DOUX makecol(255,50,50)
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



void Plan_Pistes::point(Point p)            //affichage d'un point sur le buffer
{
    int x = p.x()+35, y = p.y()+40;
    FONT *old = load_font("Fonts/regles.pcx",NULL,NULL);
    circle(m_plan, x, y, 25, NOIR);
    textout_centre_ex(m_plan, old, std::to_string(p.getNum()).c_str(), x, y-15, NOIR, -1);
    afficher();
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
    else if(type=="TPH") couleur = NOIR;
    else if(type=="TC") couleur = NOIR;
    else if(type=="TSD") couleur = NOIR;
    else if(type=="TS") couleur = makecol(255, 125, 0);
    else if(type=="TK") couleur = NOIR;
    else if(type=="BUS") couleur = makecol(255, 255, 0);

    //compteur *=7.5;
    int x1 = depart.x()+35;
    int y1 = depart.y()+40;
    int x2 = arrivee.x()+35;
    int y2 = arrivee.y()+40;


    int delta = 25;

    int polaireX1 = delta * cos(compteur*M_PI/6);
    int polaireY1 = delta * sin(compteur*M_PI/6);
    int polaireX2 = delta * cos((compteur-1)*M_PI/4);
    int polaireY2 = delta * sin((compteur-1)*M_PI/4);

    if(x1>x2 && y1>y2)
        //line(m_plan, x1-delta-compteur, y1-delta-compteur, x2+delta-compteur, y2+delta-compteur, couleur);
        trait(type, x1-polaireX1, y1-polaireY1, x2+polaireX2, y2+polaireY2, couleur);
    else if(x1<x2 && y1>y2)
        //line(m_plan, x1+delta-compteur, y1-delta-compteur, x2-delta-compteur, y2+delta-compteur, couleur);
        trait(type, x1+polaireX1, y1-polaireY1, x2-polaireX2, y2-polaireY2, couleur);
    else if(x1<x2 && y1<y2)
        //line(m_plan, x1+delta-compteur, y1+delta-compteur, x2-delta-compteur, y2-delta-compteur, couleur);
        trait(type, x1+polaireX1, y1+polaireY1, x2-polaireX2, y2-polaireY2, couleur);
    else if(x1>x2 && y1<y2)
        //line(m_plan, x1-delta+compteur, y1+delta+compteur, x2+delta+compteur, y2-delta+compteur, couleur);
        trait(type, x1-polaireX1, y1+polaireY1, x2+polaireX2, y2+polaireY2, couleur);


}

void Plan_Pistes::trait(std::string type, int x1, int y1, int x2, int y2, int couleur)
{
    if(type=="V"||type=="B"||type=="R"||type=="N"||type=="KL"||type=="SURF"||type=="BUS")
    {
        line(m_plan, x1, y1, x2, y2, couleur);
        rectfill(m_plan, x2-2, y2-2, x2+2, y2+2, couleur);
    }
    else if(type=="TPH")
    {
        line(m_plan, x1, y1, x2, y2, couleur);
        rectfill(m_plan, x2-2, y2-2, x2+2, y2+2, couleur);
    }
    else if(type=="TC")
    {
        line(m_plan, x1, y1, x2, y2, couleur);
        rectfill(m_plan, x2-2, y2-2, x2+2, y2+2, couleur);
    }
    else if(type=="TSD"||type=="TS")
    {
        line(m_plan, x1, y1, x2, y2, couleur);
        for(int i=15; i>0;i--)
        {
            circlefill(m_plan, x1+i*(x2-x1)/16,y1+i*(y2-y1)/16, 2, couleur);
        }
        rectfill(m_plan, x2-2, y2-2, x2+2, y2+2, makecol(0, 195, 195));
    }
    else if(type=="TK")
    {
        line(m_plan, x1, y1, x2, y2, couleur);
        rectfill(m_plan, x2-2, y2-2, x2+2, y2+2, couleur);
    }
}


void Plan_Pistes::emphase(std::string titre, std::string sousTitre)
{   ///Message accentu� avec une premi�re chose �crite en gros et une seconde en un peu plus petit en dessous
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);

    // Texte centr�
    textout_centre_ex(m_plan, MiddleTitle,titre.c_str(), SCREEN_W/2,SCREEN_H/2-100, makecol(255,200,0),-1);
    textout_centre_ex(m_plan, SubTitle,sousTitre.c_str(), SCREEN_W/2,SCREEN_H/2-20, makecol(0,170,250),-1);

    afficher();
}

int Plan_Pistes::menu(std::string titre, std::vector<std::string> propositions, int nbChoix)///Menu divers avec choix param�trables, attendant un clic de l'utilisateur sur une des options propos�es
{
    //Polices utilis�es
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);

    //image de fond
    BITMAP* menu = load_bitmap_check("img/Menu.bmp");
    masked_blit(menu,m_plan,0,0,SCREEN_W/2-360,SCREEN_H/10-72,SCREEN_W,SCREEN_H);

    //Texte centr�
    textout_centre_ex(m_plan, MiddleTitle,titre.c_str(), SCREEN_W/2,SCREEN_H/4, makecol(213,70,40),-1);
    int j=100;
    for(const auto& elem: propositions)
    {
        textout_centre_ex(m_plan, SubTitle,elem.c_str(), SCREEN_W/2,SCREEN_H/4+j, makecol(160,108,61),-1);
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
    std::string saisie;
    int poscarac=0; //position du charact�re pour son affichage
    bool fin = false;
    int touche;
    //Polices utilis�es
    FONT *MiddleTitle = load_font("Fonts/MiddleTitle.pcx",NULL,NULL);
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);
    //Ce que l'on demande � l'utilisateur
    textout_centre_ex(m_plan, MiddleTitle,titre.c_str(), SCREEN_W/2,SCREEN_H/4, makecol(255,200,0),-1);
    textout_centre_ex(m_plan, SubTitle,texte.c_str(), SCREEN_W/2,SCREEN_H/4+80, makecol(160,108,61),-1);
    afficher();
    set_keyboard_rate(500,200); //pour �viter qu'un appui de touche fasse appara�tre plus de charact�res que d�sir�
    rest(50);
    char carac;                 //Charact�re saisi
    while (!fin)
    {

        // keypressed() : d�tection de touche appuy�e
        if (keypressed())
        {
            effacer();
            textout_centre_ex(m_plan, MiddleTitle,titre.c_str(), SCREEN_W/2,SCREEN_H/4, makecol(255,200,0),-1);
            textout_centre_ex(m_plan, SubTitle,texte.c_str(), SCREEN_W/2,SCREEN_H/4+80, makecol(160,108,61),-1);
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
            textout_centre_ex(m_plan, SubTitle,saisie.c_str(), SCREEN_W/2,SCREEN_H/2, makecol(160,108,61),-1);
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
    afficher();
}

void Plan_Pistes::erreur(std::string msg)///Message d'erreur en rouge au bas de l'�cran
{
    //Police
    FONT *SubTitle = load_font("Fonts/SubTitle.pcx",NULL,NULL);
    //Message
    textout_centre_ex(m_plan, SubTitle,msg.c_str(), SCREEN_W/2,SCREEN_H/4*3, makecol(255,50,50),-1);
    afficher();
    rest(1500);
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
    textout_centre_ex(m_plan, MiddleTitle,"TOUCHOUSSE", SCREEN_W/2,SCREEN_H/4-75, ROUGE_DOUX,-1);

    std::vector<std::string> propositions;//Ajout des choix � afficher
    propositions.push_back("Plan interactif");
    propositions.push_back("???");
    propositions.push_back("???");
    propositions.push_back("???");
    propositions.push_back("Credits");
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
            choix = 1;//Lancer le jeu
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-450,SCREEN_H/4+60,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+300,SCREEN_H/4+60,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+145 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+225)
        {
            choix = 2;//Affichge des r�gles
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-450,SCREEN_H/4+155,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+300,SCREEN_H/4+155,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+230 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+310)
        {
            choix = 3;//Sous-menus de cr�ation de deck
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-450,SCREEN_H/4+240,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+300,SCREEN_H/4+240,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+315 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+395)
        {
            choix = 4;//ouverture de la boutique
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-450,SCREEN_H/4+325,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+300,SCREEN_H/4+325,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+400 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+480)
        {
            choix = 5;//Affichage des cr�dits
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-450,SCREEN_H/4+410,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+300,SCREEN_H/4+410,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+485 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+565)
        {
            choix = 6;//Sous-menu des extensions
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-450,SCREEN_H/4+495,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+300,SCREEN_H/4+495,SCREEN_W,SCREEN_H);
            afficher();
            rest(250);
            fin = true;
        }
        if ( mouse_b&1 && mouse_x>=SCREEN_W/2-350 && mouse_y>=SCREEN_H/4+570 && mouse_x<=SCREEN_W/2+350 &&mouse_y<=SCREEN_H/4+650)
        {
            choix = 7;//Quitter
            masked_blit(arrow1,m_plan,0,0,SCREEN_W/2-450,SCREEN_H/4+580,SCREEN_W,SCREEN_H);//fl�ches accentuant la s�lection de l'utilisateur
            masked_blit(arrow2,m_plan,0,0,SCREEN_W/2+300,SCREEN_H/4+580,SCREEN_W,SCREEN_H);
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
