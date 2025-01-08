#include <iostream>
#include <cassert>
#include "func_graph.hpp"


template <typename T>
T getValue(int index) {
    if constexpr (std::is_same_v<T, int>) {
        return index + 1;
    } else {
        return "v" + std::to_string(index + 1);
    }
}

template <typename T>
void getValues(T& v1, T& v2, T& v3) {
    v1 = getValue<T>(1);
    v2 = getValue<T>(2);
    v3 = getValue<T>(3);
}

template <typename T>
void testAddVertex() {
    Graph<T> graph;
    graph.addVertex(T());
    assert(("AddVertex_vertex_0", graph.hasVertex(T())));
}

template <typename T>
void testAddEdge() {
    Graph<T> graph;
    T v1 = getValue<T>(1);
    T v2 = getValue<T>(2);

    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addEdge(v1, v2);
    assert(("AddEdge_edge_0", graph.hasEdge(v1, v2)));
    assert(("AddEdge_edge_1", graph.hasEdge(v2, v1)));
}

template <typename T>
void testAddDuplicateEdge() {
    Graph<T> graph;
    T v1 = getValue<T>(1);
    T v2 = getValue<T>(2);

    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addEdge(v1, v2);
    graph.addEdge(v1, v2);
    assert(("AddDuplicateEdge_edge_0", graph.hasEdge(v1, v2)));
    assert(("AddDuplicateEdge_edge_1", graph.hasEdge(v2, v1)));
}

template <typename T>
void testRemoveEdge() {
    Graph<T> graph;
    T v1 = getValue<T>(1);
    T v2 = getValue<T>(2);

    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addEdge(v1, v2);
    graph.removeEdge(v1, v2);
    assert(("RemoveEdge_edge_0", !graph.hasEdge(v1, v2)));
    assert(("RemoveEdge_edge_1", !graph.hasEdge(v2, v1)));
}

template <typename T>
void testRemoveVertex() {
    Graph<T> graph;
    T v1 = getValue<T>(1);
    T v2 = getValue<T>(2);

    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addEdge(v1, v2);
    graph.removeVertex(v1);
    assert(("RemoveVertex_vertex_0", !graph.hasVertex(v1)));
    assert(("RemoveVertex_edge_0", !graph.hasEdge(v1, v2)));
    assert(("RemoveVertex_edge_1", !graph.hasEdge(v2, v1)));
}

template <typename T>
void testGetNeighbors() {
    Graph<T> graph;
    T v1, v2, v3;
    getValues(v1, v2, v3);

    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addVertex(v3);
    graph.addEdge(v1, v2);
    graph.addEdge(v1, v3);
    ArraySequence<T> neighbors = graph.getNeighbors(v1);
    assert(("GetNeighbors_size_0", neighbors.getSize() == 2));
    assert(("GetNeighbors_value_0", neighbors.contains(v2)));
    assert(("GetNeighbors_value_1", neighbors.contains(v3)));
}

template <typename T>
void testGetEdges() {
    Graph<T> graph;
    T v1, v2, v3;
    getValues(v1, v2, v3);

    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addVertex(v3);
    graph.addEdge(v1, v2);
    graph.addEdge(v2, v3);
    ArraySequence<Edge<T>> edges = graph.getEdges();
    assert(("GetEdges_size_0", edges.getSize() == 2));
    assert(("GetEdges_value_0", edges.contains(Edge<T>(v1, v2))));
    assert(("GetEdges_value_1", edges.contains(Edge<T>(v2, v3))));
}

template <typename T>
void testGetVertexCount() {
    Graph<T> graph;
    T v1, v2, v3;
    getValues(v1, v2, v3);

    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addVertex(v3);
    assert(("GetVertexCount_size_0", graph.getVertexCount() == 3));
}

template <typename T>
void testGetEdgeCount() {
    Graph<T> graph;
    T v1, v2, v3;
    getValues(v1, v2, v3);

    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addVertex(v3);
    graph.addEdge(v1, v2);
    graph.addEdge(v2, v3);
    graph.addEdge(v3, v1);
    assert(("GetEdgeCount_size_0", graph.getEdgeCount() == 3));
}

void testGraph() {
    testAddVertex<int>();
    testAddEdge<int>();
    testAddDuplicateEdge<int>();
    testRemoveEdge<int>();
    testRemoveVertex<int>();
    testGetNeighbors<int>();
    testGetEdges<int>();
    testGetVertexCount<int>();
    testGetEdgeCount<int>();

    testAddVertex<std::string>();
    testAddEdge<std::string>();
    testAddDuplicateEdge<std::string>();
    testRemoveEdge<std::string>();
    testRemoveVertex<std::string>();
    testGetNeighbors<std::string>();
    testGetEdges<std::string>();
    testGetVertexCount<std::string>();
    testGetEdgeCount<std::string>();

    std::cout << "All tests passed! (Graph)" << std::endl;
}