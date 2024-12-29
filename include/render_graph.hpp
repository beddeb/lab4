#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include "graph.hpp"

template <typename T>
class GraphRenderer {
public:
    GraphRenderer(const Graph<T>& graph, unsigned int windowWidth, unsigned int windowHeight)
            : graph(graph), window(sf::VideoMode(windowWidth, windowHeight), "Graph Renderer"), isColored(false), highlightMST(false) {
        font.loadFromFile("../externallibs/font.ttf");
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

    // Метод для раскраски графа по правилам
    void colorizeGraph() {
        isColored = true;
        coloring = greedyColoring();
    }

    // Метод для выделения остова графа
    void highlightSpanningTree() {
        highlightMST = true;
        spanningTreeEdges = primMST();
    }

private:
    const Graph<T>& graph;
    sf::RenderWindow window;
    std::vector<sf::CircleShape> vertices;
    std::vector<sf::VertexArray> edges;
    std::vector<sf::Text> vertexLabels;
    sf::Font font;
    bool isColored;  // Флаг для раскраски
    bool highlightMST;  // Флаг для выделения остова
    std::vector<size_t> coloring;  // Хранит цвета для каждой вершины
    std::vector<std::pair<size_t, size_t>> spanningTreeEdges;  // Хранит рёбра остова

    // Жадный алгоритм раскраски графа
    std::vector<size_t> greedyColoring() {
        size_t vertexCount = graph.getVertexCount();
        std::vector<size_t> colors(vertexCount, -1);  // Изначально все вершины не раскрашены
        colors[0] = 0;  // Первая вершина получает первый цвет

        for (size_t i = 1; i < vertexCount; ++i) {
            std::vector<bool> available(vertexCount, true);  // Доступные цвета

            // Проверяем цвета соседей
            const auto& neighbors = graph.getNeighbors(i);
            for (size_t j = 0; j < neighbors.getSize(); ++j) {
                size_t neighborIndex = neighbors.get(j);
                if (colors[neighborIndex] != -1) {
                    available[colors[neighborIndex]] = false;
                }
            }

            // Находим минимальный доступный цвет
            for (size_t c = 0; c < vertexCount; ++c) {
                if (available[c]) {
                    colors[i] = c;
                    break;
                }
            }
        }

        return colors;
    }

    // Алгоритм Прима для построения минимального остовного дерева
    std::vector<std::pair<size_t, size_t>> primMST() {
        size_t vertexCount = graph.getVertexCount();
        std::vector<bool> inMST(vertexCount, false);  // Вершины, включённые в MST
        std::vector<std::pair<size_t, size_t>> mstEdges;  // Рёбра MST

        // Очередь с приоритетом для выбора рёбер с минимальным весом
        using Edge = std::pair<size_t, size_t>;
        auto compare = [](const Edge& a, const Edge& b) { return a.second > b.second; };
        std::priority_queue<Edge, std::vector<Edge>, decltype(compare)> pq(compare);

        // Начинаем с вершины 0
        pq.push({0, 0});
        inMST[0] = true;

        while (!pq.empty()) {
            size_t u = pq.top().first;
            pq.pop();

            const auto& neighbors = graph.getNeighbors(u);
            for (size_t i = 0; i < neighbors.getSize(); ++i) {
                size_t v = neighbors.get(i);
                if (!inMST[v]) {
                    inMST[v] = true;
                    mstEdges.push_back({u, v});
                    pq.push({v, 1});  // Вес рёбер предполагается равным 1
                }
            }
        }

        return mstEdges;
    }

    // Проверяет, является ли ребро частью MST
    bool isEdgeInSpanningTree(size_t from, size_t to) {
        for (const auto& edge : spanningTreeEdges) {
            if ((edge.first == from && edge.second == to) || (edge.first == to && edge.second == from)) {
                return true;
            }
        }
        return false;
    }

// Преобразует индекс цвета в SFML Color
    sf::Color getColorFromIndex(size_t index) {
        static const std::vector<sf::Color> colors = {
                sf::Color::Red,
                sf::Color::Green,
                sf::Color::Blue,
                sf::Color::Yellow,
                sf::Color::Cyan,
                sf::Color::Magenta,
                sf::Color::White,
                sf::Color::Black
        };
        return colors[index % colors.size()];
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
        vertexLabels.clear();

        float radius = 150.0f;
        float angleStep = 2 * M_PI / graph.getVertexCount();
        for (size_t i = 0; i < graph.getVertexCount(); ++i) {
            sf::CircleShape vertex(20);
            // Если флаг isColored установлен, раскрашиваем вершины
            if (isColored) {
                size_t colorIndex = coloring[i];
                sf::Color color = getColorFromIndex(colorIndex);
                vertex.setFillColor(color);
            } else {
                vertex.setFillColor(sf::Color::White);
            }
            vertex.setOutlineColor(sf::Color::Black);
            vertex.setOutlineThickness(2);
            float angle = i * angleStep;
            vertex.setPosition(
                    window.getSize().x / 2 + radius * cos(angle) - vertex.getRadius(),
                    window.getSize().y / 2 + radius * sin(angle) - vertex.getRadius()
            );
            vertices.push_back(vertex);

            sf::Text label;
            label.setFont(font);
            label.setString(graph.getVertex(i));
            label.setCharacterSize(20);
            label.setFillColor(sf::Color::Black);
            label.setPosition(
                    window.getSize().x / 2 + radius * cos(angle) - label.getLocalBounds().width / 2,
                    window.getSize().y / 2 + radius * sin(angle) - label.getLocalBounds().height / 2
            );
            vertexLabels.push_back(label);
        }

        for (size_t i = 0; i < graph.getVertexCount(); ++i) {
            const auto& neighbors = graph.getNeighbors(i);
            for (size_t j = 0; j < neighbors.getSize(); ++j) {
                size_t to = neighbors.get(j);
                // Проверяем, является ли ребро частью MST
                bool isMSTEdge = highlightMST && isEdgeInSpanningTree(i, to);
                sf::Color edgeColor = isMSTEdge ? sf::Color::Red : sf::Color::Black;

                sf::VertexArray edge(sf::Lines, 2);
                edge[0].position = vertices[i].getPosition() + sf::Vector2f(vertices[i].getRadius(), vertices[i].getRadius());
                edge[1].position = vertices[to].getPosition() + sf::Vector2f(vertices[to].getRadius(), vertices[to].getRadius());
                edge[0].color = edgeColor;
                edge[1].color = edgeColor;
                edges.push_back(edge);
            }
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
        for (const auto& label : vertexLabels) {
            window.draw(label);
        }
        window.display();
    }
};