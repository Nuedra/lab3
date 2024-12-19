#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdexcept>

#define INDEX_OUT_OF_RANGE "Index out of range"
#define NEGATIVE_SIZE "New size must be greater than or equal to zero."

template<typename T>
class DynamicArray {
private:
    T* data;
    size_t length;

public:
    DynamicArray();
    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& other);
    DynamicArray<T>& operator=(const DynamicArray<T>& other);
    ~DynamicArray();

    const T& get(int index) const;
    T& get(int index);
    void set(int index, const T& value);
    int get_size() const;
    void resize(int new_size);

    T& operator[](int index);
    const T& operator[](int index) const;
};

#include "DynamicArray.tpp"

#endif // DYNAMIC_ARRAY_H
