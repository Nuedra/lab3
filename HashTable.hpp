#ifndef LAB3_HASHTABLE_HPP
#define LAB3_HASHTABLE_HPP

#include <functional> // Для std::hash
#include <stdexcept>
#include "data_structures/DynamicArray.h"
#include "data_structures/LinkedList.h"
#include <string>
#include <iostream>

template<typename TKey, typename TValue>
struct Pair {
    TKey key;
    TValue value;
};

template<typename TKey, typename TValue>
class HashTable {
public:
    explicit HashTable(int capacity = 16)
            : capacity_(capacity), count_(0)
    {
        buckets_ = DynamicArray<LinkedList<Pair<TKey,TValue>>>(capacity_);
    }

    ~HashTable() {

    }

    void Add(const TKey& key, const TValue& value) {
        int idx = getIndex(key);
        LinkedList<Pair<TKey,TValue>>& bucket = buckets_[idx];
        // Проверяем, есть ли уже такой ключ
        for (int i = 0; i < bucket.GetLength(); i++) {
            auto pair = bucket.Get(i);
            if (pair.key == key) {
                pair.value = value;
                bucket.Set(i, pair);
                return;
            }
        }

        Pair<TKey,TValue> newPair{key, value};
        bucket.Append(newPair);
        count_++;

        if (count_ == capacity_) {
            int newCap = (int)(capacity_ * q_);
            if (newCap < 1) newCap = 1;
            resize(newCap);
        }
    }

    TValue Get(const TKey& key) const {
        int idx = getIndex(key);
        const LinkedList<Pair<TKey,TValue>>& bucket = buckets_[idx];
        for (int i = 0; i < bucket.GetLength(); i++) {
            auto pair = bucket.Get(i);
            if (pair.key == key) {
                return pair.value;
            }
        }
        throw std::out_of_range("Key not found");
    }

    void Remove(const TKey& key) {
        int idx = getIndex(key);
        LinkedList<Pair<TKey,TValue>>& bucket = buckets_[idx];
        for (int i = 0; i < bucket.GetLength(); i++) {
            auto pair = bucket.Get(i);
            if (pair.key == key) {
                bucket.RemoveAt(i);
                count_--;

                // Проверяем условие сжатия: n ≤ c/p
                if (count_ <= (int)(capacity_ / p_) && capacity_ > 1) {
                    int newCap = (int)(capacity_ / q_);
                    if (newCap < 1) newCap = 1;
                    resize(newCap);
                }
                return;
            }
        }
        throw std::out_of_range("Key not found to remove");
    }

    bool ContainsKey(const TKey& key) const {
        int idx = getIndex(key);
        const LinkedList<Pair<TKey,TValue>>& bucket = buckets_[idx];
        for (int i = 0; i < bucket.GetLength(); i++) {
            auto pair = bucket.Get(i);
            if (pair.key == key) {
                return true;
            }
        }
        return false;
    }

    int GetCount() const {
        return count_;
    }

    int GetCapacity() const {
        return capacity_;
    }

private:
    DynamicArray<LinkedList<Pair<TKey,TValue>>> buckets_;
    int capacity_;
    int count_;
    std::hash<TKey> hasher_;

    const double p_ = 2.0;
    const double q_ = 2.0;

    int getIndex(const TKey& key) const {
        size_t h = hasher_(key);
        return (int)(h % (size_t)capacity_);
    }

    void resize(int newCapacity) {
        DynamicArray<LinkedList<Pair<TKey,TValue>>> newBuckets(newCapacity);
        // Переносим элементы
        for (int i = 0; i < capacity_; i++) {
            LinkedList<Pair<TKey,TValue>>& oldBucket = buckets_[i];
            for (int j = 0; j < oldBucket.GetLength(); j++) {
                auto pair = oldBucket.Get(j);
                size_t h = hasher_(pair.key);
                int newIdx = (int)(h % (size_t)newCapacity);
                newBuckets[newIdx].Append(pair);
            }
        }
        buckets_ = newBuckets;
        capacity_ = newCapacity;
    }
};

#endif //LAB3_HASHTABLE_HPP
