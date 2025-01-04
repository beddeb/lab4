#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include "chrono_sequences.hpp"
#include "demangle.hpp"

template<typename Container, typename T, typename Comparator>
void chronoAddInt(size_t num_elements) {
    Container cont;
    Comparator comp;

    auto start_cont = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_elements; ++i) {
        cont.add(i);
    }
    auto end_cont = std::chrono::high_resolution_clock::now();
    auto duration_cont = std::chrono::duration_cast<std::chrono::microseconds>(end_cont - start_cont);

    auto start_comp = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_elements; ++i) {
        comp.push_back(i);
    }
    auto end_comp = std::chrono::high_resolution_clock::now();
    auto duration_comp = std::chrono::duration_cast<std::chrono::microseconds>(end_comp - start_comp);

    std::cout << "Add " << num_elements << " elements:" << std::endl;
    std::cout << "  " << demangle(typeid(Container).name()) << ": " << duration_cont.count() << " ms" << std::endl;
    std::cout << "  " << demangle(typeid(Comparator).name()) << ": " << duration_comp.count() << " ms" << std::endl;
    std::cout << std::endl;
}

template<typename Container, typename T, typename Comparator>
void chronoGetIndexInt(size_t num_elements, size_t step) {
    Container cont;
    Comparator comp;
    for (size_t i = 0; i < num_elements; ++i) {
        cont.add(i);
        comp.push_back(i);
    }

    auto start_cont = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_elements; i += num_elements / step) {
        volatile T temp = cont.get(i);
    }
    auto end_cont = std::chrono::high_resolution_clock::now();
    auto duration_cont = std::chrono::duration_cast<std::chrono::nanoseconds>(end_cont - start_cont);

    auto start_comp = std::chrono::high_resolution_clock::now();
    if constexpr (std::is_same_v<Comparator, std::list<T>>) {
        for (size_t i = 0; i < num_elements; i += num_elements / step) {
            auto it = comp.begin();
            std::advance(it, i);
            volatile T temp = *it;
        }
    } else {
        for (size_t i = 0; i < num_elements; i += num_elements / step) {
            volatile T temp = comp[i];
        }
    }
    auto end_comp = std::chrono::high_resolution_clock::now();
    auto duration_comp = std::chrono::duration_cast<std::chrono::nanoseconds>(end_comp - start_comp);

    std::cout << "Get element by index (average over " << step << " accesses):" << std::endl;
    std::cout << "  " << demangle(typeid(Container).name()) << ": " << duration_cont.count() / step << " ns"
              << std::endl;
    std::cout << "  " << demangle(typeid(Comparator).name()) << ": " << duration_comp.count() / step << " ns"
              << std::endl;
    std::cout << std::endl;
}

template<typename Container, typename T, typename Comparator>
void chronoIterateInt(size_t num_elements) {
    Container cont;
    Comparator comp;
    for (size_t i = 0; i < num_elements; ++i) {
        cont.add(i);
        comp.push_back(i);
    }

    auto start_cont = std::chrono::high_resolution_clock::now();
    for (T val: cont) {
        volatile T temp = val;
    }
    auto end_cont = std::chrono::high_resolution_clock::now();
    auto duration_cont = std::chrono::duration_cast<std::chrono::microseconds>(end_cont - start_cont);

    auto start_comp = std::chrono::high_resolution_clock::now();
    for (T val: comp) {
        volatile T temp = val;
    }
    auto end_comp = std::chrono::high_resolution_clock::now();
    auto duration_comp = std::chrono::duration_cast<std::chrono::microseconds>(end_comp - start_comp);

    std::cout << "Iterate over " << num_elements << " elements:" << std::endl;
    std::cout << "  " << demangle(typeid(Container).name()) << ": " << duration_cont.count() << " ms" << std::endl;
    std::cout << "  " << demangle(typeid(Comparator).name()) << ": " << duration_comp.count() << " ms" << std::endl;
    std::cout << std::endl;
}

template<typename Container, typename T, typename Comparator>
void chronoRemoveInt(size_t num_elements) {
    Container cont;
    Comparator comp;

    // Удаление с конца
    for (size_t i = 0; i < num_elements; ++i) {
        cont.add(i);
        comp.push_back(i);
    }

    auto start_cont_end = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_elements / 2; ++i) {
        cont.remove(cont.getSize() - 1);
    }
    auto end_cont_end = std::chrono::high_resolution_clock::now();
    auto duration_cont_end = std::chrono::duration_cast<std::chrono::microseconds>(end_cont_end - start_cont_end);

    auto start_comp_end = std::chrono::high_resolution_clock::now();
    if constexpr (std::is_same_v<Comparator, std::list<T>>) {
        for (size_t i = 0; i < num_elements / 2; ++i) {
            comp.pop_back();
        }
    } else {
        for (size_t i = 0; i < num_elements / 2; ++i) {
            comp.pop_back();
        }
    }
    auto end_comp_end = std::chrono::high_resolution_clock::now();
    auto duration_comp_end = std::chrono::duration_cast<std::chrono::microseconds>(end_comp_end - start_comp_end);

    // Удаление из начала
    Container cont2;
    Comparator comp2;
    for (size_t i = 0; i < num_elements; ++i) {
        cont2.add(i);
        comp2.push_back(i);
    }
    auto start_cont_begin = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_elements / 2; ++i) {
        cont2.remove(0);
    }
    auto end_cont_begin = std::chrono::high_resolution_clock::now();
    auto duration_cont_begin = std::chrono::duration_cast<std::chrono::microseconds>(end_cont_begin - start_cont_begin);

    auto start_comp_begin = std::chrono::high_resolution_clock::now();
    if constexpr (std::is_same_v<Comparator, std::list<T>>) {
        for (size_t i = 0; i < num_elements / 2; ++i) {
            comp2.erase(comp2.begin());
        }
    } else {
        for (size_t i = 0; i < num_elements / 2; ++i) {
            comp2.erase(comp2.begin());
        }
    }
    auto end_comp_begin = std::chrono::high_resolution_clock::now();
    auto duration_comp_begin = std::chrono::duration_cast<std::chrono::microseconds>(end_comp_begin - start_comp_begin);

    std::cout << "Remove " << num_elements / 2 << " elements:" << std::endl;
    std::cout << "  From the end:" << std::endl;
    std::cout << "    " << demangle(typeid(Container).name()) << ": " << duration_cont_end.count() << " ms"
              << std::endl;
    std::cout << "    " << demangle(typeid(Comparator).name()) << ": " << duration_comp_end.count() << " ms"
              << std::endl;

    std::cout << "  From the beginning:" << std::endl;
    std::cout << "    " << demangle(typeid(Container).name()) << ": " << duration_cont_begin.count() << " ms"
              << std::endl;
    std::cout << "    " << demangle(typeid(Comparator).name()) << ": " << duration_comp_begin.count() << " ms"
              << std::endl;
    std::cout << std::endl;
}

void chronoSequence(size_t num_elements, size_t step) {
    std::cout << "Performance tests for Sequence vs std::vector" << std::endl << std::endl;
    chronoAddInt<Sequence<int>, int, std::vector<int>>(num_elements);
    chronoGetIndexInt<Sequence<int>, int, std::vector<int>>(num_elements, step);
    chronoIterateInt<Sequence<int>, int, std::vector<int>>(num_elements);
    chronoRemoveInt<Sequence<int>, int, std::vector<int>>(num_elements);
    std::cout << std::endl;
}

void chronoListSequence(size_t num_elements, size_t step) {
    std::cout << "Performance tests for ListSequence vs std::list" << std::endl << std::endl;
    chronoAddInt<ListSequence<int>, int, std::list<int>>(num_elements);
    chronoGetIndexInt<ListSequence<int>, int, std::list<int>>(num_elements, step);
    chronoIterateInt<ListSequence<int>, int, std::list<int>>(num_elements);
    chronoRemoveInt<ListSequence<int>, int, std::list<int>>(num_elements);
    std::cout << std::endl;
}

void chronoArraySequence(size_t num_elements, size_t step) {
    std::cout << "Performance tests for ArraySequence vs std::vector" << std::endl << std::endl;
    chronoAddInt<ArraySequence<int>, int, std::vector<int>>(num_elements);
    chronoGetIndexInt<ArraySequence<int>, int, std::vector<int>>(num_elements, step);
    chronoIterateInt<ArraySequence<int>, int, std::vector<int>>(num_elements);
    chronoRemoveInt<ArraySequence<int>, int, std::vector<int>>(num_elements);
    std::cout << std::endl;
}