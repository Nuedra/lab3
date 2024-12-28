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

    void clear();

public:
    LinkedList();
    LinkedList(const T* items, int count);
    LinkedList(const LinkedList<T>& list);
    ~LinkedList();
    LinkedList<T>& operator=(const LinkedList<T>& other);

    LinkedList<T>* get_sub_list(int start_index, int end_index);
    void append(T item);
    void prepend(T item);
    void insert_at(T item, int index);
    void remove_at(int index);
    void remove_node(LinkedListNode<T>* prev, LinkedListNode<T>* node);

    size_t get_length() const;
    T get_first() const;
    T get_last() const;
    T get(int index) const;
    LinkedListNode<T>* get_node(int index) const;
    void set(int index, const T& value);
};

#include "LinkedList.tpp"

#endif //LAB3_LINKEDIN_H