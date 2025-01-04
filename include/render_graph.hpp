#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
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
              isColored(false) {
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
        isColored = !isColored;
    }

private:
    const Graph<T>& graph;
    sf::RenderWindow window;
    std::vector<sf::CircleShape> vertices;
    std::vector<sf::VertexArray> edges;
    std::vector<sf::Text> vertexLabels;
    sf::Font font;
    bool isColored;

    std::vector<size_t> coloring;  // Хранит цвета для каждой вершины

    // Жадный алгоритм раскраски графа
    std::vector<size_t> greedyColoring() {
        size_t vertexCount = graph.getVertexCount();
        std::vector<size_t> colors(vertexCount, -1);
        std::map<T, size_t> vertexIndexMap; // Создаем карту соответствия вершины и индекса
        size_t index = 0;

        // Назначаем индексы вершинам, чтобы использовать их в массиве цветов
        for (const auto& pair : graph.getAdjacencyList()) {
            vertexIndexMap[pair.first] = index++;
        }

        index = 0;
        for (const auto& pair : graph.getAdjacencyList()) {
            const T& currentVertex = pair.first;
            std::vector<bool> available(vertexCount, true);

            // Проверяем цвета соседей
            const auto& neighbors = graph.getNeighbors(currentVertex);
            for (size_t j = 0; j < neighbors.getSize(); ++j) {
                const T& neighbor = neighbors.get(j);
                if (colors[vertexIndexMap[neighbor]] != -1) {
                    available[colors[vertexIndexMap[neighbor]]] = false;
                }
            }

            // Находим минимальный доступный цвет
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

        float radius = std::min(window.getSize().x, window.getSize().y) * 0.35f;
        auto adjacencyList = graph.getAdjacencyList();
        size_t vertexCount = adjacencyList.size();

        if (vertexCount == 0) return;

        float angleStep = 2 * M_PI / vertexCount;
        std::map<T, sf::Vector2f> vertexPositions;
        size_t colorIndex = 0;

        // Вычисляем позиции вершин
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

        // Создаем рёбра
        for (const auto& pair : adjacencyList) {
            const T& vertex = pair.first;
            const auto& neighbors = pair.second;
            sf::Vector2f pos1 = vertexPositions[vertex];

            for (size_t i = 0; i < neighbors.getSize(); ++i) {
                const T& neighbor = neighbors.get(i);
                if (vertex < neighbor) { // Рисуем каждое ребро только один раз
                    sf::Vector2f pos2 = vertexPositions[neighbor];

                    sf::VertexArray edge(sf::Lines, 2);
                    edge[0].position = pos1;
                    edge[1].position = pos2;
                    edge[0].color = sf::Color::Black;
                    edge[1].color = sf::Color::Black;
                    edges.push_back(edge);
                }
            }
        }

        // Создаем вершины и метки
        colorIndex = 0;
        for (const auto& pair : adjacencyList) {
            const T& vertex = pair.first;
            sf::Vector2f pos = vertexPositions[vertex];

            // Вершина
            sf::CircleShape vertexShape(20);
            vertexShape.setFillColor(isColored ? getColorFromIndex(coloring[colorIndex]) : sf::Color::White);
            vertexShape.setOutlineColor(sf::Color::Black);
            vertexShape.setOutlineThickness(2);
            vertexShape.setPosition(pos.x - vertexShape.getRadius(), pos.y - vertexShape.getRadius());
            vertices.push_back(vertexShape);

            // Метка
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
            vertexLabels.push_back(label);

            colorIndex++;
        }
    }

    void render() {
        window.clear(sf::Color::White);

        // Сначала рисуем рёбра
        for (const auto& edge : edges) {
            window.draw(edge);
        }

        // Вершины
        for (const auto& vertex : vertices) {
            window.draw(vertex);
        }

        // Метки
        for (const auto& label : vertexLabels) {
            window.draw(label);
        }

        window.display();
    }
};