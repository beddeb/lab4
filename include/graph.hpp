#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "list_sequence.hpp"
#include "array_sequence.hpp"


template <typename T>
class Graph {
public:
    using Vertex = T;

private:
    // Хранилище вершин
    ArraySequence<Vertex> vertices;

    // Список смежности: для каждой вершины хранится список её соседей
    ArraySequence<ListSequence<size_t>> adjacencyList;

public:
    Graph() = default;

    // Конструктор копирования
    Graph(const Graph& other);

    // Конструктор перемещения
    Graph(Graph&& other) noexcept;

    // Деструктор
    ~Graph() = default;

    // Оператор присваивания копированием
    Graph& operator=(const Graph& other);

    // Оператор присваивания перемещением
    Graph& operator=(Graph&& other) noexcept;

    // Добавление вершины
    void addVertex(const Vertex& vertex);

    // Удаление вершины по индексу
    void removeVertex(size_t index);

    // Добавление ребра между двумя вершинами по индексам
    void addEdge(size_t from, size_t to);

    // Удаление ребра между двумя вершинами по индексам
    void removeEdge(size_t from, size_t to);

    // Проверка наличия ребра между двумя вершинами
    [[nodiscard]] bool hasEdge(size_t from, size_t to) const;

    // Получение списка смежных вершин для данной вершины
    [[nodiscard]] ListSequence<size_t> getNeighbors(size_t index) const;

    // Получение количества вершин
    [[nodiscard]] size_t getVertexCount() const;

    // Получение количества рёбер
    [[nodiscard]] size_t getEdgeCount() const;

    // Получение вершины по индексу
    const Vertex& getVertex(size_t index) const;

    // Рендеринг графа (для отладки)
    void print() const;
};

// Подключаем файл с определениями методов шаблона
#include "../src/graph.tpp"

#endif // GRAPH_HPP