#include <iostream>
#include "include/render_graph.hpp"


int main() {
    Graph<int> graph;

    // Добавляем вершины и рёбра в граф
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(2, 4);

//    sf::ContextSettings settings;
//    settings.antialiasingLevel = 8;

    GraphRenderer<int> renderer(graph, 800, 600);
    renderer.colorizeGraph();
    renderer.run();

    return 0;
}