#include <iostream>
#include "include/graph.hpp"
#include "include/render_graph.hpp"

int main() {
    Graph<std::string> graph;

    graph.addVertex("1");
    graph.addVertex("2");
    graph.addVertex("3");
    graph.addVertex("4");
    graph.addVertex("5");
    graph.addVertex("6");


    graph.addEdge(0, 1); // A -> B
    graph.addEdge(0, 2); // A -> C
    graph.addEdge(1, 3); // B -> D
    graph.addEdge(2, 3); // C -> D
    graph.addEdge(1, 4);

    GraphRenderer<std::string> renderer(graph, 800, 600);
    renderer.run();

    return 0;
}