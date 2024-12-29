#ifndef LAB3_HASHTABLE_HPP
#define LAB3_HASHTABLE_HPP

#include <functional> // Для std::hash
#include <stdexcept>
#include "data_structures/DynamicArray.h"
#include "data_structures/LinkedList.h"
#include "data_structures/ArraySequence.h"
#include "IDictionary.hpp"
#include "IIterator.hpp"
#include "Pair.hpp"
#include "HeapSorter.hpp"

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

        auto it = bucket.get_iterator();
        KeyValuePair<TKey, TValue> kvp;
        while (it.try_get_current_item(kvp)) {
            if (kvp.key == key) {
                // Ключ уже есть — обновим значение
                // Но для обновления нам нужно знать индекс или уметь set_current_item
                // (чтобы не искать заново).
                // Для упрощения здесь сделаем removeAt + append или через find_index_in_bucket
                update_value_in_bucket(bucket, key, value);
                return;
            }
            it.next();
        }


        KeyValuePair<TKey, TValue> new_pair{key, value};
        bucket.append(new_pair);
        count_++;

        if (!ordered_keys_.contains(key)) {
            ordered_keys_.append(key);
            HeapSorter<TKey> sorter;
            sorter.Sort(ordered_keys_);
        }

        if (count_ == capacity_) {
            int new_cap = capacity_ * resize_factor;
            if (new_cap < 16) new_cap = 16;
            resize(new_cap);
        }
    }

    TValue get(const TKey& key) const override {
        int idx = get_index(key);
        const LinkedList<KeyValuePair<TKey, TValue>>& bucket = buckets_[idx];

        auto it = bucket.get_iterator();
        KeyValuePair<TKey, TValue> kvp;
        while (it.try_get_current_item(kvp)) {
            if (kvp.key == key) {
                return kvp.value;
            }
            it.next();
        }
        throw std::out_of_range("Key not found");
    }

    void remove(const TKey& key) override {
        int idx = get_index(key);
        LinkedList<KeyValuePair<TKey, TValue>>& bucket = buckets_[idx];

        int index = find_index_in_bucket(bucket, key);
        if (index == -1) {
            throw std::out_of_range("Key not found to remove");
        }
        bucket.remove_at(index);
        count_--;

        ordered_keys_.remove_item(key);

        // Проверяем уменьшение
        if (count_ <= (capacity_ / resize_threshold_factor) && capacity_ > 1) {
            int new_cap = capacity_ / resize_factor;
            if (new_cap < 16) new_cap = 16;
            resize(new_cap);
        }
    }

    bool contains_key(const TKey& key) const override {
        int idx = get_index(key);
        const LinkedList<KeyValuePair<TKey, TValue>>& bucket = buckets_[idx];

        auto it = bucket.get_iterator();
        KeyValuePair<TKey, TValue> kvp;
        while (it.try_get_current_item(kvp)) {
            if (kvp.key == key) {
                return true;
            }
            it.next();
        }
        return false;
    }

    int get_count() const override {
        return count_;
    }

    int get_capacity() const override {
        return capacity_;
    }

    //  итератор
    class HashTableIterator : public IIterator<KeyValuePair<TKey, TValue>> {
    public:
        explicit HashTableIterator(const ArraySequence<TKey>& keys, const HashTable& table): keys_(keys), table_(table), index_(0) {}

        KeyValuePair<TKey, TValue> get_current_item() const override {
            if (index_ >= keys_.get_length()) {
                throw std::out_of_range("Iterator out of range");
            }
            TKey key = keys_.get(index_);
            TValue value = table_.get(key);
            return KeyValuePair<TKey, TValue>{key, value};
        }

        bool has_next() const override {
            return (index_ + 1) < keys_.get_length();
        }

        bool next() override {
            if(index_ >= keys_.get_length()) {
                return false;
            }

            index_++;
            return (index_ < keys_.get_length());
        }

        bool try_get_current_item(KeyValuePair<TKey, TValue>& element) const override {
            if(index_ >= keys_.get_length()) {
                return false;
            }
            TKey key = keys_.get(index_);
            element = KeyValuePair<TKey, TValue>{key, table_.get(key)};
            return true;
        }

    private:
        const ArraySequence<TKey>& keys_;
        const HashTable& table_;
        size_t index_;
    };

    HashTableIterator get_iterator() const {
        return HashTableIterator(ordered_keys_, *this);
    }

private:
    DynamicArray<LinkedList<KeyValuePair<TKey, TValue>>> buckets_;
    ArraySequence<TKey> ordered_keys_;
    int capacity_;
    int count_;
    std::hash<TKey> hasher_;

    const double resize_threshold_factor = 2.0;
    const double resize_factor = 2.0;

    int get_index(const TKey& key) const {
        size_t h = hasher_(key);
        return static_cast<int>(h % capacity_);
    }

    // поиск индекса элемента в bucket
    int find_index_in_bucket(LinkedList<KeyValuePair<TKey, TValue>>& bucket, const TKey& key) const {
        auto it = bucket.get_iterator();
        KeyValuePair<TKey,TValue> kvp;
        int idx = 0;
        while (it.try_get_current_item(kvp)) {
            if (kvp.key == key) {
                return idx;
            }
            it.next();
            idx++;
        }
        return -1;
    }

    // если есть ключ, обновляем значение, иначе добавляем
    void update_value_in_bucket(LinkedList<KeyValuePair<TKey, TValue>>& bucket, const TKey& key, const TValue& new_val) {
        int idx = find_index_in_bucket(bucket, key);
        if (idx == -1) {
            bucket.append(KeyValuePair<TKey,TValue>{key, new_val});
            count_++;
        }
        else {
            KeyValuePair<TKey,TValue> old_kvp = bucket.get(idx);
            old_kvp.value = new_val;
            bucket.set(idx, old_kvp);
        }
    }

    void resize(int new_capacity) {
        DynamicArray<LinkedList<KeyValuePair<TKey, TValue>>> new_buckets(new_capacity);

        for (int i = 0; i < capacity_; i++) {
            LinkedList<KeyValuePair<TKey, TValue>>& old_bucket = buckets_[i];
            if (old_bucket.get_length() == 0) continue;

            auto it = old_bucket.get_iterator();
            KeyValuePair<TKey,TValue> kvp;
            while (it.try_get_current_item(kvp)) {
                size_t h = hasher_(kvp.key);
                int new_idx = static_cast<int>(h % new_capacity);
                new_buckets[new_idx].append(kvp);
                it.next();
            }
        }
        buckets_ = new_buckets;
        capacity_ = new_capacity;
    }
};

#endif //LAB3_HASHTABLE_HPP
