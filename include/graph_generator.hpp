#pragma once

#include "graph.hpp"
#include <random>


std::random_device rd;
std::mt19937 gen(rd());

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
Graph<T> generateChainGraph(int numVertices, int minWeight, int maxWeight) {
    if (numVertices <= 0) {
        throw std::invalid_argument("Number of vertices must be positive");
    }

    if (minWeight > maxWeight) {
        throw std::invalid_argument("Minimum weight cannot be greater than maximum weight");
    }

    Graph<T> graph;
    std::uniform_int_distribution<> dist(minWeight, maxWeight);

    for (int i = 0; i < numVertices; ++i) {
        graph.addVertex(generateUniqueVertex<T>(i));
    }

    for (int i = 0; i < numVertices - 1; ++i) {
        int weight = dist(gen);
        graph.addEdge(generateUniqueVertex<T>(i), generateUniqueVertex<T>(i + 1), weight);
    }
    return graph;
}

template<typename T>
Graph<T> generateCycleGraph(int numVertices, int minWeight, int maxWeight) {
    if (numVertices <= 0) {
        throw std::invalid_argument("Number of vertices must be positive");
    }

    if (minWeight > maxWeight) {
        throw std::invalid_argument("Minimum weight cannot be greater than maximum weight");
    }

    Graph<T> graph;
    std::uniform_int_distribution<> dist(minWeight, maxWeight);

    for (int i = 0; i < numVertices; ++i) {
        graph.addVertex(generateUniqueVertex<T>(i));
    }

    for (int i = 0; i < numVertices; ++i) {
        int weight = dist(gen);
        graph.addEdge(generateUniqueVertex<T>(i), generateUniqueVertex<T>((i + 1) % numVertices), weight);
    }
    return graph;
}

template<typename T>
Graph<T> generateCompleteGraph(int numVertices, int minWeight, int maxWeight) {
    if (numVertices <= 0) {
        throw std::invalid_argument("Number of vertices must be positive");
    }

    if (minWeight > maxWeight) {
        throw std::invalid_argument("Minimum weight cannot be greater than maximum weight");
    }

    Graph<T> graph;
    std::uniform_int_distribution<> dist(minWeight, maxWeight);

    for (int i = 0; i < numVertices; ++i) {
        graph.addVertex(generateUniqueVertex<T>(i));
    }

    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            int weight = dist(gen);
            graph.addEdge(generateUniqueVertex<T>(i), generateUniqueVertex<T>(j), weight);
        }
    }
    return graph;
}