#pragma once

#include <stdexcept>
#include "set.hpp"

template<typename T>
struct Edge {
    T vertex1;
    T vertex2;
    int weight;

    bool operator==(const Edge& other) const {
        return vertex1 == other.vertex1 && vertex2 == other.vertex2 && weight == other.weight;
    }
};

template<typename T>
class Graph {
private:
    HashTable<T, ArraySequence<T>> adjacency_list;
    ArraySequence<Edge<T>> edges;

public:
    void addVertex(const T &vertex) {
        if (!adjacency_list.contains(vertex)) {
            adjacency_list.insert(vertex, ArraySequence<T>());
        }
    }

    void addEdge(const T &vertex1, const T &vertex2, int weight = 0) {
        addVertex(vertex1);
        addVertex(vertex2);

        ArraySequence<T>& neighbors1 = adjacency_list.get(vertex1);
        ArraySequence<T>& neighbors2 = adjacency_list.get(vertex2);

        if (!neighbors1.contains(vertex2)) {
            neighbors1.add(vertex2);
        }

        if (!neighbors2.contains(vertex1)) {
            neighbors2.add(vertex1);
        }

        edges.add(Edge<T>{vertex1, vertex2, weight});
    }

    bool hasVertex(const T &vertex) const {
        return adjacency_list.contains(vertex);
    }

    bool hasEdge(const T &vertex1, const T &vertex2) const {
        if (!hasVertex(vertex1) || !hasVertex(vertex2)) {
            return false;
        }
        const ArraySequence<T>& neighbors = adjacency_list.get(vertex1);
        return neighbors.contains(vertex2);
    }

    ArraySequence<T> getNeighbors(const T &vertex) const {
        if (!hasVertex(vertex)) {
            throw std::runtime_error("Vertex not found");
        }
        return adjacency_list.get(vertex);
    }

    void removeEdge(const T &vertex1, const T &vertex2) {
        if (!hasVertex(vertex1) || !hasVertex(vertex2)) {
            return;
        }

        ArraySequence<T>& neighbors1 = adjacency_list.get(vertex1);
        neighbors1.removeElement(vertex2);

        ArraySequence<T>& neighbors2 = adjacency_list.get(vertex2);
        neighbors2.removeElement(vertex1);

        for (auto& edge : edges) {
            if (edge.vertex1 == vertex1 && edge.vertex2 == vertex2) {
                edges.removeElement(edge);
                break;
            }
        }
    }

    void removeVertex(const T &vertex) {
        if(!hasVertex(vertex)) {
            throw std::runtime_error("Vertex not found");
        }

        ArraySequence<T> neighbors = adjacency_list.get(vertex);
        for (size_t i = 0; i < neighbors.getSize(); ++i) {
            const T& adjacentVertex = neighbors.get(i);
            ArraySequence<T>& adjNeighbors = adjacency_list.get(adjacentVertex);
            adjNeighbors.removeElement(vertex);
        }

        adjacency_list.remove(vertex);

        for (auto& edge : edges) {
            if (edge.vertex1 == vertex || edge.vertex2 == vertex) {
                edges.removeElement(edge);
                break;
            }
        }
    }

    size_t getVertexCount() const {
        return adjacency_list.size();
    }

    size_t getEdgeCount() const {
        size_t edgeCount = 0;
        HashSet<std::pair<T, T>> edges;
        for (const auto& pair : adjacency_list) {
            const T& vertex = pair.first;
            const ArraySequence<T>& neighbors = pair.second;
            for (size_t i = 0; i < neighbors.getSize(); ++i) {
                const T& neighbor = neighbors.get(i);
                if (vertex < neighbor) {
                    edges.insert(std::make_pair(vertex, neighbor));
                }
            }
        }
        return edges.size();
    }

    HashTable<T, ArraySequence<T>> getAdjacencyList() const {
        return adjacency_list;
    }

    ArraySequence<Edge<T>> getEdges() const {
        return edges;
    }
};