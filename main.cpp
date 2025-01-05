#include <iostream>
#include "include/render_graph.hpp"


#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

void clearConsole() {
    std::system(CLEAR_COMMAND);
}

void menuStaff() {
    std::cout << "Available commands:\n"
              << "  help                - Commands list\n"
              << "  addv <vertex>       - Add a vertex\n"
              << "  delv <vertex>       - Remove a vertex\n"
              << "  adde <v1> <v2> <w>  - Add an edge (bidirectional) with weight w\n"
              << "  dele <v1> <v2>      - Remove an edge (bidirectional)\n"
              << "  color               - Toggle vertex coloring\n"
              << "  mst                 - Color minimum spanning tree\n"
              << "  render              - Start rendering the graph\n"
              << "  cls                 - Clear console\n"
              << "  exit                - Exit the program\n\n";
}

int main() {
    Graph<std::string> graph;

    menuStaff();

    bool isRunning = true;
    bool needColor = false;
    bool needMSTColor = false;

    while (isRunning) {
        std::cout << "> ";
        std::string command;
        std::cin >> command;

        if (command == "help"){
            menuStaff();
        } else if (command == "addv") {
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
            int w;
            std::cin >> v1 >> v2 >> w;
            graph.addEdge(v1, v2, w);
            std::cout << "Edge (" << v1 << ", " << v2 << ") with weight " << w << " added.\n";
        }
        else if (command == "dele") {
            std::string v1, v2;
            std::cin >> v1 >> v2;
            graph.removeEdge(v1, v2);
            std::cout << "Edge (" << v1 << ", " << v2 << ") removed.\n";
        }
        else if (command == "color") {
            needColor = !needColor;
            std::cout << "Vertex coloring is now " << (needColor ? "enabled" : "disabled") << ".\n";
        }
        else if (command == "mst") {
            needMSTColor = !needMSTColor;
            std::cout << "Minimum spanning tree coloring is now " << (needMSTColor ? "enabled" : "disabled") << ".\n";
        }
        else if (command == "render") {
            GraphRenderer<std::string> renderer(graph, 800, 600);
            if (needColor) {
                renderer.toggleColorizeGraph();
            }
            if (needMSTColor) {
                renderer.toggleColorizeMST();
            }
            std::cout << "Visualization window opened. Close the window to return to the console.\n";
            renderer.run();
            std::cout << "Rendering completed. You have returned to the console.\n";
        }
        else if (command == "cls") {
            clearConsole();
            menuStaff();
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