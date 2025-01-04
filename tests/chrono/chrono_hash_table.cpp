#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_map>
#include "chrono_hash_table.hpp"
#include "demangle.hpp"


using namespace std;
using namespace chrono;

template<typename K, typename V, typename Container>
void chronoInsertionTable(size_t elementCount) {
    Container table;
    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < elementCount; ++i) {
        if constexpr (is_same_v<Container, HashTable<K, V>>) {
            if constexpr (std::is_same_v<K, std::string> && std::is_same_v<V, std::string>) {
                table.insert(to_string(i), to_string(i));
            } else if constexpr (std::is_same_v<K, std::string>) {
                table.insert(to_string(i), i);
            } else if constexpr (std::is_same_v<V, std::string>) {
                table.insert(i, to_string(i));
            } else {
                table.insert(i, i);
            }
        } else {
            if constexpr (std::is_same_v<K, std::string> && std::is_same_v<V, std::string>) {
                table[to_string(i)] = to_string(i);
            } else if constexpr (std::is_same_v<K, std::string>) {
                table[to_string(i)] = i;
            } else if constexpr (std::is_same_v<V, std::string>) {
                table[i] = to_string(i);
            } else {
                table[i] = i;
            }
        }
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Insertion time for " << elementCount << " elements " << demangle(typeid(Container).name()) << ": "
         << duration.count() << " ms" << endl;
}

template<typename K, typename V, typename Container>
void chronoGetTable(size_t elementCount) {
    Container table;
    std::vector<K> keys;
    for (size_t i = 0; i < elementCount; ++i) {
        if constexpr (is_same_v<Container, HashTable<K, V>>) {
            if constexpr (std::is_same_v<K, std::string> && std::is_same_v<V, std::string>) {
                table.insert(to_string(i), to_string(i));
                keys.push_back(to_string(i));
            } else if constexpr (std::is_same_v<K, std::string>) {
                table.insert(to_string(i), i);
                keys.push_back(to_string(i));
            } else if constexpr (std::is_same_v<V, std::string>) {
                table.insert(i, to_string(i));
                keys.push_back(i);
            } else {
                table.insert(i, i);
                keys.push_back(i);
            }
        } else {
            if constexpr (std::is_same_v<K, std::string> && std::is_same_v<V, std::string>) {
                table[to_string(i)] = to_string(i);
                keys.push_back(to_string(i));
            } else if constexpr (std::is_same_v<K, std::string>) {
                table[to_string(i)] = i;
                keys.push_back(to_string(i));
            } else if constexpr (std::is_same_v<V, std::string>) {
                table[i] = to_string(i);
                keys.push_back(i);
            } else {
                table[i] = i;
                keys.push_back(i);
            }
        }
    }
    auto start = chrono::high_resolution_clock::now();
    if constexpr (is_same_v<Container, HashTable<K, V>>) {
        for (const auto &key: keys) {
            table.get(key);
        }
    } else {
        for (const auto &key: keys) {
            table.at(key);
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Get time for " << elementCount << " elements " << demangle(typeid(Container).name()) << ": "
         << duration.count() << " ms" << endl;
}

template<typename K, typename V, typename Container>
void chronoRemovalTable(size_t elementCount) {
    Container table;
    std::vector<K> keys;
    for (size_t i = 0; i < elementCount; ++i) {
        if constexpr (is_same_v<Container, HashTable<K, V>>) {
            if constexpr (std::is_same_v<K, std::string> && std::is_same_v<V, std::string>) {
                table.insert(to_string(i), to_string(i));
                keys.push_back(to_string(i));
            } else if constexpr (std::is_same_v<K, std::string>) {
                table.insert(to_string(i), i);
                keys.push_back(to_string(i));
            } else if constexpr (std::is_same_v<V, std::string>) {
                table.insert(i, to_string(i));
                keys.push_back(i);
            } else {
                table.insert(i, i);
                keys.push_back(i);
            }
        } else {
            if constexpr (std::is_same_v<K, std::string> && std::is_same_v<V, std::string>) {
                table[to_string(i)] = to_string(i);
                keys.push_back(to_string(i));
            } else if constexpr (std::is_same_v<K, std::string>) {
                table[to_string(i)] = i;
                keys.push_back(to_string(i));
            } else if constexpr (std::is_same_v<V, std::string>) {
                table[i] = to_string(i);
                keys.push_back(i);
            } else {
                table[i] = i;
                keys.push_back(i);
            }
        }
    }
    auto start = chrono::high_resolution_clock::now();
    if constexpr (is_same_v<Container, HashTable<K, V>>) {
        for (const auto &key: keys) {
            table.remove(key);
        }
    } else {
        for (const auto &key: keys) {
            table.erase(key);
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Removal time for " << elementCount << " elements " << demangle(typeid(Container).name()) << ": "
         << duration.count() << " ms" << endl;
}


void chronoHashTable(size_t num_elements) {
    cout << "Comparing HashTable and std::unordered_map for <int, int>:" << endl;
    chronoInsertionTable<int, int, HashTable<int, int>>(num_elements);
    chronoInsertionTable<int, int, std::unordered_map<int, int>>(num_elements);
    chronoGetTable<int, int, HashTable<int, int>>(num_elements);
    chronoGetTable<int, int, std::unordered_map<int, int>>(num_elements);
    chronoRemovalTable<int, int, HashTable<int, int>>(num_elements);
    chronoRemovalTable<int, int, std::unordered_map<int, int>>(num_elements);
    cout << endl;

    cout << "Comparing HashTable and std::unordered_map for <int, string>:" << endl;
    chronoInsertionTable<int, string, HashTable<int, string>>(num_elements);
    chronoInsertionTable<int, string, std::unordered_map<int, string>>(num_elements);
    chronoGetTable<int, string, HashTable<int, string>>(num_elements);
    chronoGetTable<int, string, std::unordered_map<int, string>>(num_elements);
    chronoRemovalTable<int, string, HashTable<int, string>>(num_elements);
    chronoRemovalTable<int, string, std::unordered_map<int, string>>(num_elements);
    cout << endl;

    cout << "Comparing HashTable and std::unordered_map for <string, int>:" << endl;
    chronoInsertionTable<string, int, HashTable<string, int>>(num_elements);
    chronoInsertionTable<string, int, std::unordered_map<string, int>>(num_elements);
    chronoGetTable<string, int, HashTable<string, int>>(num_elements);
    chronoGetTable<string, int, std::unordered_map<string, int>>(num_elements);
    chronoRemovalTable<string, int, HashTable<string, int>>(num_elements);
    chronoRemovalTable<string, int, std::unordered_map<string, int>>(num_elements);
    cout << endl;

    cout << "Comparing HashTable and std::unordered_map for <string, string>:" << endl;
    chronoInsertionTable<string, string, HashTable<string, string>>(num_elements);
    chronoInsertionTable<string, string, std::unordered_map<string, string>>(num_elements);
    chronoGetTable<string, string, HashTable<string, string>>(num_elements);
    chronoGetTable<string, string, std::unordered_map<string, string>>(num_elements);
    chronoRemovalTable<string, string, HashTable<string, string>>(num_elements);
    chronoRemovalTable<string, string, std::unordered_map<string, string>>(num_elements);
    cout << endl;
}