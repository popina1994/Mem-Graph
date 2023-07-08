#include "pch.h"
#include "../GraphStorage.h"
#include <random>


TEST(TestingBasicFunctionality, BasicFunctionality) {
    using namespace MemGraph;
    GraphStorage gs;
    EXPECT_EQ(gs.CreateVertex(), 0);
    EXPECT_EQ(gs.CreateVertex(), 1);
    EXPECT_EQ(gs.CreateVertex(), 2);
    EXPECT_EQ(gs.CreateVertex(), 3);

    gs.AddLabel(0, Label("A"));
    gs.AddLabel(1, Label("A"));
    gs.AddLabel(2, Label("A"));
    gs.AddLabel(3, Label("A"));

    EXPECT_EQ(gs.VertexHasLabel(0, { "A" }), true);
    EXPECT_EQ(gs.VertexHasLabel(1, { "A" }), true);
    EXPECT_EQ(gs.VertexHasLabel(2, { "A" }), true);
    EXPECT_EQ(gs.VertexHasLabel(3, { "A" }), true);

    gs.CreateEdge(0, 1);
    gs.CreateEdge(1, 2);
    gs.CreateEdge(2, 3);

    EXPECT_EQ(gs.ExistsEdge(0, 1), true);
    EXPECT_EQ(gs.ExistsEdge(1, 2), true);
    EXPECT_EQ(gs.ExistsEdge(2, 3), true);

    auto v = gs.ShortestPath(0, 3, { "A" });
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 3);
}

TEST(TestingBasicFunctionality, StressTest) {
    using namespace MemGraph;
    GraphStorage gs;
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, 99'999);
    std::uniform_int_distribution<> dist2(0, 1);

    try {

        for (uint32_t idx = 0; idx < 100'000; idx++)
        {
            Vertex::VERTEX_ID vId = gs.CreateVertex();
            gs.AddLabel(vId, Label("A"));
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    for (uint32_t idx = 0; idx < 500'000; idx++)
    {
        gs.CreateEdge(distrib(gen), distrib(gen));
    }

    for (uint32_t idx = 0; idx < 100; idx++)
    {
        Vertex::VERTEX_ID vertStart = distrib(gen);
        Vertex::VERTEX_ID vertEnd = distrib(gen);
        auto v = gs.ShortestPath(vertStart, vertEnd, { "A" });
    }
}
