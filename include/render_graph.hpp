#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <queue>
#include <map>
#include "graph.hpp"

template <typename T>
std::string toString(const T& value) {
    return std::to_string(value);
}

template <>
std::string toString(const std::string& value) {
    return value;
}

template <typename T>
class GraphRenderer {
public:
    GraphRenderer(const Graph<T>& graph, unsigned int windowWidth, unsigned int windowHeight)
            : graph(graph),
              window(sf::VideoMode(windowWidth, windowHeight), "Graph Renderer"),
              is_colored(false),
              is_mst_colored(false) {
        if (!font.loadFromFile("../externallibs/font.ttf")) {
            throw std::runtime_error("Failed to load font");
        }
        coloring = greedyColoring();
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

    void toggleColorizeGraph() {
        is_colored = !is_colored;
    }

    void toggleColorizeMST() {
        is_mst_colored = !is_mst_colored;
    }

private:
    const Graph<T>& graph;
    sf::RenderWindow window;
    std::vector<sf::CircleShape> vertices;
    std::vector<sf::VertexArray> edges;
    std::vector<sf::Text> vertex_labels;
    sf::Font font;
    bool is_colored;
    bool is_mst_colored;
    std::vector<size_t> coloring;
    ArraySequence<Edge<T>> mst_edges;

    void colorMST() {
        mst_edges = kruskal(graph);
    }

    std::vector<size_t> greedyColoring() {
        size_t vertexCount = graph.getVertexCount();
        std::vector<size_t> colors(vertexCount, -1);
        std::map<T, size_t> vertexIndexMap;
        size_t index = 0;

        for (const auto& pair : graph.getAdjacencyList()) {
            vertexIndexMap[pair.first] = index++;
        }

        index = 0;
        for (const auto& pair : graph.getAdjacencyList()) {
            const T& currentVertex = pair.first;
            std::vector<bool> available(vertexCount, true);

            const auto& neighbors = graph.getNeighbors(currentVertex);
            for (size_t j = 0; j < neighbors.getSize(); ++j) {
                const T& neighbor = neighbors.get(j);
                if (colors[vertexIndexMap[neighbor]] != -1) {
                    available[colors[vertexIndexMap[neighbor]]] = false;
                }
            }

            for (size_t c = 0; c < vertexCount; ++c) {
                if (available[c]) {
                    colors[vertexIndexMap[currentVertex]] = c;
                    break;
                }
            }
            index++;
        }

        return colors;
    }

    sf::Color getColorFromIndex(size_t index) {
        static const std::vector<sf::Color> colors = {
                sf::Color::Red,
                sf::Color::Green,
                sf::Color::Blue,
                sf::Color::Yellow,
                sf::Color::Cyan,
                sf::Color::Magenta
        };
        return colors[index % colors.size()];
    }

    ArraySequence<Edge<T>> kruskal(const Graph<T>& graph) {
        ArraySequence<Edge<T>> mst;
        DisjointSet disjointSet;

        for (const auto& vertex : graph.getAdjacencyList()) {
            disjointSet.makeSet(vertex.first);
        }

        ArraySequence<Edge<T>> edges = graph.getEdges();
        edges.sort([](const Edge<T>& a, const Edge<T>& b) {
            return a.weight < b.weight;
        });

        for (const auto& edge : edges) {
            if (disjointSet.findSet(edge.vertex1) != disjointSet.findSet(edge.vertex2)) {
                mst.add(edge);
                disjointSet.unionSets(edge.vertex1, edge.vertex2);
            }
        }

        return mst;
    }

    class DisjointSet {
    private:
        std::map<T, T> parent;
        std::map<T, int> rank;

    public:
        void makeSet(const T& vertex) {
            parent[vertex] = vertex;
            rank[vertex] = 0;
        }

        T findSet(const T& vertex) {
            if (parent[vertex] != vertex) {
                parent[vertex] = findSet(parent[vertex]);
            }
            return parent[vertex];
        }

        void unionSets(const T& vertex1, const T& vertex2) {
            T root1 = findSet(vertex1);
            T root2 = findSet(vertex2);

            if (root1 != root2) {
                if (rank[root1] < rank[root2]) {
                    parent[root1] = root2;
                } else if (rank[root1] > rank[root2]) {
                    parent[root2] = root1;
                } else {
                    parent[root2] = root1;
                    rank[root1]++;
                }
            }
        }

    };

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    void update() {
        vertices.clear();
        edges.clear();
        vertex_labels.clear();

        float radius = std::min(window.getSize().x, window.getSize().y) * 0.35f;
        auto adjacencyList = graph.getAdjacencyList();
        size_t vertexCount = adjacencyList.size();

        if (vertexCount == 0) return;

        float angleStep = 2 * M_PI / vertexCount;
        std::map<T, sf::Vector2f> vertexPositions;
        size_t colorIndex = 0;

        for (const auto& pair : adjacencyList) {
            const T& vertex = pair.first;
            float angle = colorIndex * angleStep;
            sf::Vector2f position(
                    window.getSize().x / 2 + radius * cos(angle),
                    window.getSize().y / 2 + radius * sin(angle)
            );
            vertexPositions[vertex] = position;
            colorIndex++;
        }

        if (is_mst_colored) {
            colorMST();
        }

        for (const auto& edge : graph.getEdges()) {
            sf::Vector2f pos1 = vertexPositions[edge.vertex1];
            sf::Vector2f pos2 = vertexPositions[edge.vertex2];

            sf::VertexArray edgeArray(sf::Lines, 2);
            edgeArray[0].position = pos1;
            edgeArray[1].position = pos2;

            // Проверяем, входит ли ребро в MST
            bool isMSTEdge = false;
            for (const auto& mstEdge : mst_edges) {
                if ((mstEdge.vertex1 == edge.vertex1 && mstEdge.vertex2 == edge.vertex2) ||
                    (mstEdge.vertex1 == edge.vertex2 && mstEdge.vertex2 == edge.vertex1)) {
                    isMSTEdge = true;
                    break;
                }
            }

            if (is_mst_colored && isMSTEdge) {
                edgeArray[0].color = sf::Color::Red;
                edgeArray[1].color = sf::Color::Red;
            } else {
                edgeArray[0].color = sf::Color::Black;
                edgeArray[1].color = sf::Color::Black;
            }

            edges.push_back(edgeArray);

            sf::Text weightText;
            weightText.setFont(font);
            weightText.setString(std::to_string(edge.weight));
            weightText.setCharacterSize(20);
            weightText.setFillColor(sf::Color::Black);
            sf::FloatRect textBounds = weightText.getLocalBounds();
            weightText.setPosition(
                    (pos1.x + pos2.x) / 2 - textBounds.width / 2,
                    (pos1.y + pos2.y) / 2 - textBounds.height / 2
            );
            vertex_labels.push_back(weightText);
        }

        colorIndex = 0;
        for (const auto& pair : adjacencyList) {
            const T& vertex = pair.first;
            sf::Vector2f pos = vertexPositions[vertex];

            sf::CircleShape vertexShape(20);
            vertexShape.setFillColor(is_colored ? getColorFromIndex(coloring[colorIndex]) : sf::Color::White);
            vertexShape.setOutlineColor(sf::Color::Black);
            vertexShape.setOutlineThickness(2);
            vertexShape.setPosition(pos.x - vertexShape.getRadius(), pos.y - vertexShape.getRadius());
            vertices.push_back(vertexShape);

            sf::Text label;
            label.setFont(font);
            label.setString(toString(vertex));
            label.setCharacterSize(20);
            label.setFillColor(sf::Color::Black);
            sf::FloatRect textBounds = label.getLocalBounds();
            label.setPosition(
                    pos.x - textBounds.width / 2,
                    pos.y - textBounds.height / 2
            );
            vertex_labels.push_back(label);

            colorIndex++;
        }
    }

    void render() {
        window.clear(sf::Color::White);

        for (const auto& edge : edges) {
            window.draw(edge);
        }

        for (const auto& vertex : vertices) {
            window.draw(vertex);
        }

        for (const auto& label : vertex_labels) {
            window.draw(label);
        }

        window.display();
    }
};