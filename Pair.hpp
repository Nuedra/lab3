#ifndef LAB3_PAIR_HPP
#define LAB3_PAIR_HPP

#include <iostream>
#include <functional>

// Базовая структура Pair
template<typename T1, typename T2>
struct Pair {
    T1 value1;
    T2 value2;

    Pair() = default;
    Pair(const T1& v1, const T2& v2) : value1(v1), value2(v2) {}

    bool operator==(const Pair& other) const {
        return value1 == other.value1 && value2 == other.value2;
    }
};

template<typename TKey, typename TValue>
struct KeyValuePair : public Pair<TKey, TValue> {
    TKey key;
    TValue value;

    KeyValuePair(const TKey& k, const TValue& v) : Pair<TKey, TValue>(k, v), key(this->value1), value(this->value2) {}
};

template<typename T>
struct TimeRange : public Pair<T, T> {
    T start_year;
    T last_year;

    TimeRange(const T& start, const T& last) : Pair<T, T>(start, last), start_year(this->value1), last_year(this->value2) {}
};

static inline void hash_combine(std::size_t& seed, std::size_t h) {
    seed ^= h + 0x9e3779b97f4a7c16ULL + (seed << 6) + (seed >> 2);
}

namespace std {
    template<typename T1, typename T2>
    struct hash<Pair<T1, T2>> {
    std::size_t operator()(const Pair<T1, T2>& p) const {
        std::size_t h1 = std::hash<T1>()(p.value1);
        std::size_t h2 = std::hash<T2>()(p.value2);

        std::size_t seed = 0;
        hash_combine(seed, h1);
        hash_combine(seed, h2);

        return seed;
    }
};
}

#endif //LAB3_PAIR_HPP