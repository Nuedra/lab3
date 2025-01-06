#ifndef ISORTER_H
#define ISORTER_H

#include "../data_structures/ArraySequence.h"
#include "compare.hpp"

template<typename T>
class ISorter {
public:
    virtual ~ISorter() = default;

    virtual void Sort(ArraySequence<T>& seq,
                      int (*cmp)(const T&, const T&) = compare_default<T>) = 0;
};

#endif // ISORTER_H
