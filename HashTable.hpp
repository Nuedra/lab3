#ifndef LAB3_HASHTABLE_HPP
#define LAB3_HASHTABLE_HPP

#include <functional> // Для std::hash
#include <stdexcept>
#include "data_structures/DynamicArray.h"
#include "data_structures/LinkedList.h"
#include "IDictionary.hpp"
#include "IIterator.hpp"
#include "Pair.hpp"

template<typename TKey, typename TValue>
class HashTable : public IDictionary<TKey, TValue> {
public:
    explicit HashTable(int capacity = 16) : capacity_(capacity), count_(0) {
        buckets_ = DynamicArray<LinkedList<Pair<TKey, TValue>>>(capacity_);
    }

    ~HashTable() override = default;

    void add(const TKey& key, const TValue& value) override {
        int idx = get_index(key);
        LinkedList<Pair<TKey, TValue>>& bucket = buckets_[idx];
        // Проверяем, есть ли уже такой ключ
        for (int i = 0; i < bucket.GetLength(); i++) {
            auto pair = bucket.Get(i); // Квадратичная сложность от длины цепочки переполнения
            if (pair.key == key) {
                pair.value = value;
                bucket.Set(i, pair);
                return;
            }
        }

        Pair<TKey, TValue> new_pair{key, value};
        bucket.Append(new_pair);
        count_++;

        if (count_ == capacity_) {
            int new_cap = capacity_ * q_;
            if (new_cap < 1) new_cap = 1;
            resize(new_cap);
        }
    }

    TValue get(const TKey& key) const override {
        int idx = get_index(key);
        const LinkedList<Pair<TKey, TValue>>& bucket = buckets_[idx];
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
        LinkedList<Pair<TKey, TValue>>& bucket = buckets_[idx];
        for (int i = 0; i < bucket.GetLength(); i++) {
            auto pair = bucket.Get(i); // Квадратичная сложность от длинны цепочки переполнения
            if (pair.key == key) {
                bucket.RemoveAt(i);
                count_--;

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
        const LinkedList<Pair<TKey, TValue>>& bucket = buckets_[idx];
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
    class HashTableIterator : public IIterator<Pair<TKey, TValue>> {
    public:
        explicit HashTableIterator(const HashTable& table) : table_(table), bucket_index_(0), element_index_(0)
        {
            if (!move_to_first_element()) {
                // Если элементов нет, помечаем итератор как "за концом"
                bucket_index_ = table_.capacity_;
                element_index_ = 0;
            }
        }

        Pair<TKey, TValue> get_current_item() const override {
            if (!is_valid_position()) {
                throw std::out_of_range("Iterator out of range");
            }
            const auto& bucket = table_.buckets_[bucket_index_];
            return bucket.Get(element_index_);
        }

        bool has_next() const override {
            // Проверяем, можно ли сдвинуться на следующий элемент без изменения состояния итератора
            if (!is_valid_position()) return false;

            int b = bucket_index_;
            int e = element_index_;
            e++; // пробуем сдвинуться вперёд
            while (b < table_.capacity_) {
                const auto& bucket = table_.buckets_[b];
                if (e < bucket.GetLength()) {
                    // Следующий элемент существует
                    return true;
                }
                else{
                    // Переходим к следующему бакету
                    b++;
                    e = 0;
                }
            }
            return false; // Следующего элемента нет
        }

        bool next() override {
            // Пытаемся перейти к следующему элементу
            int b = bucket_index_;
            int e = element_index_;
            e++;
            while (b < table_.capacity_) {
                const auto& bucket = table_.buckets_[b];
                if (e < bucket.GetLength()) {
                    // Нашли следующий элемент
                    bucket_index_ = b;
                    element_index_ = e;
                    return true;
                }
                else{
                    b++;
                    e = 0;
                }
            }

            return false;
        }

        bool try_get_current_item(Pair<TKey, TValue>& element) const override {
            if (!is_valid_position()) return false;
            const auto& bucket = table_.buckets_[bucket_index_];
            element = bucket.Get(element_index_);
            return true;
        }

    private:
        const HashTable& table_;
        int bucket_index_;
        int element_index_;

        bool move_to_first_element() {
            // Ищем первый непустой бакет и первый элемент в нём
            while (bucket_index_ < table_.capacity_) {
                const auto& bucket = table_.buckets_[bucket_index_];
                if (bucket.GetLength() > 0) {
                    element_index_ = 0;
                    return true;
                }
                else{
                    bucket_index_++;
                }
            }
            return false;
        }

        bool is_valid_position() const {
            if (bucket_index_ < 0 || bucket_index_ >= table_.capacity_) return false;
            const auto& bucket = table_.buckets_[bucket_index_];
            return (element_index_ >= 0 && element_index_ < bucket.GetLength());
        }
    };

    // Метод для получения итератора
    IIterator<Pair<TKey, TValue>>* get_iterator() const {
        return new HashTableIterator(*this);
    }

private:
    DynamicArray<LinkedList<Pair<TKey, TValue>>> buckets_;
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
        DynamicArray<LinkedList<Pair<TKey, TValue>>> new_buckets(new_capacity);
        // Переносим элементы
        for (int i = 0; i < capacity_; i++) {
            LinkedList<Pair<TKey, TValue>>& old_bucket = buckets_[i];
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