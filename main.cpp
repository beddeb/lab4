#include <iostream>
#include <sstream>
#include "include/render_graph.hpp"

#include "tests/func/func_sequences.hpp"
#include "tests/func/func_hash_table.hpp"
#include "tests/func/func_set.hpp"
#include "tests/func/func_graph.hpp"

#include "tests/chrono/chrono_sequences.hpp"
#include "tests/chrono/chrono_hash_table.hpp"
#include "tests/chrono/chrono_set.hpp"

#include "graphics/plot.hpp"

#include "include/graph_generator.hpp"


#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

void clearConsole() {
    std::system(CLEAR_COMMAND);
}

template<typename T>
bool readValue(std::stringstream &ss, T &value, const std::string &error_message) {
    if (!(ss >> value)) {
        std::cerr << error_message << "\n";
        return false;
    }
    return true;
}

void menuUser() {
    std::cout << "Available commands:\n"
              << "  help                ~ Commands list\n"
              << "  gen                 ~ Creating a graph using the generator\n"
              << "  addv <vertex>       ~ Add a vertex\n"
              << "  delv <vertex>       ~ Remove a vertex\n"
              << "  adde <v1> <v2> <w>  ~ Add an edge (bidirectional) with weight w\n"
              << "  dele <v1> <v2>      ~ Remove an edge (bidirectional)\n"
              << "  color               ~ Toggle vertex coloring\n"
              << "  mst                 ~ Color minimum spanning tree\n"
              << "  render              ~ Start rendering the graph\n"
              << "  path <v1> <v2>      ~ Shortest path between vertices\n"
              << "  cls                 ~ Clear console\n"
              << "  admin               ~ Admin zone\n"
              << "  exit                ~ Exit the program\n";
}

void menuAdmin() {
    std::cout << "Available ADMIN commands:\n"
              << "  help                ~ Admin commands list\n"
              << "  func                ~ Functional tests\n"
              << "  chrono              ~ Load chrono tests\n"
              << "  plots               ~ Plotting load tests\n"
              << "  cls                 ~ Clear console\n"
              << "  help                ~ Admin commands list"
              << "  exit                ~ Exit admin zone\n";
}

void menuFunc() {
    std::cout << "Available function tests:\n"
              << "  fseq                ~ Sequence\n"
              << "  flistseq            ~ ListSequence\n"
              << "  farrseq             ~ ArraySequence\n"
              << "  fhasht              ~ HashTable\n"
              << "  fhset               ~ HashSet\n"
              << "  fgraph              ~ Graph\n";
}

void menuChrono() {
    std::cout << "Available chrono tests:\n"
              << "  chseq <value>       ~ Sequence vs std::vector\n"
              << "  chlistseq <value>   ~ ListSequence std::list\n"
              << "  charrseq <value>    ~ ArraySequence vs std::vector\n"
              << "  chhasht <value>     ~ HashTable vs std::unorderedmap\n"
              << "  chhset <value>      ~ HashSet vs std::set\n";
}

void menuPlots() {
    std::cout << "Available comparative plots:\n"
              << "  pseq <value>        ~ Sequence vs std::vector\n"
              << "  plistseq <value>    ~ ListSequence vs std::list\n"
              << "  parrseq <value>     ~ ArraySequence vs std::vector\n"
              << "  phasht <value>      ~ HashTable vs std::unorderedmap\n"
              << "  phset <value>       ~ HashSet vs std::set\n";
}

void menuGen() {
    std::cout << "Available graphs:\n"
              << "  chgraph <value>     ~ Chain & number of vertexes\n"
              << "  cygraph <value>     ~ Cycle & number of vertexes\n"
              << "  cograph <value>     ~ Complete & number of vertexes\n";
}

void adminProcess() {
    menuAdmin();
    while (true) {
        std::cout << "> ";
        Sequence<DataSeries> plot_data;
        int values;
        std::string command;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, command);
        std::stringstream ss(command);
        ss >> command;

        if (command == "fseq") {
            testSequence();
        } else if (command == "flistseq") {
            testListSequence();
        } else if (command == "farrseq") {
            testArraySequence();
        } else if (command == "fhasht") {
            testHashTable();
        } else if (command == "fhset") {
            testSet();
        } else if (command == "fgraph") {
            testGraph();
        } else if (command == "chseq") {
            if (!readValue(ss, values, "Error: Number of values is required.")) { return; }
            chronoSequence(values, 100);
        } else if (command == "chlistseq") {
            if (!readValue(ss, values, "Error: Number of values is required.")) { return; }
            chronoListSequence(values, 100);
        } else if (command == "charrseq") {
            if (!readValue(ss, values, "Error: Number of values is required.")) { return; }
            chronoArraySequence(values, 100);
        } else if (command == "chhasht") {
            if (!readValue(ss, values, "Error: Number of values is required.")) { return; }
            chronoHashTable(values);
        } else if (command == "chhset") {
            if (!readValue(ss, values, "Error: Number of values is required.")) { return; }
            chronoSet(values);
        } else if (command == "pseq") {
            if (!readValue(ss, values, "Error: Number of values is required.")) { return; }

            plot_data.add(DataSeries("Sequence", Color::Red));
            plot_data.add(DataSeries("std::vector", Color::Blue));

            plotInit<Sequence<int>, std::vector<int>>(values, plot_data);
        } else if (command == "plistseq") {
            if (!readValue(ss, values, "Error: Number of values is required.")) { return; }

            plot_data.add(DataSeries("ListSequence", Color::Red));
            plot_data.add(DataSeries("std::list", Color::Blue));

            plotInit<ListSequence<int>, std::list<int>>(values, plot_data);
        } else if (command == "parrseq") {
            if (!readValue(ss, values, "Error: Number of values is required.")) { return; }

            plot_data.add(DataSeries("ArraySequence", Color::Red));
            plot_data.add(DataSeries("std::vector", Color::Blue));

            plotInit<ArraySequence<int>, std::vector<int>>(values, plot_data);
        } else if (command == "phasht") {
            if (!readValue(ss, values, "Error: Number of values is required.")) { return; }

            plot_data.add(DataSeries("HashTable", Color::Red));
            plot_data.add(DataSeries("std::unordered_map", Color::Blue));

            plotInit<HashTable<int, int>, std::unordered_map<int, int>>(values, plot_data);
        } else if (command == "phset") {
            if (!readValue(ss, values, "Error: Number of values is required.")) { return; }

            plot_data.add(DataSeries("HashSet", Color::Red));
            plot_data.add(DataSeries("std::set", Color::Blue));

            plotInit<HashSet<int>, std::set<int>>(values, plot_data);
        } else if (command == "func") {
            menuFunc();
        } else if (command == "chrono") {
            menuChrono();
        } else if (command == "plots") {
            menuPlots();
        } else if (command == "cls") {
            clearConsole();
            menuAdmin();
        } else if (command == "help") {
            menuAdmin();
        } else if (command == "exit") {
            return;
        } else {
            std::cout << "Admin zone: Unknown command.\n";
        }
    }
}

int main() {
    Graph<std::string> graph;

    menuUser();

    bool isRunning = true;
    bool needColor = false;
    bool needMSTColor = false;

    while (isRunning) {
        std::cout << "> ";
        std::string command;
        std::cin >> command;

        if (command == "help") {
            menuUser();
        } else if (command == "gen") {
            Graph<std::string> gen_graph;
            std::string choise;
            int numbers;
            menuGen();
            std::cout << "> ";
            std::cin >> choise;
            std::cin >> numbers;
            try {
                if (choise == "chgraph") {
                    graph = generateChainGraph<std::string>(numbers);
                    std::cout << "Chain graph generated: edge's weight is 0.\n";
                } else if (choise == "cygraph") {
                    graph = generateCycleGraph<std::string>(numbers);
                    std::cout << "Cycle graph generated: edge's weight is 0.\n";
                } else if (choise == "cograph") {
                    graph = generateCompleteGraph<std::string>(numbers);
                    std::cout << "Complete graph generated: edge's weight is 0.\n";
                } else {
                    std::cout << "Unknown command.\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "\tError: " << e.what();
            }
        } else if (command == "addv") {
            std::string v;
            std::cin >> v;
            try {
                graph.addVertex(v);
                std::cout << "Vertex " << v << " added.\n";
            } catch (const std::exception& e) {
                std::cerr << "\tError: " << e.what();
            }
        }
        else if (command == "delv") {
            std::string v;
            std::cin >> v;
            try {
                graph.removeVertex(v);
                std::cout << "Vertex " << v << " removed.\n";
            } catch (const std::exception& e) {
                std::cerr << "\tError: " << e.what();
            }
        }
        else if (command == "adde") {
            std::string v1, v2;
            int w;
            std::cin >> v1 >> v2 >> w;
            try {
                graph.addEdge(v1, v2, w);
                std::cout << "Edge (" << v1 << ", " << v2 << ") with weight " << w << " added.\n";
            } catch (const std::exception& e) {
                std::cerr << "\tError: " << e.what();
            }
        }
        else if (command == "dele") {
            std::string v1, v2;
            std::cin >> v1 >> v2;
            try {
                graph.removeEdge(v1, v2);
                std::cout << "Edge (" << v1 << ", " << v2 << ") removed.\n";
            } catch (const std::exception& e) {
                std::cerr << "\tError: " << e.what();
            }
        }
        else if (command == "color") {
            needColor = !needColor;
            std::cout << "Vertex coloring is now " << (needColor ? "enabled" : "disabled") << ".\n";
        }
        else if (command == "mst") {
            needMSTColor = !needMSTColor;
            std::cout << "Minimum spanning tree coloring is now " << (needMSTColor ? "enabled" : "disabled") << ".\n";
        } else if (command == "path") {
            GraphRenderer<std::string> renderer(graph, 800, 600);
            std::string start, end;
            std::cin >> start >> end;
            if (graph.hasVertex(start) && graph.hasVertex(end)) {
                renderer.findShortestPath(start, end);
                std::cout << "Shortest path will be shown in green.\nVisualization window opened. Close the window to return to the console.\n";
                renderer.run();
            } else {
                std::cout << "One or both vertices don't exist in the graph.\n";
            }
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
            menuUser();
        }
        else if (command == "admin") {
            adminProcess();
            menuUser();
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