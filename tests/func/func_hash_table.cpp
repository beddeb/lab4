#include <iostream>
#include <cassert>
#include "func_hash_table.hpp"


void testInsertAndGetHT() {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(2, "two");

    assert(table.get(1) == "one");
    assert(table.get(2) == "two");

    try {
        table.get(3);
        assert(("InsertAndGet_value", false));
    } catch (const std::runtime_error &) {
        assert(("InsertAndGet_OK", true));
    }
}

void testRemoveHT() {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(2, "two");

    table.remove(1);
    try {
        table.get(1);
        assert(("Remove_value_0", false));
    } catch (const std::runtime_error &) {
        assert(("Remove_OK", true));
    }
    assert(("Remove_value_1", table.get(2)) == "two");
}

void testContainsHT() {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(2, "two");

    assert(("Contains_value_0", table.contains(1)));
    assert(("Contains_value_1", table.contains(2)));
    assert(("Contains_value_2", !table.contains(3)));
}

void testUpdateValueHT() {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(1, "one_updated");

    assert(("UpdateValue_OK", table.get(1) == "one_updated"));
}

void testSizeHT() {
    HashTable<int, std::string> table;
    assert(("Size_0", table.size() == 0));

    table.insert(1, "one");
    assert(("Size_1", table.size() == 1));

    table.insert(2, "two");
    assert(("Size_2", table.size() == 2));

    table.remove(1);
    assert(("Size_3", table.size() == 1));
}

void testRebuildHT() {
    HashTable<int, std::string> table(2);
    table.insert(1, "one");
    table.insert(2, "two");
    table.insert(3, "three");

    assert(("Rebuild_size_0", table.size() == 3));
    assert(("Rebuild_value_0", table.get(1) == "one"));
    assert(("Rebuild_value_1", table.get(2) == "two"));
    assert(("Rebuild_value_2", table.get(3) == "three"));

    table.remove(1);
    table.remove(2);
    table.remove(3);

    assert(("Rebuild_size_1", table.size() == 0));
}

void testAutoResizeHT() {
    HashTable<int, std::string> table(2);
    table.insert(1, "one");
    table.insert(2, "two");
    table.insert(3, "three");

    assert(("AutoResize_size_0", table.size() == 3));
    assert(("AutoResize_value_0", table.get(1) == "one"));
    assert(("AutoResize_value_1", table.get(2) == "two"));
    assert(("AutoResize_value_2", table.get(3) == "three"));

    table.insert(4, "four");
    assert(("AutoResize_size_1", table.size() == 4));
}

void testIteratorsHT() {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(2, "two");
    table.insert(3, "three");

    int count = 0;
    for (auto it = table.begin(); it != table.end(); ++it) {
        ++count;
    }
    assert(("Iterators_counter_0", count == 3));

    count = 0;
    for (auto it = table.cbegin(); it != table.cend(); ++it) {
        ++count;
    }
    assert(("Iterators_counter_1", count == 3));
}

void testIteratorsAfterRemoveHT() {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(2, "two");
    table.insert(3, "three");

    table.remove(2);

    int count = 0;
    for (auto it = table.begin(); it != table.end(); ++it) {
        ++count;
    }
    assert(("IteratorsAfterRemove_counter_0", count == 2));

    assert(("IteratorsAfterRemove_value_0", table.get(1) == "one"));
    assert(("IteratorsAfterRemove_value_1", table.get(3) == "three"));
}

void testHashTable() {
    testInsertAndGetHT();
    testRemoveHT();
    testContainsHT();
    testUpdateValueHT();
    testSizeHT();
    testRebuildHT();
    testAutoResizeHT();
    testIteratorsHT();
    testIteratorsAfterRemoveHT();

    std::cout << "All tests passed! (HashTable)" << std::endl;
}