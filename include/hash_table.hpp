#pragma once

#include <stdexcept>
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "hash_table_iterators.hpp"


struct PairHashInt {
    std::size_t operator()(const std::pair<int, int>& p) const {
        auto hash1 = std::hash<int>{}(p.first);
        auto hash2 = std::hash<int>{}(p.second);
        return hash1 ^ hash2;
    }
};

struct PairHashStr {
    std::size_t operator()(const std::pair<std::string, std::string>& p) const {
        auto hash1 = std::hash<std::string>{}(p.first);
        auto hash2 = std::hash<std::string>{}(p.second);
        return hash1 ^ hash2;
    }
};

template<typename K, typename V>
struct HashNode {
    K key;
    V value;
    bool isOccupied;

    HashNode() : isOccupied(false) {}

    HashNode(const K &k, const V &v) : key(k), value(v), isOccupied(true) {}
};

template<typename K, typename V>
class HashTable {
private:
    template<typename KI, typename VI> friend
    class HashTableIterator;

    template<typename KI, typename VI> friend
    class ConstHashTableIterator;

    static constexpr double compression_threshold = 2.0;  // Пороговое значение для сжатия
    static constexpr double expansion_factor = 1.5;  // Коэффициент расширения/сжатия
    static constexpr size_t default_capacity = 4; // capacity по умолчанию

    ArraySequence<ListSequence<HashNode<K, V>>> table;  // Основная таблица с цепочками
    size_t size_table;
    size_t capacity;

    // Инициализация таблицы пустыми бакетами
    void initializeTable() {
        for (size_t i = 0; i < capacity; ++i) {
            table.add(ListSequence<HashNode<K, V>>());
        }
    }

    // Определение базовой хеш-функции
    size_t getHashCode(const K &key) const {
        if constexpr (std::is_same_v<K, std::pair<int, int>>) {
            PairHashInt ph;
            return ph(key);
        } else if constexpr (std::is_same_v<K, std::pair<std::string, std::string>>) {
            PairHashStr ph;
            return ph(key);
        } else {
            return std::hash<K>{}(key);
        }
    }

    // Получение индекса в таблице
    size_t getIndex(const K &key) const {
        return getHashCode(key) % capacity;
    }

    // Перестройка таблицы
    void rebuild(size_t new_capacity) {
        ArraySequence<ListSequence<HashNode<K, V>>> newTable(new_capacity);
        for (size_t i = 0; i < new_capacity; ++i) {
            newTable.add(ListSequence<HashNode<K, V>>());
        }

        for (auto it = table.begin(); it != table.end(); ++it) {
            for (auto inner_it = (*it).begin(); inner_it != (*it).end(); ++inner_it) {
                const auto &node = *inner_it;
                if (node.isOccupied) {
                    size_t newIndex = getHashCode(node.key) % new_capacity;
                    newTable.get(newIndex).add(node);
                }
            }
        }
        table = std::move(newTable);
        capacity = new_capacity;
    }

    void checkAndResize() {
        if (size_table == capacity) {
            rebuild(static_cast<size_t>(capacity * expansion_factor));
        } else if (size_table <= capacity / compression_threshold && capacity > default_capacity) {
            rebuild(static_cast<size_t>(capacity / expansion_factor));
        }
    }

    ListSequence<HashNode<K, V>>& getBucket(const K& key) {
        size_t index = getIndex(key);
        return table.get(index);
    }

    const ListSequence<HashNode<K, V>>& getBucket(const K& key) const {
        size_t index = getIndex(key);
        return table.get(index);
    }

    bool isMatchingNode(const HashNode<K, V>& node, const K& key) const {
        return node.isOccupied && node.key == key;
    }

    const HashNode<K, V>* findNode(const ListSequence<HashNode<K, V>>& bucket, const K& key) const {
        for (const auto& node : bucket) {
            if (isMatchingNode(node, key)) {
                return &node;
            }
        }
        return nullptr;
    }

    HashNode<K,V>* findNode(ListSequence<HashNode<K, V>>& bucket, const K& key) {
        for (auto& node : bucket) {
            if (isMatchingNode(node, key)) {
                return &node;
            }
        }
        return nullptr;
    }


public:
    explicit HashTable(size_t initial_capacity = default_capacity)
            : size_table(0), capacity(initial_capacity),
              table(initial_capacity) {
        initializeTable();
    }

    void insert(const K &key, const V &value) {
        ListSequence<HashNode<K, V>> &bucket = getBucket(key);

        for (auto& node : bucket) {
            if (isMatchingNode(node, key)) {
                node.value = value;
                return;
            }
        }
        bucket.add(HashNode<K, V>(key, value));
        ++size_table;
        checkAndResize();
    }

    const V &get(const K &key) const {
        const ListSequence<HashNode<K, V>> &bucket = getBucket(key);
        const HashNode<K, V>* node = findNode(bucket, key);

        if (node != nullptr) {
            return node->value;
        }
        throw std::runtime_error("Key not found");
    }

    V &get(const K &key)  {
        ListSequence<HashNode<K, V>> &bucket = getBucket(key);
        HashNode<K, V>* node = findNode(bucket, key);
        if (node != nullptr) {
            return node->value;
        }
        throw std::runtime_error("Key not found");
    }

    void remove(const K &key) {
        ListSequence<HashNode<K, V>> &bucket = getBucket(key);
        for (auto& node : bucket) {
            if (isMatchingNode(node, key)) {
                node.isOccupied = false;
                --size_table;
                checkAndResize();
                return;
            }
        }
        throw std::runtime_error("Key not found");
    }

    [[nodiscard]] size_t size() const {
        return size_table;
    }

    bool contains(const K &key) const {
        const ListSequence<HashNode<K, V>> &bucket = getBucket(key);
        return findNode(bucket, key) != nullptr;
    }

    V& operator[](const K& key) {
        ListSequence<HashNode<K, V>>& bucket = getBucket(key);
        HashNode<K, V>* node = findNode(bucket, key);

        if (node != nullptr) {
            return node->value;
        }

        insert(key, V());
        return get(key);
    }

    const V& operator[](const K& key) const {
        return get(key);
    }

    using iterator = HashTableIterator<K, V>;
    using const_iterator = ConstHashTableIterator<K, V>;

    iterator begin() {
        return iterator(this);
    }

    iterator end() {
        return iterator(this, capacity, 0);
    }

    const_iterator begin() const {
        return const_iterator(this);
    }

    const_iterator end() const {
        return const_iterator(this, capacity, 0);
    }

    [[maybe_unused]] const_iterator cbegin() const {
        return const_iterator(this);
    }

    [[maybe_unused]] const_iterator cend() const {
        return const_iterator(this, capacity, 0);
    }
};