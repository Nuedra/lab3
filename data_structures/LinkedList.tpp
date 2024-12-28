#include "LinkedList.h"

template<class T>
LinkedListNode<T>::LinkedListNode(const T& value, LinkedListNode<T>* next)
        : value(value), next(next) {}

template<class T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), length(0) {}

template<class T>
LinkedList<T>::LinkedList(const T* items, int count) : head(nullptr), tail(nullptr), length(0) {
    for (int i = 0; i < count; ++i) {
        Append(items[i]);
    }
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
    if (this == &other) {
        return *this;
    }

    Clear();

    LinkedListNode<T>* current = other.head;
    while (current != nullptr) {
        Append(current->value);
        current = current->next;
    }

    return *this;
}


template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) : head(nullptr), tail(nullptr), length(0) {
    LinkedListNode<T>* current = list.head;
    while (current != nullptr) {
        Append(current->value);
        current = current->next;
    }
}

template<class T>
LinkedList<T>::~LinkedList() {
    Clear();
}

template<class T>
void LinkedList<T>::Clear() {
    while (head != nullptr) {
        LinkedListNode<T>* current = head;
        head = head->next;
        delete current;
    }
    tail = nullptr;
    length = 0;
}

template<class T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) {
    if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= (int)length) {
        throw std::out_of_range("Index out of range");
    }

    auto* subList = new LinkedList<T>();
    LinkedListNode<T>* current = this->head;
    for (int i = 0; i < startIndex; ++i) {
        current = current->next;
    }
    for (int i = startIndex; i <= endIndex; ++i) {
        subList->Append(current->value);
        current = current->next;
    }
    return subList;
}

template<class T>
void LinkedList<T>::Append(T item) {
    LinkedListNode<T>* node = new LinkedListNode<T>(item, nullptr);
    if (tail == nullptr) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    length++;
}

template<class T>
void LinkedList<T>::Prepend(T item) {
    LinkedListNode<T>* newNode = new LinkedListNode<T>(item, head);
    head = newNode;
    if (length == 0) {
        tail = newNode;
    }
    length++;
}

template<class T>
void LinkedList<T>::InsertAt(T item, int index) {
    if (index < 0 || index > (int)length) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        Prepend(item);
        return;
    }
    if (index == (int)length) {
        Append(item);
        return;
    }
    LinkedListNode<T>* current = head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }
    LinkedListNode<T>* newNode = new LinkedListNode<T>(item, current->next);
    current->next = newNode;
    length++;
}

template<class T>
void LinkedList<T>::RemoveAt(int index) {
    if (index < 0 || index >= (int)length) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        LinkedListNode<T>* temp = head;
        head = head->next;
        delete temp;
        length--;
        if (length == 0) tail = nullptr;
        return;
    }
    LinkedListNode<T>* current = head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }
    LinkedListNode<T>* toRemove = current->next;
    current->next = toRemove->next;
    if (toRemove == tail) tail = current;
    delete toRemove;
    length--;
}

template<class T>
void LinkedList<T>::RemoveNode(LinkedListNode<T>* prev, LinkedListNode<T>* node) {
    if (!node) {
        throw std::invalid_argument("Node is null");
    }

    if (node == head) {
        head = head->next;
        if (node == tail) {
            tail = nullptr;
        }
    }
    else if (prev) {
        prev->next = node->next;
        if (node == tail) {
            tail = prev;
        }
    }
    else {
        throw std::invalid_argument("Invalid previous node");
    }

    delete node;
    length--;
}

template<class T>
size_t LinkedList<T>::GetLength() const {
    return length;
}

template<class T>
T LinkedList<T>::GetFirst() const {
    if (head == nullptr) throw std::out_of_range("LinkedList is empty");
    return head->value;
}

template<class T>
T LinkedList<T>::GetLast() const {
    if (tail == nullptr) throw std::out_of_range("LinkedList is empty");
    return tail->value;
}

template<class T>
T LinkedList<T>::Get(int index) const {
    if (index < 0 || index >= (int)length) {
        throw std::out_of_range("Index out of range");
    }
    LinkedListNode<T>* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->value;
}

template<class T>
LinkedListNode<T>* LinkedList<T>::GetNode(int index) const {
    if (index < 0 || index >= (int)length) {
        throw std::out_of_range("Index out of range");
    }
    LinkedListNode<T>* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

template<class T>
void LinkedList<T>::Set(int index, const T& value) {
    if (index < 0 || index >= (int)length) {
        throw std::out_of_range("Index out of range");
    }
    LinkedListNode<T>* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    current->value = value;
}
