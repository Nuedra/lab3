#ifndef QUICKSORTER_HPP
#define QUICKSORTER_HPP

#include "Isorter.hpp"
#include "data_structures/ArraySequence.h"
#include "compare.hpp"

template<typename T>
class QuickSorter : public ISorter<T> {
public:
    void Sort(ArraySequence<T>& seq, int (*cmp)(const T&, const T&) = compare_default<T>) override {
        QuickSort(seq, 0, seq.get_length() - 1, cmp);
    }

private:
    void QuickSort(ArraySequence<T>& seq, int left, int right, int (*cmp)(const T&, const T&)) {
        if (left >= right) return;

        int pivot_index = Partition(seq, left, right, cmp);
        QuickSort(seq, left, pivot_index - 1, cmp);
        QuickSort(seq, pivot_index + 1, right, cmp);
    }

    int Partition(ArraySequence<T>& seq, int left, int right, int (*cmp)(const T&, const T&)) {
        const T& pivot = seq.get(right);
        int i = left - 1;

        for (int j = left; j < right; ++j) {
            if (cmp(seq.get(j), pivot) < 0) {
                ++i;
                seq.swap(i, j);
            }
        }

        seq.swap(i + 1, right);

        return i + 1;
    }
};

#endif //QUICKSORTER_HPP
