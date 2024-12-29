#pragma once

#include <stdexcept>
#include <utility>
#include "iterators.hpp"


template<typename T>
class Sequence {
protected:
    T *data;
    size_t capacity;
    size_t size;

    void resize() {
        capacity = capacity ? capacity * 2 : 1;
        T *newData = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = std::move(data[i]);
        }
        delete[] data;
        data = newData;
    }

public:
    explicit Sequence(size_t initialCapacity = 4) : data(new T[initialCapacity]), capacity(initialCapacity), size(0) {}

    Sequence(const Sequence &other) : data(new T[other.capacity]), capacity(other.capacity), size(other.size) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    Sequence(Sequence &&other) noexcept: data(other.data), capacity(other.capacity), size(other.size) {
        other.data = nullptr;
        other.size = other.capacity = 0;
    }

    ~Sequence() {
        delete[] data;
    }

    virtual void add(const T &element) {
        if (size == capacity) {
            resize();
        }
        data[size++] = element;
    }

    virtual void remove(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --size;
    }

    virtual T &get(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    virtual const T &get(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    [[nodiscard]] size_t getSize() const {
        return size;
    }

    Sequence &operator=(const Sequence &other) {
        if (this != &other) {
            Sequence tmp(other);
            *this = std::move(tmp);
        }
        return *this;
    }

    Sequence &operator=(Sequence &&other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            capacity = other.capacity;
            other.data = nullptr;
            other.size = other.capacity = 0;
        }
        return *this;
    }

    T &operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T &operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    Sequence &operator+=(const T &element) {
        add(element);
        return *this;
    }

    bool operator==(const Sequence &other) const {
        if (size != other.size) return false;
        for (size_t i = 0; i < size; ++i) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    bool operator!=(const Sequence &other) const {
        return !(*this == other);
    }

    Iterator<T> begin() {
        return Iterator<T>(data);
    }

    Iterator<T> end() {
        return Iterator<T>(data + size);
    }

    ConstIterator<T> begin() const {
        return ConstIterator<T>(data);
    }

    ConstIterator<T> end() const {
        return ConstIterator<T>(data + size);
    }

    ConstIterator<T> cbegin() const {
        return ConstIterator<T>(data);
    }

    ConstIterator<T> cend() const {
        return ConstIterator<T>(data + size);
    }
};