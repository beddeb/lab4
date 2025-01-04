#pragma once

#include <stdexcept>
#include <utility>
#include "sequence.hpp"


template<typename T>
class ListSequence : public Sequence<T> {
private:
    struct Node {
        T data;
        Node *next;

        explicit Node(const T &data) : data(data), next(nullptr) {}
    };

    Node *head;
    Node *tail;

public:
    ListSequence() : head(nullptr), tail(nullptr) {
        Sequence<T>::size = 0;
    }

    ListSequence(const ListSequence &other) : Sequence<T>(other.getSize()) {
        head = nullptr;
        tail = nullptr;
        Node *current = other.head;
        while (current) {
            add(current->data);
            current = current->next;
        }
    }

    ListSequence(ListSequence &&other) noexcept
            : Sequence<T>(std::move(other)),
              head(other.head),
              tail(other.tail) {
        other.head = nullptr;
        other.tail = nullptr;
    }

    ~ListSequence() {
        clear();
    }

    void add(const T &element) override {
        Node *newNode = new Node(element);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        Sequence<T>::size++;
    }

    void remove(size_t index) override {
        if (index >= Sequence<T>::size) {
            throw std::out_of_range("Index out of range");
        }

        Node *current = head;
        if (index == 0) {
            head = head->next;
            delete current;
            if (!head) {
                tail = nullptr;
            }
        } else {
            Node *prev = nullptr;
            for (size_t i = 0; i < index; ++i) {
                prev = current;
                current = current->next;
            }
            prev->next = current->next;
            if (current == tail) {
                tail = prev;
            }
            delete current;
        }
        Sequence<T>::size--;
    }

    T &get(size_t index) override {
        if (index >= Sequence<T>::size) {
            throw std::out_of_range("Index out of range");
        }

        Node *current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    const T &get(size_t index) const override {
        if (index >= Sequence<T>::size) {
            throw std::out_of_range("Index out of range");
        }

        Node *current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    void clear() {
        while (head) {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        Sequence<T>::size = 0;
    }

    ListSequence &operator=(const ListSequence &other) {
        if (this != &other) {
            clear();
            Node *current = other.head;
            while (current) {
                add(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    ListSequence &operator=(ListSequence &&other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            Sequence<T>::size = other.Sequence<T>::size;
            other.head = nullptr;
            other.tail = nullptr;
        }
        return *this;
    }

    bool operator==(const ListSequence &other) const {
        if (this->getSize() != other.getSize()) {
            return false;
        }
        Node *current1 = this->head;
        Node *current2 = other.head;

        while (current1 && current2) {
            if (current1->data != current2->data) {
                return false;
            }
            current1 = current1->next;
            current2 = current2->next;
        }
        return true;
    }

    bool operator!=(const ListSequence &other) const {
        return !(*this == other);
    }

    class ListIterator {
    private:
        Node *current;

    public:
        explicit ListIterator(Node *node) : current(node) {}

        T &operator*() const {
            return current->data;
        }

        ListIterator &operator++() {
            current = current->next;
            return *this;
        }

        ListIterator operator++(int) {
            ListIterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const ListIterator &other) const {
            return current == other.current;
        }

        bool operator!=(const ListIterator &other) const {
            return !(*this == other);
        }
    };

    ListIterator begin() {
        return ListIterator(head);
    }

    ListIterator end() {
        return ListIterator(nullptr);
    }

    class ConstListIterator {
    private:
        const Node *current;

    public:
        explicit ConstListIterator(const Node *node) : current(node) {}

        const T &operator*() const {
            return current->data;
        }

        ConstListIterator &operator++() {
            current = current->next;
            return *this;
        }

        ConstListIterator operator++(int) {
            ConstListIterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const ConstListIterator &other) const {
            return current == other.current;
        }

        bool operator!=(const ConstListIterator &other) const {
            return !(*this == other);
        }
    };

    ConstListIterator begin() const {
        return ConstListIterator(head);
    }

    ConstListIterator end() const {
        return ConstListIterator(nullptr);
    }

    ConstListIterator cbegin() const {
        return ConstListIterator(head);
    }

    ConstListIterator cend() const {
        return ConstListIterator(nullptr);
    }
};