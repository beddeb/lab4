#pragma once

#include "hash_table.hpp"


template<typename T>
class HashSet {
private:
    HashTable<T, bool> table;

public:
    explicit HashSet(size_t initial_capacity = 4)
            : table(initial_capacity) {}

    void insert(const T &key) {
        table.insert(key, true);
    }

    bool contains(const T &key) const {
        return table.contains(key);
    }

    void remove(const T &key) {
        table.remove(key);
    }

    [[nodiscard]] size_t size() const {
        return table.size();
    }

    using iterator = typename HashTable<T, bool>::iterator;
    using const_iterator = typename HashTable<T, bool>::const_iterator;

    iterator begin() {
        return table.begin();
    }

    iterator end() {
        return table.end();
    }

    const_iterator begin() const {
        return table.begin();
    }

    const_iterator end() const {
        return table.end();
    }

    [[maybe_unused]] const_iterator cbegin() const {
        return table.begin();
    }

    [[maybe_unused]] const_iterator cend() const {
        return table.end();
    }
};