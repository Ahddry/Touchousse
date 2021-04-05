#include <iostream>
#include "Plan_Pistes.h"
#include "Station.h"

int main(int argc, char *argv[])
{
    Plan_Pistes p("");
    /*
    p.setup();
    Station s(p);
    s.afficher();
    s.bfs(1, 5);
    std::cout<<"\n-----------------\nDijkstra :"<<std::endl;
    s.resetAttributs();
    s.aretes();
    s.graphique();
    //s.dijkstra(31, 35);
    //p.setup();
    p.afficher();
    p.standby();
    */
    bool fin = false;

    while(!fin)
    {
        int choix = p.menuPrincipal();
        switch(choix)
        {
            case 1:
                {
                    p.effacer();
                    p.setup();
                    Station s(p);
                    s.graphique();
                    p.standby();
                    break;
                }

            case 7:
                fin = true;
                break;
            default:
                break;
        }

    }
    return 0;

}
END_OF_MAIN();                  //Instruction Allegro de fin de programme
