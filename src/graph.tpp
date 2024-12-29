#pragma once

#include <iostream>
#include "../include/graph.hpp"

// Конструктор копирования
template <typename T>
Graph<T>::Graph(const Graph& other)
        : vertices(other.vertices), adjacencyList(other.adjacencyList) {}

// Конструктор перемещения
template <typename T>
Graph<T>::Graph(Graph&& other) noexcept
        : vertices(std::move(other.vertices)),
          adjacencyList(std::move(other.adjacencyList)) {}

// Оператор присваивания копированием
template <typename T>
Graph<T>& Graph<T>::operator=(const Graph& other) {
    if (this != &other) {
        vertices = other.vertices;
        adjacencyList = other.adjacencyList;
    }
    return *this;
}

// Оператор присваивания перемещением
template <typename T>
Graph<T>& Graph<T>::operator=(Graph&& other) noexcept {
    if (this != &other) {
        vertices = std::move(other.vertices);
        adjacencyList = std::move(other.adjacencyList);
    }
    return *this;
}

// Добавление вершины
template <typename T>
void Graph<T>::addVertex(const Vertex& vertex) {
    vertices.add(vertex);
    adjacencyList.add(ListSequence<size_t>());  // Инициализируем пустой список смежности
}

// Удаление вершины по индексу
template <typename T>
void Graph<T>::removeVertex(size_t index) {
    if (index >= vertices.getSize()) {
        throw std::out_of_range("Vertex index out of range");
    }

    // Удаляем все рёбра, связанные с данной вершиной
    for (size_t i = 0; i < adjacencyList.getSize(); ++i) {
        if (i == index) continue;
        auto& neighbors = adjacencyList.get(i);
        // Используем итерацию в обратном порядке чтобы корректно удалять элементы
        for (size_t j = 0; j < neighbors.getSize(); /* не увеличиваем j */) {
            if (neighbors.get(j) == index) {
                neighbors.remove(j);
                // Не увеличиваем j, так как элементы сдвинулись
            } else {
                // Если индекс вершины больше удалённого, нужно его уменьшить
                if (neighbors.get(j) > index) {
                    neighbors.get(j) -= 1;
                }
                ++j;
            }
        }
    }

    // Удаляем вершину и её список смежности
    vertices.remove(index);
    adjacencyList.remove(index);
}

// Добавление ребра между двумя вершинами по индексам
template <typename T>
void Graph<T>::addEdge(size_t from, size_t to) {
    if (from >= vertices.getSize() || to >= vertices.getSize()) {
        throw std::out_of_range("Vertex index out of range");
    }

    // Избегаем добавления дублирующих рёбер
    auto& fromNeighbors = adjacencyList.get(from);
    for (size_t i = 0; i < fromNeighbors.getSize(); ++i) {
        if (fromNeighbors.get(i) == to) {
            return;  // Ребро уже существует
        }
    }
    fromNeighbors.add(to);

    auto& toNeighbors = adjacencyList.get(to);
    for (size_t i = 0; i < toNeighbors.getSize(); ++i) {
        if (toNeighbors.get(i) == from) {
            return;  // Ребро уже существует
        }
    }
    toNeighbors.add(from);
}

// Удаление ребра между двумя вершинами по индексам
template <typename T>
void Graph<T>::removeEdge(size_t from, size_t to) {
    if (from >= vertices.getSize() || to >= vertices.getSize()) {
        throw std::out_of_range("Vertex index out of range");
    }

    auto& fromNeighbors = adjacencyList.get(from);
    for (size_t i = 0; i < fromNeighbors.getSize(); ++i) {
        if (fromNeighbors.get(i) == to) {
            fromNeighbors.remove(i);
            break;
        }
    }

    auto& toNeighbors = adjacencyList.get(to);
    for (size_t i = 0; i < toNeighbors.getSize(); ++i) {
        if (toNeighbors.get(i) == from) {
            toNeighbors.remove(i);
            break;
        }
    }
}

// Проверка наличия ребра между двумя вершинами
template <typename T>
bool Graph<T>::hasEdge(size_t from, size_t to) const {
    if (from >= vertices.getSize() || to >= vertices.getSize()) {
        throw std::out_of_range("Vertex index out of range");
    }

    const auto& fromNeighbors = adjacencyList.get(from);
    for (size_t i = 0; i < fromNeighbors.getSize(); ++i) {
        if (fromNeighbors.get(i) == to) {
            return true;
        }
    }
    return false;
}

// Получение списка смежных вершин для данной вершины
template <typename T>
ListSequence<size_t> Graph<T>::getNeighbors(size_t index) const {
    if (index >= vertices.getSize()) {
        throw std::out_of_range("Vertex index out of range");
    }
    return adjacencyList.get(index);
}

// Получение количества вершин
template <typename T>
size_t Graph<T>::getVertexCount() const {
    return vertices.getSize();
}

// Получение количества рёбер
template <typename T>
size_t Graph<T>::getEdgeCount() const {
    size_t count = 0;
    for (size_t i = 0; i < adjacencyList.getSize(); ++i) {
        count += adjacencyList.get(i).getSize();
    }
    return count / 2;  // Поскольку граф неориентированный
}

// Получение вершины по индексу
template <typename T>
const typename Graph<T>::Vertex& Graph<T>::getVertex(size_t index) const {
    if (index >= vertices.getSize()) {
        throw std::out_of_range("Vertex index out of range");
    }
    return vertices.get(index);
}

// Рендеринг графа (для отладки)
template <typename T>
void Graph<T>::print() const {
    for (size_t i = 0; i < vertices.getSize(); ++i) {
        std::cout << vertices.get(i) << ": ";
        const auto& neighbors = adjacencyList.get(i);
        for (size_t j = 0; j < neighbors.getSize(); ++j) {
            std::cout << vertices.get(neighbors.get(j)) << " ";
        }
        std::cout << "\n";
    }
}