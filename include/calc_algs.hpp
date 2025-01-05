#pragma once

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

template <typename T>
std::vector<size_t> greedyColoring(Graph<T> graph) {
    size_t vertex_count = graph.getVertexCount();
    std::vector<size_t> colors(vertex_count, -1);
    HashTable<T, size_t> vertex_index_m;
    size_t index = 0;

    for (const auto& pair : graph.getAdjacencyList()) {
        vertex_index_m[pair.first] = index++;
    }

    index = 0;
    for (const auto& pair : graph.getAdjacencyList()) {
        const T& currentVertex = pair.first;
        std::vector<bool> available(vertex_count, true);

        const auto& neighbors = graph.getNeighbors(currentVertex);
        for (size_t j = 0; j < neighbors.getSize(); ++j) {
            const T& neighbor = neighbors.get(j);
            if (colors[vertex_index_m[neighbor]] != -1) {
                available[colors[vertex_index_m[neighbor]]] = false;
            }
        }

        for (size_t c = 0; c < vertex_count; ++c) {
            if (available[c]) {
                colors[vertex_index_m[currentVertex]] = c;
                break;
            }
        }
        index++;
    }
    return colors;
}

template <typename T>
class DisjointSet {
private:
    HashTable<T, T> parent;
    HashTable<T, int> rank;

public:
    void makeSet(const T& vertex) {
        parent[vertex] = vertex;
        rank[vertex] = 0;
    }

    T findSet(const T& vertex) {
        if (parent[vertex] != vertex) {
            parent[vertex] = findSet(parent[vertex]);
        }
        return parent[vertex];
    }

    void unionSets(const T& vertex1, const T& vertex2) {
        T root1 = findSet(vertex1);
        T root2 = findSet(vertex2);

        if (root1 != root2) {
            if (rank[root1] < rank[root2]) {
                parent[root1] = root2;
            } else if (rank[root1] > rank[root2]) {
                parent[root2] = root1;
            } else {
                parent[root2] = root1;
                rank[root1]++;
            }
        }
    }
};

template <typename T>
ArraySequence<Edge<T>> kruskal(const Graph<T>& graph) {
    ArraySequence<Edge<T>> mst;
    DisjointSet<T> disjointSet;

    for (const auto& vertex : graph.getAdjacencyList()) {
        disjointSet.makeSet(vertex.first);
    }

    ArraySequence<Edge<T>> edges = graph.getEdges();
    edges.sort([](const Edge<T>& a, const Edge<T>& b) {
        return a.weight < b.weight;
    });

    for (const auto& edge : edges) {
        if (disjointSet.findSet(edge.vertex1) != disjointSet.findSet(edge.vertex2)) {
            mst.add(edge);
            disjointSet.unionSets(edge.vertex1, edge.vertex2);
        }
    }
    return mst;
}