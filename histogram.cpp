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
HashTable<Pair<int, int>, int> build_histogram(const ArraySequence<person>& persons, int start_year, int end_year, int intervals_count) {
    HashTable<Pair<int, int>, int> histogram(50);

    // Проверяем, корректен ли диапазон
    int range = end_year - start_year;
    if (range <= 0 || intervals_count <= 0) {
        // Если диапазон некорректен, возвращаем пустую гистограмму
        return histogram;
    }

    int dynamic_step = (range + intervals_count - 1) / intervals_count;

    for(int y = start_year; y < end_year; y += dynamic_step) {
        int interval_end = std::min(y + dynamic_step, end_year);
        Pair<int, int> interval{y, interval_end};
        histogram.add(interval, 0);
    }

    for(int i = 0; i < persons.get_length(); ++i) {
        const person& p = persons.get(i);

        auto interval = find_interval_for_year(p.birth_year, start_year, end_year,dynamic_step);

        if(!histogram.contains_key(interval)) {
            histogram.add(interval, 0);
        }

        int count = histogram.get(interval);
        histogram.add(interval, count + 1);
    }

    return histogram;
}
