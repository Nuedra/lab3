#ifndef LAB3_LINKEDIN_H
#define LAB3_LINKEDIN_H

#include <stdexcept>
#include <cstddef>

template<class T>
class LinkedListNode {
public:
    T value;
    LinkedListNode<T>* next;

    LinkedListNode(const T& value, LinkedListNode<T>* next = nullptr);
};

template<class T>
class LinkedList {
private:
    LinkedListNode<T>* head;
    LinkedListNode<T>* tail;
    size_t length;

    void Clear();

public:
    LinkedList();
    LinkedList(const T* items, int count);
    LinkedList(const LinkedList<T>& list);
    ~LinkedList();
    LinkedList<T>& operator=(const LinkedList<T>& other);

    LinkedList<T>* GetSubList(int startIndex, int endIndex);
    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    void RemoveAt(int index);

    size_t GetLength() const;
    T GetFirst() const;
    T GetLast() const;
    T Get(int index) const;
    void Set(int index, const T& value);
};

#include "LinkedList.tpp"


#endif //LAB3_LINKEDIN_H
