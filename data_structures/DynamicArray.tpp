// DynamicArray.tpp
#ifndef DYNAMIC_ARRAY_TPP
#define DYNAMIC_ARRAY_TPP

#include "DynamicArray.h"

template<typename T>
DynamicArray<T>::DynamicArray() : data(nullptr), length(0) {}

template<typename T>
DynamicArray<T>::DynamicArray(T* items, int count) : length(count) {
    data = new T[length];
    for (int i = 0; i < count; ++i) {
        data[i] = items[i];
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(int size) : length(size) {
    data = new T[length];
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other)
        : length(other.length) {
    data = new T[length];
    for (size_t i = 0; i < length; ++i) {
        data[i] = other.data[i];
    }
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other) {
    if (this != &other) {
        delete[] data;
        data = nullptr;
        length = other.length;
        data = new T[length];
        for (size_t i = 0; i < length; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
    data = nullptr;
}

template<typename T>
const T& DynamicArray<T>::get(int index) const {
    if (index < 0 || index >= static_cast<int>(length)) {
        throw std::out_of_range(INDEX_OUT_OF_RANGE);
    }
    return data[index];
}

template<typename T>
T& DynamicArray<T>::get(int index) {
    if (index < 0 || index >= static_cast<int>(length)) {
        throw std::out_of_range(INDEX_OUT_OF_RANGE);
    }
    return data[index];
}

template<typename T>
void DynamicArray<T>::set(int index, const T& value) {
    if (index < 0 || index >= static_cast<int>(length)) {
        throw std::out_of_range(INDEX_OUT_OF_RANGE);
    }
    data[index] = value;
}

template<typename T>
int DynamicArray<T>::get_size() const {
    return static_cast<int>(length);
}

template<typename T>
void DynamicArray<T>::resize(int new_size) {
    if (new_size < 0) {
        throw std::length_error(NEGATIVE_SIZE);
    }

    T* new_data = (new_size > 0) ? new T[new_size] : nullptr;
    int elements_to_copy = (new_size > static_cast<int>(length)) ? static_cast<int>(length) : new_size;

    for (int i = 0; i < elements_to_copy; ++i) {
        new_data[i] = data[i];
    }

    delete[] data;
    data = new_data;
    length = new_size;
}

template<typename T>
T& DynamicArray<T>::operator[](int index) {
    return get(index);
}

template<typename T>
const T& DynamicArray<T>::operator[](int index) const {
    return get(index);
}

#endif // DYNAMIC_ARRAY_TPP
