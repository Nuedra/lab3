#ifndef LAB3_LINKEDIN_H
#define LAB3_LINKEDIN_H

#include "../IIterator.hpp"

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

    size_t get_length() const;
    T get_first() const;
    T get_last() const;
    T get(int index) const;
    void set(int index, const T& value);
    bool remove_value(const T& value);

    void remove_at(int index);

    template<typename T_I>
    class LinkedListIterator : public IIterator<T_I> {
    public:
        explicit LinkedListIterator(LinkedListNode<T_I>* start_node): current_(start_node) {}

        T get_current_item() const override {
            if (!current_) {
                throw std::out_of_range("LinkedListIterator: out of range");
            }
            return current_->value;
        }

        bool has_next() const override {
            if (!current_) return false;
            return (current_->next != nullptr);
        }

        bool next() override {
            if (!current_) {
                return false;
            }
            current_ = current_->next;
            return (current_ != nullptr);
        }

        bool try_get_current_item(T& element) const override {
            if (!current_) {
                return false;
            }
            element = current_->value;
            return true;
        }

    private:
        LinkedListNode<T>* current_;
    };

    LinkedListIterator<T> get_iterator() const {
        return LinkedListIterator<T>(this->head);
    }

};

#include "LinkedList.tpp"

#endif //LAB3_LINKEDIN_H