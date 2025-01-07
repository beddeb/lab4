#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "graph.hpp"
#include "calc_algs.hpp"


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
    ArraySequence<T> shortest_path;
    bool show_shortest_path;

    void colorMST() {
        mst_edges = kruskal(graph);
    }

    void drawShortestPath() {
        if (!show_shortest_path || shortest_path.getSize() < 2) return;

        float radius = std::min(window.getSize().x, window.getSize().y) * 0.35f;
        float angle_step = 2 * M_PI / graph.getVertexCount();

        // Создаем карту позиций вершин
        HashTable<T, size_t> vertexIndices;
        size_t index = 0;
        for (const auto& pair : graph.getAdjacencyList()) {
            vertexIndices[pair.first] = index++;
        }

        // Рисуем только кратчайший путь
        for (size_t i = 0; i < shortest_path.getSize() - 1; ++i) {
            const T& v1 = shortest_path.get(i);
            const T& v2 = shortest_path.get(i + 1);

            float angle1 = vertexIndices[v1] * angle_step;
            float angle2 = vertexIndices[v2] * angle_step;

            sf::Vector2f pos1(
                    window.getSize().x / 2 + radius * cos(angle1),
                    window.getSize().y / 2 + radius * sin(angle1)
            );
            sf::Vector2f pos2(
                    window.getSize().x / 2 + radius * cos(angle2),
                    window.getSize().y / 2 + radius * sin(angle2)
            );

            sf::RectangleShape line(sf::Vector2f(
                    std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2)),
                    3.0f
            ));
            line.setPosition(pos1);
            line.setFillColor(sf::Color::Green);

            float angle = std::atan2(pos2.y - pos1.y, pos2.x - pos1.x) * 180 / M_PI;
            line.setRotation(angle);

            window.draw(line);
        }
    }

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
        HashTable<T, sf::Vector2f> vertexPositions;
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
        if (show_shortest_path) {
            drawShortestPath();
        }

        for (const auto& vertex : vertices) {
            window.draw(vertex);
        }

        for (const auto& label : vertex_labels) {
            window.draw(label);
        }

        window.display();
    }

public:
    GraphRenderer(const Graph<T>& graph, unsigned int windowWidth, unsigned int windowHeight)
            : graph(graph),
              window(sf::VideoMode(windowWidth, windowHeight), "Graph Renderer"),
              is_colored(false),
              is_mst_colored(false) {
        if (!font.loadFromFile("../externallibs/font.ttf")) {
            throw std::runtime_error("Failed to load font");
        }
        coloring = greedyColoring(graph);
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

    void findShortestPath(const T& start, const T& end) {
        auto result = dijkstra(graph, start, end);
        shortest_path = result.first;
        show_shortest_path = true;
    }
};