#include <cassert>
#include <iostream>
#include "func_sequences.hpp"


template<typename Container>
void testAddElement() {
    Container seq;
    seq.add(10);
    assert(("AddElement_size", seq.getSize() == 1));
    assert(("AddElement_value", seq.get(0) == 10));
}

template<typename Container>
void testAddMultipleElements() {
    Container seq;
    seq.add(10);
    seq.add(20);
    seq.add(30);
    assert(("AddMultipleElements_size", seq.getSize() == 3));
    assert(("AddMultipleElements_value_0", seq.get(0) == 10));
    assert(("AddMultipleElements_value_1", seq.get(1) == 20));
    assert(("AddMultipleElements_value_2", seq.get(2) == 30));
}

template<typename Container>
void testRemoveElement() {
    Container seq;
    seq.add(10);
    seq.add(20);
    seq.add(30);
    seq.remove(1);
    assert(("RemoveElement_size", seq.getSize() == 2));
    assert(("RemoveElement_value_0", seq.get(0) == 10));
    assert(("RemoveElement_value_1", seq.get(1) == 30));
}

template<typename Container>
void testRemoveOutOfRange() {
    Container seq;
    seq.add(10);
    seq.add(20);
    try {
        seq.remove(2);
        assert(("RemoveOutOfRange_remove", false));
    } catch (const std::out_of_range &) {
        assert(("RemoveOutOfRange_OK", true));
    }
}

template<typename Container>
void testGetOutOfRange() {
    Container seq;
    seq.add(10);
    try {
        seq.get(1);
        assert(("GetOutOfRange_value", false));
    } catch (const std::out_of_range &) {
        assert(("GetOutOfRange_OK", true));
    }
}

template<typename Container>
void testCopyConstructor() {
    Container seq;
    seq.add(10);
    seq.add(20);
    Container copy(seq);
    assert(("CopyConstructor_size", copy.getSize() == 2));
    assert(("CopyConstructor_value_0", copy.get(0) == 10));
    assert(("CopyConstructor_value_1", copy.get(1) == 20));
}

template<typename Container>
void testMoveConstructor() {
    Container seq;
    seq.add(10);
    seq.add(20);
    Container moved(std::move(seq));
    assert(("MoveConstructor_size_0", moved.getSize() == 2));
    assert(("MoveConstructor_value_0", moved.get(0) == 10));
    assert(("MoveConstructor_value_1", moved.get(1) == 20));
    assert(("MoveConstructor_size_1", seq.getSize() == 0));
}

template<typename Container>
void testAssignmentOperator() {
    Container seq;
    seq.add(10);
    seq.add(20);
    Container copy;
    copy = seq;
    assert(("SelfAssignment_size", copy.getSize() == 2));
    assert(("SelfAssignment_value_0", copy.get(0) == 10));
    assert(("SelfAssignment_value_1", copy.get(1) == 20));
}

template<typename Container>
void testSelfAssignment() {
    Container seq;
    seq.add(10);
    seq = seq;
    assert(("SelfAssignment_size", seq.getSize() == 1));
    assert(("SelfAssignment_value", seq.get(0) == 10));
}

template<typename Container>
void testEqualityOperator() {
    Container seq1;
    seq1.add(10);
    seq1.add(20);

    Container seq2;
    seq2.add(10);
    seq2.add(20);

    assert(("EqualityOperator", seq1 == seq2));
}

template<typename Container>
void testInequalityOperator() {
    Container seq1;
    seq1.add(10);

    Container seq2;
    seq2.add(20);

    assert(("InequalityOperator", seq1 != seq2));
}

template<typename Container>
void testResize() {
    Container seq;
    for (int i = 0; i < 10; ++i) {
        seq.add(i);
    }
    assert(seq.getSize() == 10);
    for (int i = 0; i < 10; ++i) {
        assert(("Resize", seq.get(i) == i));
    }
}

void testSequence() {
    testAddElement<Sequence<int>>();
    testAddMultipleElements<Sequence<int>>();
    testRemoveElement<Sequence<int>>();
    testRemoveOutOfRange<Sequence<int>>();
    testGetOutOfRange<Sequence<int>>();
    testCopyConstructor<Sequence<int>>();
    testMoveConstructor<Sequence<int>>();
    testAssignmentOperator<Sequence<int>>();
    testSelfAssignment<Sequence<int>>();
    testEqualityOperator<Sequence<int>>();
    testInequalityOperator<Sequence<int>>();
    testResize<Sequence<int>>();

    std::cout << "All tests passed! (Sequence)" << std::endl;
}

void testListSequence() {
    testAddElement<ListSequence<int>>();
    testAddMultipleElements<ListSequence<int>>();
    testRemoveElement<ListSequence<int>>();
    testRemoveOutOfRange<ListSequence<int>>();
    testGetOutOfRange<ListSequence<int>>();
    testCopyConstructor<ListSequence<int>>();
    testMoveConstructor<ListSequence<int>>();
    testAssignmentOperator<ListSequence<int>>();
    testSelfAssignment<ListSequence<int>>();
    testEqualityOperator<ListSequence<int>>();
    testInequalityOperator<ListSequence<int>>();
    testResize<ListSequence<int>>();

    std::cout << "All tests passed! (ListSequence)" << std::endl;
}

void testArraySequence() {
    testAddElement<ArraySequence<int>>();
    testAddMultipleElements<ArraySequence<int>>();
    testRemoveElement<ArraySequence<int>>();
    testRemoveOutOfRange<ArraySequence<int>>();
    testGetOutOfRange<ArraySequence<int>>();
    testCopyConstructor<ArraySequence<int>>();
    testMoveConstructor<ArraySequence<int>>();
    testAssignmentOperator<ArraySequence<int>>();
    testSelfAssignment<ArraySequence<int>>();
    testEqualityOperator<ArraySequence<int>>();
    testInequalityOperator<ArraySequence<int>>();
    testResize<ArraySequence<int>>();

    std::cout << "All tests passed! (ArraySequence)" << std::endl;
}