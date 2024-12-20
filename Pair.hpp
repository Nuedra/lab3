#ifndef LAB3_PAIR_HPP
#define LAB3_PAIR_HPP

#include <functional> // Для std::hash

template<typename TKey, typename TValue>
struct Pair {
    TKey key;
    TValue value;

    bool operator==(const Pair& other) const {
        return key == other.key && value == other.value;
    }
};

static inline void hash_combine(std::size_t& seed, std::size_t h) {
    seed ^= h + 0x9e3779b97f4a7c16ULL + (seed << 6) + (seed >> 2);
}

namespace std {
    template<>
    struct hash<Pair<int, int>> {
    std::size_t operator()(const Pair<int, int>& p) const {
        // Хешируем два int: p.key, p.value
        std::size_t h1 = std::hash<int>()(p.key);
        std::size_t h2 = std::hash<int>()(p.value);

        std::size_t seed = 0;

        hash_combine(seed, h1);
        hash_combine(seed, h2);

        return seed;
    }
};
}

#endif //LAB3_PAIR_HPP
