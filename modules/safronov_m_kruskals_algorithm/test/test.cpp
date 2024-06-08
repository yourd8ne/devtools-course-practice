// Copyright 2024 Safronov Mikhail

#include <gtest/gtest.h>
#include <set>
#include "include/graph.h"

struct EdgeComparator {
    bool operator()(const Edge& a, const Edge& b) const {
        if (a.src != b.src) return a.src < b.src;
        if (a.dest != b.dest) return a.dest < b.dest;
        return a.weight < b.weight;
    }
};

void checkMST(const std::vector<Edge>& mst, const std::vector<Edge>& expected) {
    std::set<Edge, EdgeComparator> mst_set(mst.begin(), mst.end());
    std::set<Edge, EdgeComparator> expected_set(expected.begin(),
    expected.end());

    ASSERT_EQ(mst_set.size(), expected_set.size());

    auto it1 = mst_set.begin();
    auto it2 = expected_set.begin();
    for (; it1 != mst_set.end() && it2 != expected_set.end(); ++it1, ++it2) {
        EXPECT_EQ(it1->src, it2->src);
        EXPECT_EQ(it1->dest, it2->dest);
        EXPECT_EQ(it1->weight, it2->weight);
    }
}

TEST(KruskalAlgorithmTest, SingleEdgeGraph) {
    Graph g(2);
    g.addEdge(0, 1, 10);

    std::vector<Edge> mst = g.kruskalMST();
    std::vector<Edge> expected = {{0, 1, 10}};
    checkMST(mst, expected);
}

TEST(KruskalAlgorithmTest, DisconnectedGraph) {
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(2, 3, 5);

    std::vector<Edge> mst = g.kruskalMST();
    std::vector<Edge> expected = {{2, 3, 5}, {0, 1, 10}};
    checkMST(mst, expected);
}

TEST(KruskalAlgorithmTest, GraphWithCycles) {
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);
    g.addEdge(1, 2, 1);  // This edge creates a cycle

    std::vector<Edge> mst = g.kruskalMST();
    std::vector<Edge> expected = {{1, 2, 1}, {2, 3, 4}, {0, 3, 5}};
    checkMST(mst, expected);
}

TEST(KruskalAlgorithmTest, EmptyGraph) {
    Graph g(0);

    std::vector<Edge> mst = g.kruskalMST();
    std::vector<Edge> expected = {};
    checkMST(mst, expected);
}

TEST(KruskalAlgorithmTest, NegativeWeightsGraph) {
    Graph g(4);
    g.addEdge(0, 1, -10);
    g.addEdge(0, 2, -6);
    g.addEdge(0, 3, -5);
    g.addEdge(1, 3, -15);
    g.addEdge(2, 3, -4);

    std::vector<Edge> mst = g.kruskalMST();
    std::vector<Edge> expected = {{1, 3, -15}, {0, 1, -10}, {0, 2, -6}};
    checkMST(mst, expected);
}

TEST(KruskalAlgorithmTest, HandleDuplicateEdges) {
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 1, 10);  // Duplicate edge
    g.addEdge(1, 2, 15);
    g.addEdge(2, 3, 4);

    std::vector<Edge> mst = g.kruskalMST();
    std::vector<Edge> expected = {{2, 3, 4}, {0, 1, 10}, {1, 2, 15}};
    checkMST(mst, expected);
}

TEST(KruskalAlgorithmTest, DetectDisconnectedComponents) {
    Graph g(5);
    g.addEdge(0, 1, 1);
    g.addEdge(2, 3, 2);

    std::vector<Edge> mst = g.kruskalMST();
    std::vector<Edge> expected = {{0, 1, 1}, {2, 3, 2}};
    checkMST(mst, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}