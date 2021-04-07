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
    return 0;

}
END_OF_MAIN();                  //Instruction Allegro de fin de programme
