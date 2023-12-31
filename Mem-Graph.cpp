// Mem-Graph.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GraphStorage.h"
#include <random>

int main()
{
    using namespace MemGraph;
    GraphStorage gs;
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, 99'999);
    
    try {

        for (uint32_t idx = 0; idx < 100'000; idx++)
        {
            Type::VERTEX_ID vId = gs.CreateVertex();
            gs.AddLabel(vId, Label("A"));
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    for (uint32_t idx = 0; idx < 100'000; idx++)
    {
        gs.CreateEdge(idx, distrib(gen));
    }
}
