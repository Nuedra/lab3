#include "histogram.hpp"
#include "Pair.hpp"
#include "HashTable.hpp"
#include "person.hpp"
#include "data_structures/ArraySequence.h"

Pair<int, int> find_interval_for_year(int year, int start_year, int end_year, int step) {
    if(year < start_year) {
        return Pair<int, int>{start_year, start_year + step};
    }

    if(year >= end_year) {
        return Pair<int, int>{end_year - step, end_year};
    }

    int offset = year - start_year;
    int interval_index = offset / step;
    int interval_start = start_year + interval_index * step;
    int interval_end = interval_start + step;

    return Pair<int, int>{interval_start, std::min(interval_end, end_year)};
}

// Плавающитй шаг для гистограмы (шаг определяется диопозоном)
HashTable<Pair<int, int>, int> build_histogram(const ArraySequence<person>& persons, int start_year, int end_year, int step) {
    HashTable<Pair<int, int>, int> histogram(50);

    for(int y = start_year; y < end_year; y += step) {
        int interval_end = std::min(y + step, end_year);
        Pair<int, int> interval{y, interval_end};
        histogram.add(interval, 0);
    }

    if((end_year - start_year) % step != 0) {
        Pair<int, int> last_interval{end_year - step, end_year};
        if(!histogram.contains_key(last_interval)) {
            histogram.add(last_interval, 0);
        }
    }

    for(int i = 0; i < persons.get_length(); ++i) {
        const person& p = persons.get(i);
        auto interval = find_interval_for_year(p.birth_year, start_year, end_year, step);

        if(!histogram.contains_key(interval)) {
            histogram.add(interval, 0);
        }

        int count = histogram.get(interval);
        histogram.add(interval, count + 1);
    }

    return histogram;
}
