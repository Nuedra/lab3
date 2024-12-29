#include "LinkedList.h"
#include <stdexcept>
#include <cstddef>

template<class T>
LinkedListNode<T>::LinkedListNode(const T& value, LinkedListNode<T>* next)
        : value(value), next(next) {}

template<class T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), length(0) {}

template<class T>
LinkedList<T>::LinkedList(const T* items, int count) : head(nullptr), tail(nullptr), length(0) {
    for (int i = 0; i < count; ++i) {
        append(items[i]);
    }
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
    if (this == &other) {
        return *this;
    }

    clear();

    LinkedListNode<T>* current = other.head;
    while (current != nullptr) {
        append(current->value);
        current = current->next;
    }

    return *this;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) : head(nullptr), tail(nullptr), length(0) {
    LinkedListNode<T>* current = list.head;
    while (current != nullptr) {
        append(current->value);
        current = current->next;
    }
}

template<class T>
LinkedList<T>::~LinkedList() {
    clear();
}

template<class T>
void LinkedList<T>::clear() {
    while (head != nullptr) {
        LinkedListNode<T>* current = head;
        head = head->next;
        delete current;
    }
    tail = nullptr;
    length = 0;
}

template<class T>
LinkedList<T>* LinkedList<T>::get_sub_list(int start_index, int end_index) {
    if (start_index < 0 || end_index < 0 || start_index > end_index || end_index >= (int)length) {
        throw std::out_of_range("Index out of range");
    }

    auto* sub_list = new LinkedList<T>();
    LinkedListNode<T>* current = this->head;
    for (int i = 0; i < start_index; ++i) {
        current = current->next;
    }
    for (int i = start_index; i <= end_index; ++i) {
        sub_list->append(current->value);
        current = current->next;
    }
    return sub_list;
}

template<class T>
void LinkedList<T>::append(T item) {
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
void LinkedList<T>::prepend(T item) {
    LinkedListNode<T>* new_node = new LinkedListNode<T>(item, head);
    head = new_node;
    if (length == 0) {
        tail = new_node;
    }
    length++;
}

template<class T>
void LinkedList<T>::insert_at(T item, int index) {
    if (index < 0 || index > (int)length) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        prepend(item);
        return;
    }
    if (index == (int)length) {
        append(item);
        return;
    }
    LinkedListNode<T>* current = head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }
    LinkedListNode<T>* new_node = new LinkedListNode<T>(item, current->next);
    current->next = new_node;
    length++;
}

template<class T>
bool LinkedList<T>::remove_value(const T& value) {
    if (!head) {
        return false;
    }

    if (head->value == value) {
        LinkedListNode<T>* tmp = head;
        head = head->next;
        if (!head) {
            tail = nullptr; // если список стал пуст
        }
        delete tmp;
        length--;
        return true;
    }

    LinkedListNode<T>* prev = head;
    LinkedListNode<T>* current = head->next;
    while (current) {
        if (current->value == value) {
            prev->next = current->next;
            if (current == tail) {
                tail = prev;
            }
            delete current;
            length--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

template<class T>
size_t LinkedList<T>::get_length() const {
    return length;
}

template<class T>
T LinkedList<T>::get_first() const {
    if (head == nullptr) throw std::out_of_range("LinkedList is empty");
    return head->value;
}

template<class T>
T LinkedList<T>::get_last() const {
    if (tail == nullptr) throw std::out_of_range("LinkedList is empty");
    return tail->value;
}

template<class T>
T LinkedList<T>::get(int index) const {
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
void LinkedList<T>::set(int index, const T& value) {
    if (index < 0 || index >= (int)length) {
        throw std::out_of_range("Index out of range");
    }
    LinkedListNode<T>* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    current->value = value;
}

template<class T>
void LinkedList<T>::remove_at(int index) {
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
    LinkedListNode<T>* to_remove = current->next;
    current->next = to_remove->next;
    if (to_remove == tail) tail = current;
    delete to_remove;
    length--;
}
