#pragma once

#include <stdexcept>
#include "comparators.hpp"
#include "sequence.hpp"


template<typename T>
class ISorter {
public:
    virtual void sort(Sequence<T> &seq, bool (compareFunc)(const T &, const T &)) const {}
    virtual ~ISorter() = default;
};


template<typename T>
class QuickSort : public ISorter<T> {
private:
    void quickSortInside(Sequence<T> &seq, int low, int high, bool (compareFunc)(const T &, const T &)) const {
        if (low < high) {
            int pivot = high;

            int i = (low - 1);
            for (int j = low; j <= high - 1; ++j) {
                if (compareFunc(seq.get(j), seq.get(pivot))) {
                    ++i;
                    std::swap(seq.get(i), seq.get(j));
                }
            }
            std::swap(seq.get(i + 1), seq.get(high));

            quickSortInside(seq, low, i, compareFunc);
            quickSortInside(seq, i + 2, high, compareFunc);
        }
    }

public:
    void sort(Sequence<T> &seq, bool (compareFunc)(const T &, const T &)) const override {
        if (!seq.getSize()) return;
        bool flag = true;
        for (int i = 0; i < seq.getSize() - 1; ++i) {
            if (compareFunc(seq.get(i + 1), seq.get(i))) flag = false;
        }
        if (flag) return;

        quickSortInside(seq, 0, seq.getSize() - 1, compareFunc);
    }
};



