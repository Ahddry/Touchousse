#include <iostream>
#include "Plan_Pistes.h"
#include "Station.h"

int main()
{
    Station s("data_arcs.txt");
    s.afficher();
    s.bfs(1, 5);
    std::cout<<"\n-----------------\nDijkstra :"<<std::endl;
    s.resetAttributs();
    s.aretes();
    //s.dijkstra(31, 35);

    return 0;
}
