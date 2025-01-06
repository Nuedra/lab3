#ifndef LAB3_HISTOGRAM_HPP
#define LAB3_HISTOGRAM_HPP

#include "Pair.hpp"
#include "csv/person.hpp"
#include "data_structures/ArraySequence.h"
#include "HashTable.hpp"

int custom_int_min(int a, int b);
Pair<int,int> find_interval_for_year(int year, int start_year, int end_year, int step);
HashTable<Pair<int,int>, int> build_histogram(const ArraySequence<person>& persons, int start_year, int end_year, int step);

#endif //LAB3_HISTOGRAM_HPP
