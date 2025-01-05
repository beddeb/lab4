#pragma once

#include <stdexcept>
#include <utility>
#include "sorts_algs.hpp"
#include "sequence.hpp"


template<typename T>
class ArraySequence : public Sequence<T> {
private:
    T *data;

    template<typename U = T>
    void sortImpl() {
        QuickSort<T> sorter;

        if constexpr (std::is_same_v<U, Edge<typename U::vertex_type>>) {
            sorter.sort(*this,
                        [](const U& a, const U& b) {
                            return a.weight < b.weight;
                        }
            );
        }

        else if constexpr (std::is_arithmetic_v<U> || std::is_same_v<U, std::string>) {
            sorter.sort(*this,
                        [](const U& a, const U& b) {
                            return a < b;
                        }
            );
        }
        else {
            throw std::runtime_error("Cannot sort this type");
        }
    }

public:
    explicit ArraySequence(size_t size = 0) : Sequence<T>(size), data(new T[size]) {}

    ArraySequence(const ArraySequence &other) : Sequence<T>(other.size), data(new T[other.capacity]) {
        for (size_t i = 0; i < other.size; ++i) {
            data[i] = other.data[i];
        }
        Sequence<T>::size = other.size;
        Sequence<T>::capacity = other.capacity;
    }

    ArraySequence(ArraySequence &&other) noexcept
            : Sequence<T>(other.size), data(other.data) {
        Sequence<T>::size = other.size;
        Sequence<T>::size = other.capacity;
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    ~ArraySequence() {
        delete[] data;
    }

    void sort() {
        sortImpl();
    }

    // Переопределение сортировки
    template<typename CompareFunc>
    void sort(CompareFunc customCompare) {
        QuickSort<T> sorter;
        sorter.sort(*this, customCompare);
    }

    void add(const T &element) override {
        if (Sequence<T>::size == Sequence<T>::capacity) {
            Sequence<T>::resize();
            T *newData = new T[Sequence<T>::capacity];
            for (size_t i = 0; i < Sequence<T>::size; ++i) {
                newData[i] = std::move(data[i]);
            }
            delete[] data;
            data = newData;
        }
        data[Sequence<T>::size++] = element;
    }

    void remove(size_t index) override {
        if (index >= Sequence<T>::size) {
            throw std::out_of_range("Index out of range");
        }
        for (size_t i = index; i < Sequence<T>::size - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --Sequence<T>::size;
    }

    bool contains(const T& element) const {
        for (size_t i = 0; i < Sequence<T>::size; ++i) {
            if (data[i] == element) {
                return true;
            }
        }
        return false;
    }

    void removeElement(const T& element) {
        for (size_t i = 0; i < Sequence<T>::size; ++i) {
            if (data[i] == element) {
                for (size_t j = i; j < Sequence<T>::size - 1; ++j) {
                    data[j] = data[j + 1];
                }
                --Sequence<T>::size;
                return;
            }
        }
        throw std::runtime_error("Element not found");
    }

    T &get(size_t index) override {
        if (index >= Sequence<T>::size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T &get(size_t index) const override {
        if (index >= Sequence<T>::size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    ArraySequence &operator=(const ArraySequence &other) {
        if (this != &other) {
            delete[] data;

            Sequence<T>::size = other.size;
            Sequence<T>::capacity = other.capacity;
            data = new T[other.capacity];
            for (size_t i = 0; i < other.size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ArraySequence &operator=(ArraySequence &&other) noexcept {
        if (this != &other) {
            delete[] data;

            data = other.data;
            Sequence<T>::size = other.size;
            Sequence<T>::capacity = other.capacity;

            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    bool operator==(const ArraySequence &other) const {
        if (this->size != other.size) {
            return false;
        }
        for (size_t i = 0; i < this->size; ++i) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }

    // Оператор неравенства
    bool operator!=(const ArraySequence &other) const {
        return !(*this == other);
    }

    Iterator<T> begin() {
        return Iterator<T>(data);
    }

    Iterator<T> end() {
        return Iterator<T>(data + Sequence<T>::size);
    }

    ConstIterator<T> begin() const {
        return ConstIterator<T>(data);
    }

    ConstIterator<T> end() const {
        return ConstIterator<T>(data + Sequence<T>::size);
    }

    ConstIterator<T> cbegin() const {
        return ConstIterator<T>(data);
    }

    ConstIterator<T> cend() const {
        return ConstIterator<T>(data + Sequence<T>::size);
    }
};