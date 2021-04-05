#include <iostream>
#include "Plan_Pistes.h"
#include "Menus.h"

int main(int argc, char *argv[])
{
    Plan_Pistes p("");
    Menus m(p);
    bool fin = false;

    while(!fin)
    {
        fin = m.principal();
    }

    p.setup();
    Station s(p);
    s.afficher();
    /*
    s.bfs(1, 5);
    std::cout<<"\n-----------------\nDijkstra :"<<std::endl;
    s.resetAttributs();
    s.aretes();
    s.graphique();
    //s.dijkstra(31, 35);
    //p.setup();
    p.afficher();*/
    p.standby();


    return 0;

}
END_OF_MAIN();                  //Instruction Allegro de fin de programme
