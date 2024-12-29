#include "tests.hpp"
#include "person.hpp"
#include "iostream"
#include "Pair.hpp"
#include "histogram.hpp"
#include "data_structures/LinkedList.h"

void int_tests_hash_table(){
    {
        HashTable<int, int> table(10);

        table.add(42, 100);
        assert(table.get_count() == 1);
        assert(table.contains_key(42));
        assert(table.get(42) == 100);

        table.add(10, 200);
        assert(table.get_count() == 2);
        assert(table.contains_key(10));
        assert(table.get(10) == 200);

        table.add(42, 300);
        assert(table.get_count() == 2);
        assert(table.get(42) == 300);

        table.add(15, 300);
        assert(table.get_count() == 3);
        assert(table.contains_key(15));
        assert(table.get(15) == 300);

        table.remove(10);
        assert(table.get_count() == 2);
        assert(!table.contains_key(10));

        bool exception_thrown = false;
        try {
            table.get(10);
        }
        catch (const std::out_of_range&) {
            exception_thrown = true;
        }
        assert(exception_thrown);
    }
}

void test_hash_table_compression_expansion(){
    HashTable<int, std::string> table(16);
    table.add(1, "one");
    table.add(2, "two");
    table.add(3, "three");
    table.add(4, "four");
    table.add(5, "five");
    table.add(6, "six");
    table.add(7, "seven");
    table.add(8, "eight");
    table.add(9, "nine");
    table.add(10, "ten");
    table.add(11, "eleven");
    table.add(12, "twelve");
    table.add(13, "thirteen");
    table.add(14, "fourteen");
    table.add(15, "fifteen");

    assert(table.get_count() == 15);
    assert(table.get_capacity() == 16);

    table.add(16, "sixteen");

    assert(table.get_count() == 16);
    assert(table.get_capacity() == 32);

    table.remove(16);

    assert(table.get_count() == 15);
    assert(table.get_capacity() == 16);

}


void test_hash_table_iterator() {
    {
        HashTable<int, int> table(10);
        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        assert(!it.try_get_current_item(element));
        assert(!it.has_next());
        // нет следующего элемента, next() вернёт false
        assert(!it.next());
    }

    {
        HashTable<int, int> table(10);
        table.add(1, 100);
        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        assert(it.try_get_current_item(element));
        assert(element.key == 1 && element.value == 100);
        assert(!it.has_next());
        //нет следующего элемента, next() вернёт false
        assert(!it.next());
    }

    {
        HashTable<int, int> table(10);
        table.add(1, 100);
        table.add(2, 200);
        table.add(3, 300);

        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        int count = 0;
        while (it.try_get_current_item(element)) {
            assert((element.key == 1 && element.value == 100) ||
                   (element.key == 2 && element.value == 200) ||
                   (element.key == 3 && element.value == 300));
            count++;
            // переходим к следующему, если его нет, next() вернёт false
            if (!it.next()) {
                //  достигнут конец
                break;
            }
        }
        assert(count == 3);
    }

    {
        HashTable<int, int> table(10);
        table.add(1, 100);
        table.add(2, 200);
        table.remove(1);

        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        assert(it.try_get_current_item(element));
        assert(element.key == 2 && element.value == 200);
        assert(!it.has_next());
        assert(!it.next());
    }

    {
        HashTable<int, int> table(10);
        for (int i = 0; i < 100; ++i) {
            table.add(i, i * 10);
        }

        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        int count = 0;
        while (it.try_get_current_item(element)) {
            assert(element.value == element.key * 10);
            count++;
            if (!it.next()) {
                break;
            }
        }
        assert(count == 100);
    }

    {
        HashTable<int, int> table(10);
        table.add(1, 100);

        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        assert(it.try_get_current_item(element));
        assert(element.key == 1 && element.value == 100);
        assert(!it.next());
        assert(element.key == 1 && element.value == 100);
    }

}

void pair_tests_hash_table() {
    // ключи –  Pair<int,int>
    HashTable<Pair<int,int>, int> table(10);

    Pair<int,int> p1{42, 10};
    Pair<int,int> p2{10, 20};
    Pair<int,int> p3{42, 10};
    Pair<int,int> p4{15, 15};
    table.add(p1, 100);
    assert(table.get_count() == 1);
    assert(table.contains_key(p1));
    assert(table.get(p1) == 100);

    table.add(p2, 200);
    assert(table.get_count() == 2);
    assert(table.contains_key(p2));
    assert(table.get(p2) == 200);

    table.add(p3, 300);
    assert(table.get_count() == 2);
    assert(table.get(p1) == 300);

    table.add(p4, 300);
    assert(table.get_count() == 3);
    assert(table.contains_key(p4));
    assert(table.get(p4) == 300);

    table.remove(p2);
    assert(table.get_count() == 2);
    assert(!table.contains_key(p2));

    bool exception_thrown = false;
    try {
        table.get(p2);
    }
    catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
}

void test_linked_list_iterator() {
    {
        LinkedList<int> list;
        auto it = list.get_iterator();
        int element;
        assert(!it.try_get_current_item(element));
        assert(!it.has_next());
        // Нет следующего элемента, next() вернёт false
        assert(!it.next());
    }

    {
        LinkedList<int> list;
        list.append(100);
        auto it = list.get_iterator();
        int element;
        assert(it.try_get_current_item(element));
        assert(element == 100);
        assert(!it.has_next());
        // Нет следующего элемента, next() вернёт false
        assert(!it.next());
    }

    {
        LinkedList<int> list;
        list.append(100);
        list.append(200);
        list.append(300);

        auto it = list.get_iterator();
        int element;
        int count = 0;
        while (it.try_get_current_item(element)) {
            assert(element == 100 || element == 200 || element == 300);
            count++;
            // Переходим к следующему, если его нет, next() вернёт false
            if (!it.next()) {
                // Достигнут конец
                break;
            }
        }
        assert(count == 3);
    }

    {
        LinkedList<int> list;
        list.append(100);
        list.append(200);
        list.remove_value(100);

        auto it = list.get_iterator();
        int element;
        assert(it.try_get_current_item(element));
        assert(element == 200);
        assert(!it.has_next());
        assert(!it.next());
    }

    {
        LinkedList<int> list;
        for (int i = 0; i < 100; ++i) {
            list.append(i * 10);
        }

        auto it = list.get_iterator();
        int element;
        int count = 0;
        while (it.try_get_current_item(element)) {
            assert(element == count * 10);
            count++;
            if (!it.next()) {
                break;
            }
        }
        assert(count == 100);
    }

    {
        LinkedList<int> list;
        list.append(100);

        auto it = list.get_iterator();
        int element;
        assert(it.try_get_current_item(element));
        assert(element == 100);
        assert(!it.next());
        assert(element == 100);
    }
}

void start_tests(){
    test_hash_table_compression_expansion();
    int_tests_hash_table();
    test_hash_table_iterator();
    pair_tests_hash_table();
    test_linked_list_iterator();
    std::cout << "All tests passed successfully!" << std::endl;
}
