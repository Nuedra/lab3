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

        LinkedListNode<KeyValuePair<TKey, TValue>>* current = nullptr;
        // указатель на первый узел
        if (bucket.get_length() > 0) {
            current = bucket.get_node(0);
        }

        for (int i = 0; i < bucket.get_length(); i++) {
            if (current->value.key == key) {
                current->value.value = value;
                return;
            }
            current = current->next;
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

        if (bucket.get_length() == 0) {
            throw std::out_of_range("Key not found");
        }

        LinkedListNode<KeyValuePair<TKey, TValue>>* current = bucket.get_node(0);

        for (int i = 0; i < (int)bucket.get_length(); i++) {
            if (current->value.key == key) {
                return current->value.value;
            }
            current = current->next;
        }
        throw std::out_of_range("Key not found");
    }

    void remove(const TKey& key) override {
        int idx = get_index(key);
        LinkedList<KeyValuePair<TKey, TValue>>& bucket = buckets_[idx];

        if (bucket.get_length() == 0) {
            throw std::out_of_range("Key not found to remove");
        }

        LinkedListNode<KeyValuePair<TKey, TValue>>* current = bucket.get_node(0);
        LinkedListNode<KeyValuePair<TKey, TValue>>* prev = nullptr;

        while (current) {
            if (current->value.key == key) {
                bucket.remove_node(prev, current);
                count_--;

                ordered_keys_.remove_item(key);

                if (count_ <= (capacity_ / resize_threshold_factor) && capacity_ > 1) {
                    int new_cap = capacity_ / resize_factor;
                    if (new_cap < 16) new_cap = 16;
                    resize(new_cap);
                }
                return;
            }

            prev = current;
            current = current->next;
        }
        throw std::out_of_range("Key not found to remove");
    }

    bool contains_key(const TKey& key) const override {
        int idx = get_index(key);
        const LinkedList<KeyValuePair<TKey, TValue>>& bucket = buckets_[idx];

        if (bucket.get_length() == 0) {
            return false;
        }

        LinkedListNode<KeyValuePair<TKey, TValue>>* current = bucket.get_node(0);

        for (int i = 0; i < (int)bucket.get_length(); i++) {
            if (current->value.key == key) {
                return true;
            }
            current = current->next;
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

    IIterator<KeyValuePair<TKey, TValue>>* get_iterator() const {
        return new HashTableIterator(ordered_keys_, *this);
    }

private:
    DynamicArray<LinkedList<KeyValuePair<TKey, TValue>>> buckets_;
    ArraySequence<TKey> ordered_keys_;
    int capacity_;
    int count_;
    std::hash<TKey> hasher_;

    const double resize_threshold_factor = 2.0; // Сделать понятнее
    const double resize_factor = 2.0;

    int get_index(const TKey& key) const {
        size_t h = hasher_(key);
        return h % static_cast<size_t>(capacity_);
    }

    void resize(int new_capacity) {
        DynamicArray<LinkedList<KeyValuePair<TKey, TValue>>> new_buckets(new_capacity);

        // Переносим элементы из старых бакетов в новые
        for (int i = 0; i < capacity_; i++) {
            LinkedList<KeyValuePair<TKey, TValue>>& old_bucket = buckets_[i];
            if (old_bucket.get_length() == 0) {
                continue;
            }

            LinkedListNode<KeyValuePair<TKey, TValue>>* current = old_bucket.get_node(0);

            for (int j = 0; j < (int)old_bucket.get_length(); j++) {
                // Текущий pair
                auto& kvp = current->value;
                size_t h = hasher_(kvp.key);
                int new_idx = h % static_cast<size_t>(new_capacity);

                new_buckets[new_idx].append(kvp);

                current = current->next;
            }
        }

        buckets_ = new_buckets;
        capacity_ = new_capacity;
    }
};

#endif //LAB3_HASHTABLE_HPP
