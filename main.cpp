#include <iostream>
#include "include/render_graph.hpp"
#include <string>


int main() {
    // Create a graph for string vertex identifiers
    Graph<std::string> graph;

    // We will use a command vector for reference
    std::cout << "Available commands:\n"
              << "  addv <vertex>       - Add a vertex\n"
              << "  delv <vertex>       - Remove a vertex\n"
              << "  adde <v1> <v2>      - Add an edge (bidirectional)\n"
              << "  dele <v1> <v2>      - Remove an edge (bidirectional)\n"
              << "  color               - Enable vertex coloring\n"
              << "  render              - Start rendering the graph\n"
              << "  exit                - Exit the program\n\n";

    bool isRunning = true;
    bool needColor = false;

    while (isRunning) {
        std::cout << "> ";
        std::string command;
        std::cin >> command;

        if (command == "addv") {
            std::string v;
            std::cin >> v;
            graph.addVertex(v);
            std::cout << "Vertex " << v << " added.\n";
        }
        else if (command == "delv") {
            std::string v;
            std::cin >> v;
            graph.removeVertex(v);
            std::cout << "Vertex " << v << " removed.\n";
        }
        else if (command == "adde") {
            std::string v1, v2;
            std::cin >> v1 >> v2;
            graph.addEdge(v1, v2);
            std::cout << "Edge (" << v1 << ", " << v2 << ") added.\n";
        }
        else if (command == "dele") {
            std::string v1, v2;
            std::cin >> v1 >> v2;
            graph.removeEdge(v1, v2);
            std::cout << "Edge (" << v1 << ", " << v2 << ") removed.\n";
        }
        else if (command == "color") {
            needColor = true;
            std::cout << "Coloring will be applied when rendering.\n";
        }
        else if (command == "render") {
            // Create a rendering object, passing our graph
            GraphRenderer<std::string> renderer(graph, 800, 600);
            if (needColor) {
                renderer.toggleColorizeGraph();
            }
            std::cout << "Visualization window opened. Close the window to return to the console.\n";
            renderer.run();
            std::cout << "Rendering completed. You have returned to the console.\n";
        }
        else if (command == "exit") {
            isRunning = false;
        }
        else {
            std::cout << "Unknown command.\n";
        }
    }

    std::cout << "Exiting the program...\n";
    return 0;
}