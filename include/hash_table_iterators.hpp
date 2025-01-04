#pragma once

#include <stdexcept>

template<typename K, typename V>
class HashTable;

template<typename K, typename V>
class HashTableIterator {
private:
    HashTable<K, V>* hash_table;
    size_t bucket_index;
    size_t node_index;

    void findNextValid() {
        while (bucket_index < hash_table->capacity) {
            while (node_index < hash_table->table.get(bucket_index).getSize()) {
                if (hash_table->table.get(bucket_index).get(node_index).isOccupied) {
                    return;
                }
                ++node_index;
            }
            ++bucket_index;
            node_index = 0;
        }
    }

public:
    static HashTableIterator end(HashTable<K, V>* table) {
        return HashTableIterator(table, table->capacity, 0);
    }

    using value_type = std::pair<K, V>;

    explicit HashTableIterator(HashTable<K, V>* table, size_t bucket = 0, size_t node = 0)
            : hash_table(table), bucket_index(bucket), node_index(node) {
        if (!hash_table) {
            throw std::runtime_error("HashTable is NULL");
        }
        findNextValid();
    }

    value_type operator*() const {
        if (bucket_index >= hash_table->capacity) {
            throw std::runtime_error("Iterator is out of range");
        }
        return std::make_pair(hash_table->table.get(bucket_index).get(node_index).key,
                              hash_table->table.get(bucket_index).get(node_index).value);
    }


    HashTableIterator& operator++() {
        ++node_index;
        findNextValid();
        return *this;
    }

    HashTableIterator operator++(int) {
        HashTableIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const HashTableIterator& other) const {
        return hash_table == other.hash_table &&
               bucket_index == other.bucket_index &&
               node_index == other.node_index;
    }

    bool operator!=(const HashTableIterator& other) const {
        return !(*this == other);
    }

    class KeyIterator {
        HashTableIterator it;
    public:
        explicit KeyIterator(const HashTableIterator& it) : it(it) {}
        const K& operator*() const { return std::get<0>(*it); }
        KeyIterator& operator++() { ++it; return *this; }
        KeyIterator operator++(int) { KeyIterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const KeyIterator& other) const { return it == other.it; }
        bool operator!=(const KeyIterator& other) const { return it != other.it; }
    };

    KeyIterator key() const { return KeyIterator(*this); }


    class ValueIterator {
        HashTableIterator it;
    public:
        explicit ValueIterator(const HashTableIterator& it) : it(it) {}
        V& operator*() const {
            return std::get<1>(*it);
        }
        ValueIterator& operator++() { ++it; return *this; }
        ValueIterator operator++(int) { ValueIterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const ValueIterator& other) const { return it == other.it; }
        bool operator!=(const ValueIterator& other) const { return it != other.it; }
    };
    ValueIterator value() const { return ValueIterator(*this); }

};


template<typename K, typename V>
class ConstHashTableIterator {
private:
    const HashTable<K, V>* hash_table;
    size_t bucket_index;
    size_t node_index;

    void findNextValid() {
        while (bucket_index < hash_table->capacity) {
            while (node_index < hash_table->table.get(bucket_index).getSize()) {
                if (hash_table->table.get(bucket_index).get(node_index).isOccupied) {
                    return;
                }
                ++node_index;
            }
            ++bucket_index;
            node_index = 0;
        }
    }

public:
    static ConstHashTableIterator end(const HashTable<K, V>* table) {
        return ConstHashTableIterator(table, table->capacity, 0);
    }

    using value_type = std::pair<const K, const V>;

    explicit ConstHashTableIterator(const HashTable<K, V>* table, size_t bucket = 0, size_t node = 0)
            : hash_table(table), bucket_index(bucket), node_index(node) {
        if (!hash_table) {
            throw std::runtime_error("HashTable is NULL");
        }
        findNextValid();
    }

    value_type operator*() const {
        if (bucket_index >= hash_table->capacity) {
            throw std::runtime_error("Iterator is out of range");
        }
        return std::make_pair(hash_table->table.get(bucket_index).get(node_index).key,
                              hash_table->table.get(bucket_index).get(node_index).value);
    }

    ConstHashTableIterator& operator++() {
        ++node_index;
        findNextValid();
        return *this;
    }

    ConstHashTableIterator operator++(int) {
        ConstHashTableIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const ConstHashTableIterator& other) const {
        return hash_table == other.hash_table &&
               bucket_index == other.bucket_index &&
               node_index == other.node_index;
    }

    bool operator!=(const ConstHashTableIterator& other) const {
        return !(*this == other);
    }

    class KeyIterator {
        ConstHashTableIterator it;
    public:
        explicit KeyIterator(const ConstHashTableIterator& it) : it(it) {}
        const K& operator*() const { return std::get<0>(*it); }
        KeyIterator& operator++() { ++it; return *this; }
        KeyIterator operator++(int) { KeyIterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const KeyIterator& other) const { return it == other.it; }
        bool operator!=(const KeyIterator& other) const { return it != other.it; }
    };

    KeyIterator key() const { return KeyIterator(*this); }


    class ValueIterator {
        ConstHashTableIterator it;
    public:
        explicit ValueIterator(const ConstHashTableIterator& it) : it(it) {}
        const V& operator*() const { return std::get<1>(*it); }
        ValueIterator& operator++() { ++it; return *this; }
        ValueIterator operator++(int) { ValueIterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const ValueIterator& other) const { return it == other.it; }
        bool operator!=(const ValueIterator& other) const { return it != other.it; }
    };

    ValueIterator value() const { return ValueIterator(*this); }
};