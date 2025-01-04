#include <iostream>
#include <chrono>
#include <vector>
#include <set>
#include "chrono_set.hpp"
#include "demangle.hpp"


using namespace std;
using namespace chrono;

template<typename T, typename Container>
void chronoInsertSet(size_t elementCount) {
    Container set;
    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < elementCount; ++i) {
        if constexpr (is_same_v<Container, HashSet<T>>) {
            if constexpr (is_same_v<T, std::string>) {
                set.insert(to_string(i));
            } else {
                set.insert(i);
            }
        } else {
            if constexpr (is_same_v<T, std::string>) {
                set.insert(to_string(i));
            } else {
                set.insert(i);
            }
        }
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Insertion time for " << elementCount << " elements " << demangle(typeid(Container).name()) << ": "
         << duration.count() << " ms" << endl;
}

template<typename T, typename Container>
void chronoRemoveSet(size_t elementCount) {
    Container set;
    std::vector<T> elements;
    for (size_t i = 0; i < elementCount; ++i) {
        if constexpr (is_same_v<Container, HashSet<T>>) {
            if constexpr (is_same_v<T, std::string>) {
                set.insert(to_string(i));
                elements.push_back(to_string(i));
            } else {
                set.insert(i);
                elements.push_back(i);
            }
        } else {
            if constexpr (is_same_v<T, std::string>) {
                set.insert(to_string(i));
                elements.push_back(to_string(i));
            } else {
                set.insert(i);
                elements.push_back(i);
            }
        }
    }
    auto start = high_resolution_clock::now();
    if constexpr (is_same_v<Container, HashSet<T>>) {
        for (const auto &element: elements) {
            set.remove(element);
        }
    } else {
        for (const auto &element: elements) {
            set.erase(element);
        }
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Removal time for " << elementCount << " elements " << demangle(typeid(Container).name()) << ": "
         << duration.count() << " ms" << endl;
}

void chronoSet(size_t num_elements) {
    cout << "Set tests for <int>:" << endl;
    chronoInsertSet<int, HashSet<int>>(num_elements);
    chronoInsertSet<int, std::set<int>>(num_elements);
    chronoRemoveSet<int, HashSet<int>>(num_elements);
    chronoRemoveSet<int, std::set<int>>(num_elements);
    cout << endl;

    cout << "Set tests for <string>:" << endl;
    chronoInsertSet<string, HashSet<string>>(num_elements);
    chronoInsertSet<string, std::set<string>>(num_elements);
    chronoRemoveSet<string, HashSet<string>>(num_elements);
    chronoRemoveSet<string, std::set<string>>(num_elements);
    cout << endl;
}