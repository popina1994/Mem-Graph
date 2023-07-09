#include "pch.h"
#include "../GraphStorage.h"
#include <random>
#include <chrono>

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

    v = gs.ShortestPath(0, 0, { "A" });
    EXPECT_EQ(v.size(), 0);

    v = gs.ShortestPath(0, 3, { "B" });
    EXPECT_EQ(v.size(), 0);
}

TEST(TestingBasicFunctionality, EdgeCases) {
    using namespace MemGraph;
    GraphStorage gs;
    EXPECT_EQ(gs.CreateVertex(), 0);
    EXPECT_EQ(gs.CreateVertex(), 1);

    gs.AddLabel(0, Label("A"));
    gs.AddLabel(0, Label("B"));
    gs.AddLabel(0, Label("C"));
    gs.AddLabel(1, Label("A"));
    gs.AddLabel(1, Label("B"));

    gs.CreateEdge(0, 1);

    auto v = gs.ShortestPath(0, 0, { "A" });
    EXPECT_EQ(v.size(), 0);
    v = gs.ShortestPath(0, 0, { "B" });
    EXPECT_EQ(v.size(), 0);
    v = gs.ShortestPath(0, 0, { "C" });
    EXPECT_EQ(v.size(), 0);

    v = gs.ShortestPath(1, 1, { "A" });
    EXPECT_EQ(v.size(), 0);
    v = gs.ShortestPath(1, 1, { "B" });
    EXPECT_EQ(v.size(), 0);
    v = gs.ShortestPath(1, 1, { "C" });
    EXPECT_EQ(v.size(), 0);

    v = gs.ShortestPath(0, 1, { "A" });
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);

    v = gs.ShortestPath(0, 1, { "B" });
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    
    v = gs.ShortestPath(0, 1, { "C" });
    EXPECT_EQ(v.size(), 0);
}
TEST(TestingBasicFunctionality, Cycles) {
    using namespace MemGraph;
    GraphStorage gs;
    EXPECT_EQ(gs.CreateVertex(), 0);
    EXPECT_EQ(gs.CreateVertex(), 1);
    EXPECT_EQ(gs.CreateVertex(), 2);

    gs.AddLabel(0, Label("A"));
    gs.AddLabel(1, Label("A"));
    gs.AddLabel(2, Label("A"));

    gs.CreateEdge(0, 1);
    gs.CreateEdge(1, 2);
    gs.CreateEdge(2, 0);

    auto v = gs.ShortestPath(0, 0, { "A" });
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 0);

    v = gs.ShortestPath(1, 1, { "A" });
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 0);
    EXPECT_EQ(v[3], 1);

    v = gs.ShortestPath(2, 2, { "A" });
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[1], 0);
    EXPECT_EQ(v[2], 1);
    EXPECT_EQ(v[3], 2);
}
TEST(TestingBasicFunctionality, SelfCycles) {
    using namespace MemGraph;
    GraphStorage gs;
    EXPECT_EQ(gs.CreateVertex(), 0);
    EXPECT_EQ(gs.CreateVertex(), 1);
    EXPECT_EQ(gs.CreateVertex(), 2);

    gs.AddLabel(0, Label("A"));
    gs.AddLabel(1, Label("A"));
    gs.AddLabel(2, Label("A"));

    gs.CreateEdge(0, 0);
    gs.CreateEdge(0, 1);
    gs.CreateEdge(1, 2);
    gs.CreateEdge(2, 0);

    auto v = gs.ShortestPath(0, 0, { "A" });
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 0);

    v = gs.ShortestPath(1, 1, { "A" });
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 0);
    EXPECT_EQ(v[3], 1);

    v = gs.ShortestPath(2, 2, { "A" });
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[1], 0);
    EXPECT_EQ(v[2], 1);
    EXPECT_EQ(v[3], 2);
}

TEST(TestingBasicFunctionality, StressTestRequirement) {
    using namespace MemGraph;
    GraphStorage gs;
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, 99'999);
    std::uniform_int_distribution<> dist2(0, 1);

    using namespace std::chrono;

    auto start = std::chrono::high_resolution_clock::now();

    for (uint32_t idx = 0; idx < 100'000; idx++)
    {
        Type::VERTEX_ID vId = gs.CreateVertex();
        gs.AddLabel(vId, Label("A"));
    }
    for (uint32_t idx = 0; idx < 100'000; idx++)
    {
        gs.CreateEdge(distrib(gen), distrib(gen));
    }

    for (uint32_t idx = 0; idx < 100; idx++)
    {
        Type::VERTEX_ID vertStart = distrib(gen);
        Type::VERTEX_ID vertEnd = distrib(gen);
        auto v = gs.ShortestPath(vertStart, vertEnd, { "A" });
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    EXPECT_EQ(duration.count() < 10000, true);
}

TEST(TestingBasicFunctionality, StressTestCompleteGraph) {
    using namespace MemGraph;
    GraphStorage gs;
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    constexpr uint32_t NUM_NODES = 75;
    std::uniform_int_distribution<> distrib(0, NUM_NODES);
    std::uniform_int_distribution<> dist2(0, 1);

    using namespace std::chrono;

    auto start = std::chrono::high_resolution_clock::now();

    for (uint32_t idx = 0; idx < NUM_NODES; idx++)
    {
        Type::VERTEX_ID vId = gs.CreateVertex();
        gs.AddLabel(vId, Label("A"));
    }
    for (uint32_t idx1 = 0; idx1 < NUM_NODES; idx1++)
    {
        for (uint32_t idx2 = 0; idx2 < NUM_NODES; idx2++)
        {
            gs.CreateEdge(idx1, idx2);
        }
    }

    for (uint32_t idx1 = 0; idx1 < NUM_NODES; idx1++)
    {
        for (uint32_t idx2 = 0; idx2 < NUM_NODES; idx2++)
        {
            auto v = gs.ShortestPath(idx1, idx2, { "A" });
            EXPECT_EQ(v.size(), 2);
            EXPECT_EQ(v[0], idx1);
            EXPECT_EQ(v[1], idx2);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    EXPECT_EQ(duration.count() < 10000, true);
}

TEST(TestingBasicFunctionality, StressTestPathGraph) {
    using namespace MemGraph;
    GraphStorage gs;
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    constexpr uint32_t NUM_NODES = 100'000;
    std::uniform_int_distribution<> distrib(0, NUM_NODES);

    using namespace std::chrono;

    auto start = std::chrono::high_resolution_clock::now();
    for (uint32_t idx = 0; idx < NUM_NODES; idx++)
    {
        Type::VERTEX_ID vId = gs.CreateVertex();
        gs.AddLabel(vId, Label("A"));
    }
    
    for (uint32_t idx1 = 0; idx1 < NUM_NODES - 1; idx1++)
    {
        gs.CreateEdge(idx1, idx1 + 1);
    }

    for (uint32_t idx1 = 0; idx1 < 10; idx1++)
    {
        for (uint32_t idx2 = NUM_NODES - 10; idx2 < NUM_NODES; idx2++)
        {
            auto v = gs.ShortestPath(idx1, idx2, { "A" });
            EXPECT_EQ(v.size(), idx2 - idx1 + 1);
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    EXPECT_EQ(duration.count() < 10000, true);
}


