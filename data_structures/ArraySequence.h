#ifndef ARRAY_SEQUENCE_H
#define ARRAY_SEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"

#define INDEX_OUT_OF_RANGE "Index out of range"
#define EMPTY_ARRAY_SEQ "Array is empty"

template<typename T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* data;
    int length;
    int capacity;

    void ensure_capacity(int min_capacity);

public:
    ArraySequence();
    ArraySequence(const T* items, int count);
    ArraySequence(const ArraySequence<T>& other);
    ArraySequence<T>& operator=(const ArraySequence<T>& other);
    ~ArraySequence();

    // Переопределение виртуальных методов
    const T& get_first() const override;
    const T& get_last() const override;
    const T& get(int index) const override;
    T& get(int index) override;

    Sequence<T>* get_subSequence(int start_index, int end_index) const override;
    int get_length() const override;
    void append(const T& item) override;
    void prepend(const T& item) override;
    int index_of(const T& item) const override;
    bool contains(const T& item) const override;
    void insert_at(const T& item, int index) override;
    void remove_at(int index) override;
    bool remove_item(const T& item) override;
    void set(int index, const T& item) override;
    Sequence<T>* concat(Sequence<T>* list) const override;
    void swap(int index1, int index2) override;
    void reserve(int new_capacity) override;
};

#include "ArraySequence.tpp"

#endif // ARRAY_SEQUENCE_H
