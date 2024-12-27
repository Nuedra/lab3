#ifndef LAB3_HASHTABLE_HPP
#define LAB3_HASHTABLE_HPP

#include <functional> // Для std::hash
#include <stdexcept>
#include <vector>
#include "data_structures/DynamicArray.h"
#include "data_structures/LinkedList.h"
#include "data_structures/ArraySequence.h"
#include "IDictionary.hpp"
#include "IIterator.hpp"
#include "Pair.hpp"

template<typename TKey, typename TValue>
class HashTable : public IDictionary<TKey, TValue> {
public:
    explicit HashTable(int capacity = 16) : capacity_(capacity), count_(0) {
        buckets_ = DynamicArray<LinkedList<KeyValuePair<TKey, TValue>>>(capacity_);
    }

    ~HashTable() override = default;

    void add(const TKey& key, const TValue& value) override {
        int idx = get_index(key);
        LinkedList<KeyValuePair<TKey, TValue>>& bucket = buckets_[idx];
        // Проверяем, есть ли уже такой ключ
        for (int i = 0; i < bucket.GetLength(); i++) {
            auto pair = bucket.Get(i); // Квадратичная сложность от длины цепочки переполнения
            if (pair.key == key) {
                pair.value = value;
                bucket.Set(i, pair);
                return;
            }
        }

        KeyValuePair<TKey, TValue> new_pair{key, value};
        bucket.Append(new_pair);
        count_++;

        if (std::find(ordered_keys_.begin(), ordered_keys_.end(), key) == ordered_keys_.end()) {
            ordered_keys_.push_back(key);
            std::sort(ordered_keys_.begin(), ordered_keys_.end());
        }

        if (count_ == capacity_) {
            int new_cap = capacity_ * q_;
            if (new_cap < 1) new_cap = 1;
            resize(new_cap);
        }
    }

    TValue get(const TKey& key) const override {
        int idx = get_index(key);
        const LinkedList<KeyValuePair<TKey, TValue>>& bucket = buckets_[idx];
        for (int i = 0; i < bucket.GetLength(); i++) {
            auto pair = bucket.Get(i);
            if (pair.key == key) {
                return pair.value;
            }
        }
        throw std::out_of_range("Key not found");
    }

    void remove(const TKey& key) override {
        int idx = get_index(key);
        LinkedList<KeyValuePair<TKey, TValue>>& bucket = buckets_[idx];
        for (int i = 0; i < bucket.GetLength(); i++) {
            auto pair = bucket.Get(i); // Квадратичная сложность от длинны цепочки переполнения
            if (pair.key == key) {
                bucket.RemoveAt(i);
                count_--;

                auto it = std::remove(ordered_keys_.begin(), ordered_keys_.end(), key);
                if (it != ordered_keys_.end()) {
                    ordered_keys_.erase(it, ordered_keys_.end());
                }

                // Проверяем условие сжатия: n ≤ c/p
                if (count_ <= (capacity_ / p_) && capacity_ > 1) {
                    int new_cap = capacity_ / q_;
                    if (new_cap < 1) new_cap = 1; //  Разобраться и поменять 1
                    resize(new_cap);
                }
                return;
            }
        }
        throw std::out_of_range("Key not found to remove");
    }

    bool contains_key(const TKey& key) const override {
        int idx = get_index(key);
        const LinkedList<KeyValuePair<TKey, TValue>>& bucket = buckets_[idx];
        for (int i = 0; i < bucket.GetLength(); i++) {
            auto pair = bucket.Get(i);
            if (pair.key == key) {
                return true;
            }
        }
        return false;
    }

    int get_count() const override {
        return count_;
    }

    int get_capacity() const override {
        return capacity_;
    }

    // Реализация итератора
    class HashTableIterator : public IIterator<KeyValuePair<TKey, TValue>> {
    public:
        explicit HashTableIterator(const std::vector<TKey>& keys, const HashTable& table): keys_(keys), table_(table), index_(0) {}

        KeyValuePair<TKey, TValue> get_current_item() const override {
            if (index_ >= keys_.size()) {
                throw std::out_of_range("Iterator out of range");
            }
            TKey key = keys_[index_];
            TValue value = table_.get(key);
            return KeyValuePair<TKey, TValue>{key, value};
        }

        bool has_next() const override {
            return (index_ + 1) < keys_.size();
        }

        bool next() override {
            if(index_ >= keys_.size()) {
                return false;
            }

            index_++;
            return (index_ < keys_.size());
        }

        bool try_get_current_item(KeyValuePair<TKey, TValue>& element) const override {
            if(index_ >= keys_.size()) {
                return false;
            }
            TKey key = keys_[index_];
            element = KeyValuePair<TKey, TValue>{key, table_.get(key)};
            return true;
        }

    private:
        const std::vector<TKey>& keys_;
        const HashTable& table_;
        size_t index_;
    };

    IIterator<KeyValuePair<TKey, TValue>>* get_iterator() const {
        return new HashTableIterator(ordered_keys_, *this);
    }

private:
    DynamicArray<LinkedList<KeyValuePair<TKey, TValue>>> buckets_;
    std::vector<TKey> ordered_keys_;
    int capacity_;
    int count_;
    std::hash<TKey> hasher_;

    const double p_ = 2.0; // Сделать понятнее
    const double q_ = 2.0;

    int get_index(const TKey& key) const {
        size_t h = hasher_(key);
        return h % static_cast<size_t>(capacity_);
    }

    void resize(int new_capacity) {
        DynamicArray<LinkedList<KeyValuePair<TKey, TValue>>> new_buckets(new_capacity);
        // Переносим элементы
        for (int i = 0; i < capacity_; i++) {
            LinkedList<KeyValuePair<TKey, TValue>>& old_bucket = buckets_[i];
            for (int j = 0; j < old_bucket.GetLength(); j++) {
                auto pair = old_bucket.Get(j);
                size_t h = hasher_(pair.key);
                int new_idx = h % static_cast<size_t>(new_capacity);
                new_buckets[new_idx].Append(pair);
            }
        }
        buckets_ = new_buckets;
        capacity_ = new_capacity;
    }
};

#endif //LAB3_HASHTABLE_HPP