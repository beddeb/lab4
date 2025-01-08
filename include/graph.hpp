#pragma once

#include <stdexcept>
#include "hash_set.hpp"


template <typename T>
class Edge {
public:
    using vertex_type = T;

    T vertex1;
    T vertex2;
    int weight;

    Edge() : weight(0) {}

    Edge(const T& v1, const T& v2, int w = 0)
            : vertex1(v1), vertex2(v2), weight(w) {}

    bool operator==(const Edge& other) const {
        return vertex1 == other.vertex1 &&
               vertex2 == other.vertex2 &&
               std::abs(weight - other.weight) < 1e-9;
    }

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

template<typename T>
class Graph {
private:
    HashTable<T, ArraySequence<T>> adjacency_list;
    ArraySequence<Edge<T>> edges;

public:
    Graph() = default;

    Graph(const Graph& other)
            : adjacency_list(other.adjacency_list),
              edges(other.edges) {}

    Graph(Graph&& other) noexcept
            : adjacency_list(std::move(other.adjacency_list)),
              edges(std::move(other.edges)) {}

    Graph& operator=(const Graph& other) {
        if (this != &other) {
            adjacency_list = other.adjacency_list;
            edges = other.edges;
        }
        return *this;
    }

    Graph& operator=(Graph&& other) noexcept {
        if (this != &other) {
            adjacency_list = std::move(other.adjacency_list);
            edges = std::move(other.edges);
        }
        return *this;
    }

    void addVertex(const T &vertex) {
        if (!adjacency_list.contains(vertex)) {
            adjacency_list.insert(vertex, ArraySequence<T>());
        }
    }

    void addEdge(const T &vertex1, const T &vertex2, int weight = 0) {
        if (!hasVertex(vertex1) || !hasVertex(vertex2)) {
            throw std::runtime_error("Both vertices must exist in the graph before adding an edge");
        }

        if (!adjacency_list.get(vertex1).contains(vertex2)) {
            adjacency_list.get(vertex1).add(vertex2);
        }

        if (!adjacency_list.get(vertex2).contains(vertex1)) {
            adjacency_list.get(vertex2).add(vertex1);
        }

        for (auto& edge : edges) {
            if ((edge.vertex1 == vertex1 && edge.vertex2 == vertex2) ||
                (edge.vertex1 == vertex2 && edge.vertex2 == vertex1)) {
                edge.weight = weight;
                return;
            }
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
            throw std::runtime_error("One or both vertexes not found");
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
            ArraySequence<T>& adj_neighbors = adjacency_list.get(adjacentVertex);
            adj_neighbors.removeElement(vertex);
        }

        adjacency_list.remove(vertex);

        ArraySequence<Edge<T>> new_edges;
        for (const auto& edge : edges) {
            if (edge.vertex1 != vertex && edge.vertex2 != vertex) {
                new_edges.add(edge);
            }
        }
        edges = new_edges;
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
                    edges.add(std::make_pair(vertex, neighbor));
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