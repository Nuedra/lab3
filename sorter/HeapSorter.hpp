#ifndef HEAPSORTER_HPP
#define HEAPSORTER_HPP

#include "Isorter.hpp"
#include "../data_structures/ArraySequence.h"
#include "compare.hpp"

template<typename T>
class HeapSorter : public ISorter<T> {
public:
    void Sort(ArraySequence<T>& seq, int (*cmp)(const T&, const T&) = compare_default<T>) override {
        HeapSort(seq, cmp);
    }

private:
    void HeapSort(ArraySequence<T>& seq, int (*cmp)(const T&, const T&)) {
        int n = seq.get_length();

        for (int i = n / 2 - 1; i >= 0; --i) {
            Heapify(seq, n, i, cmp);
        }

        for (int i = n - 1; i > 0; --i) {
            seq.swap(0, i);
            Heapify(seq, i, 0, cmp);
        }
    }

    void Heapify(ArraySequence<T>& seq, int n, int i, int (*cmp)(const T&, const T&)) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && cmp(seq.get(left), seq.get(largest)) > 0) {
            largest = left;
        }

        if (right < n && cmp(seq.get(right), seq.get(largest)) > 0) {
            largest = right;
        }

        if (largest != i) {
            seq.swap(i, largest);
            Heapify(seq, n, largest, cmp);
        }
    }
};

#endif //HEAPSORTER_HPP
