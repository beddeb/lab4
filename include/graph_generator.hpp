#pragma once

#include "graph.hpp"


template<typename T>
T generateUniqueVertex(int index) {
    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(index);
    } else if constexpr (std::is_same_v<T, std::string>) {
        return "v" + std::to_string(index);
    } else {
        throw std::invalid_argument("Unsupported type for vertex generation");
    }
}

template<typename T>
Graph<T> generateChainGraph(int numVertices) {
    if (numVertices <= 0) {
        throw std::invalid_argument("Number of vertices must be positive");
    }

    Graph<T> graph;
    for (int i = 0; i < numVertices; ++i) {
        graph.addVertex(generateUniqueVertex<T>(i));
    }

    for (int i = 0; i < numVertices - 1; ++i) {
        graph.addEdge(generateUniqueVertex<T>(i), generateUniqueVertex<T>(i + 1));
    }
    return graph;
}

template<typename T>
Graph<T> generateCycleGraph(int numVertices) {
    if (numVertices <= 0) {
        throw std::invalid_argument("Number of vertices must be positive");
    }

    Graph<T> graph;
    for (int i = 0; i < numVertices; ++i) {
        graph.addVertex(generateUniqueVertex<T>(i));
    }

    for (int i = 0; i < numVertices; ++i) {
        graph.addEdge(generateUniqueVertex<T>(i), generateUniqueVertex<T>((i + 1) % numVertices));
    }
    return graph;
}

template<typename T>
Graph<T> generateCompleteGraph(int numVertices) {
    if (numVertices <= 0) {
        throw std::invalid_argument("Number of vertices must be positive");
    }

    Graph<T> graph;
    for (int i = 0; i < numVertices; ++i) {
        graph.addVertex(generateUniqueVertex<T>(i));
    }

    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            graph.addEdge(generateUniqueVertex<T>(i), generateUniqueVertex<T>(j));
        }
    }
    return graph;
}