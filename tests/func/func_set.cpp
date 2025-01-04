#include <iostream>
#include <cassert>
#include "func_set.hpp"


void testAddS() {
    HashSet<int> set;
    set.insert(1);
    set.insert(2);

    assert(("Add_value_0", set.contains(1)));
    assert(("Add_value_1", set.contains(2)));
}

void testAddDuplicateElementS() {
    HashSet<int> set;
    set.insert(1);
    assert(("AddDuplicateElement_size_0", set.size() == 1));
    set.insert(1);
    assert(("AddDuplicateElement_size_1", set.size() == 1));
    set.remove(1);
    assert(("AddDuplicateElement_size_2", set.size() == 0));
}

void testRemoveS() {
    HashSet<int> set;
    set.insert(1);
    set.insert(2);

    set.remove(1);
    assert(("Remove_value_0", set.contains(1) == false));
    assert(("Remove_value_1", set.contains(2)));
}

void testRemoveNonexistentElementS() {
    HashSet<int> set;
    set.insert(1);

    try {
        set.remove(2);
        assert(("RemoveNonexistentElement_value", false));
    } catch (const std::runtime_error &) {
        assert(("RemoveNonexistentElement_OK", true));
    }
}

void testContainsS() {
    HashSet<int> set;
    set.insert(1);
    set.insert(2);

    assert(("Contains_value_0", set.contains(1)));
    assert(("Contains_value_1", set.contains(2)));
    assert(("Contains_value_2", set.contains(3) == false));
}

void testSizeS() {
    HashSet<int> set;
    assert(("Size_0", set.size() == 0));

    set.insert(1);
    assert(("Size_1", set.size() == 1));

    set.insert(2);
    assert(("Size_2", set.size() == 2));

    set.remove(1);
    assert(("Size_3", set.size() == 1));
}

void testIteratorsS() {
    HashSet<int> set;
    set.insert(1);
    set.insert(2);
    set.insert(3);

    int count = 0;
    for (auto it = set.begin(); it != set.end(); ++it) {
        ++count;
    }
    assert(("Iterators_counter_0", count == 3));

    count = 0;
    for (auto it = set.cbegin(); it != set.cend(); ++it) {
        ++count;
    }
    assert(("Iterators_counter_1", count == 3));
}

void testSet() {
    testAddS();
    testAddDuplicateElementS();
    testRemoveS();
    testRemoveNonexistentElementS();
    testContainsS();
    testSizeS();
    testIteratorsS();

    std::cout << "All tests passed! (Set)" << std::endl;
}