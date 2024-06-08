///-------------------------------------------------------------------------------------------------
/// File:	tiller.cpp.
///
/// Summary:	Demo app for parsing Tiled files
///-------------------------------------------------------------------------------------------------


#include <iostream>

#include "Src\Tiller.h"

int main()
{
    std::cout << "Hello World!\n";

    tile::Tiller::Instance().Load("Resources/level2/", "level2");

    //tile::Tiller::Instance().Map()->PrintMap();
}