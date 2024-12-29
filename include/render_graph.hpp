#ifndef RENDERER_GRAPH_HPP
#define RENDERER_GRAPH_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "graph.hpp"


template <typename T>
class GraphRenderer {
public:
    GraphRenderer(const Graph<T>& graph, unsigned int windowWidth, unsigned int windowHeight)
            : graph(graph), window(sf::VideoMode(windowWidth, windowHeight), "Graph Renderer") {
        font.loadFromFile("../externallibs/font.ttf");
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    const Graph<T>& graph;
    sf::RenderWindow window;
    std::vector<sf::CircleShape> vertices;
    std::vector<sf::VertexArray> edges;
    std::vector<sf::Text> vertexLabels;
    sf::Font font;

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    void update() {
        // Обновляем позиции вершин и рёбер
        vertices.clear();
        edges.clear();
        vertexLabels.clear();

        // Предполагаем, что вершины расположены равномерно по окружности
        float radius = 150.0f;
        float angleStep = 2 * M_PI / graph.getVertexCount();
        for (size_t i = 0; i < graph.getVertexCount(); ++i) {
            sf::CircleShape vertex(20);
            vertex.setFillColor(sf::Color::White);
            vertex.setOutlineColor(sf::Color::Black);
            vertex.setOutlineThickness(2);
            float angle = i * angleStep;
            vertex.setPosition(
                    window.getSize().x / 2 + radius * cos(angle) - vertex.getRadius(),
                    window.getSize().y / 2 + radius * sin(angle) - vertex.getRadius()
            );
            vertices.push_back(vertex);

            // Добавляем текстовую метку
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

        // Создаем рёбра
        for (size_t i = 0; i < graph.getVertexCount(); ++i) {
            const auto& neighbors = graph.getNeighbors(i);
            for (size_t j = 0; j < neighbors.getSize(); ++j) {
                sf::VertexArray edge(sf::Lines, 2);
                edge[0].position = vertices[i].getPosition() + sf::Vector2f(vertices[i].getRadius(), vertices[i].getRadius());
                edge[1].position = vertices[neighbors.get(j)].getPosition() + sf::Vector2f(vertices[neighbors.get(j)].getRadius(), vertices[neighbors.get(j)].getRadius());
                edge[0].color = sf::Color::Black;
                edge[1].color = sf::Color::Black;
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

#endif // RENDERER_GRAPH_HPP