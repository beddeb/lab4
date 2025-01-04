#pragma once

#include "array_sequence.hpp"
#include "hash_table.hpp"
#include <stdexcept>


template<typename T>
class Graph {
private:
    HashTable<T, ArraySequence<T>> adjacencyList;

public:
    // Добавление вершины
    void addVertex(const T &vertex) {
        if (!adjacencyList.contains(vertex)) {
            adjacencyList.insert(vertex, ArraySequence<T>());
        }
    }

    // Добавление ребра (неориентированного)
    void addEdge(const T &vertex1, const T &vertex2) {
        addVertex(vertex1);
        addVertex(vertex2);

        ArraySequence<T>& neighbors1 = adjacencyList.get(vertex1);
        ArraySequence<T>& neighbors2 = adjacencyList.get(vertex2);

        // Проверка и добавление vertex2 в список соседей vertex1
        if (!neighbors1.contains(vertex2)) {
            neighbors1.add(vertex2);
        }

        // Проверка и добавление vertex1 в список соседей vertex2
        if (!neighbors2.contains(vertex1)) {
            neighbors2.add(vertex1);
        }
    }

    // Проверка наличия вершины
    bool hasVertex(const T &vertex) const {
        return adjacencyList.contains(vertex);
    }

    // Проверка наличия ребра
    bool hasEdge(const T &vertex1, const T &vertex2) const {
        if (!hasVertex(vertex1) || !hasVertex(vertex2)) {
            return false;
        }
        const ArraySequence<T>& neighbors = adjacencyList.get(vertex1);
        return neighbors.contains(vertex2);
    }

    // Получение соседей вершины
    ArraySequence<T> getNeighbors(const T &vertex) const {
        if (!hasVertex(vertex)) {
            throw std::runtime_error("Vertex not found");
        }
        return adjacencyList.get(vertex);
    }

    // Удаление ребра
    void removeEdge(const T &vertex1, const T &vertex2) {
        if (!hasVertex(vertex1) || !hasVertex(vertex2)) {
            return;
        }

        ArraySequence<T>& neighbors1 = adjacencyList.get(vertex1);
        neighbors1.removeElement(vertex2);

        ArraySequence<T>& neighbors2 = adjacencyList.get(vertex2);
        neighbors2.removeElement(vertex1);
    }

    // Удаление вершины
    void removeVertex(const T &vertex) {
        if(!hasVertex(vertex)) {
            throw std::runtime_error("Vertex not found");
        }

        ArraySequence<T> neighbors = adjacencyList.get(vertex);
        for (size_t i = 0; i < neighbors.getSize(); ++i) {
            const T& adjacentVertex = neighbors.get(i);
            ArraySequence<T>& adjNeighbors = adjacencyList.get(adjacentVertex);
            adjNeighbors.removeElement(vertex);
        }

        adjacencyList.remove(vertex);
    }

    // Получение количества вершин
    size_t getVertexCount() const {
        return adjacencyList.size();
    }

    // Получение количества ребер
    size_t getEdgeCount() const {
        size_t edgeCount = 0;
        for (const auto& pair : adjacencyList) {
            edgeCount += pair.second.getSize();
        }
        return edgeCount / 2;
    }

    // Получение списка смежности
    HashTable<T, ArraySequence<T>> getAdjacencyList() const {
        return adjacencyList;
    }
};