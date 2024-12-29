#pragma once

template<typename T>
class Iterator {
private:
    T* ptr;

public:
    explicit Iterator(T* ptr) : ptr(ptr) {}

    T& operator*() const {
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }

    Iterator& operator++() {
        ++ptr;
        return *this;
    }

    Iterator operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const Iterator& other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const Iterator& other) const {
        return !(*this == other);
    }
};

template<typename T>
class ConstIterator {
private:
    const T* ptr;

public:
    explicit ConstIterator(const T* ptr) : ptr(ptr) {}

    const T& operator*() const {
        return *ptr;
    }

    const T* operator->() const {
        return ptr;
    }

    ConstIterator& operator++() {
        ++ptr;
        return *this;
    }

    ConstIterator operator++(int) {
        ConstIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const ConstIterator& other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const ConstIterator& other) const {
        return !(*this == other);
    }
};