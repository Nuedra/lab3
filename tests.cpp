#include "tests.hpp"
#include "person.hpp"
#include "iostream"
#include "Pair.hpp"
#include "histogram.hpp"
#include "data_structures/ArraySequence.h"

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
    HashTable<int, std::string> table(4);
    table.add(1, "one");
    table.add(2, "two");
    assert(table.get_count() == 2);
    assert(table.contains_key(1));
    assert(table.contains_key(2));
    assert(table.get(1) == "one");
    assert(table.get(2) == "two");

    table.add(1, "ONE");
    assert(table.get_count() == 2);
    assert(table.get(1) == "ONE");

    table.add(3, "three");
    assert(table.get_count() == 3);
    assert(table.get_capacity() == 4);
    table.add(4, "four");

    assert(table.get_capacity() == 8);
    assert(table.get_count() == 4);

    assert(table.get(1) == "ONE");
    assert(table.get(2) == "two");
    assert(table.get(3) == "three");
    assert(table.get(4) == "four");

    table.remove(4);
    assert(table.get_capacity() == 4);
    assert(table.get_count() == 3);

    assert(table.contains_key(1));
    assert(table.contains_key(2));
    assert(table.contains_key(3));

    table.remove(1);
    table.remove(2);
    table.remove(3);

    assert(table.get_count() == 0);
    assert(table.get_capacity() == 1);

    assert(!table.contains_key(1));
}

void test_hash_table_iterator() {
    {
        HashTable<int, int> table(10);
        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        assert(!it->try_get_current_item(element));
        assert(!it->has_next());
        // Нет следующего элемента, next() вернёт false, итератор выведет сообщение в cerr
        assert(!it->next());
        delete it;
    }

    {
        HashTable<int, int> table(10);
        table.add(1, 100);
        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        assert(it->try_get_current_item(element));
        assert(element.key == 1 && element.value == 100);
        assert(!it->has_next());
        // Поскольку нет следующего элемента, next() вернёт false и выведет в cerr
        assert(!it->next());
        delete it;
    }

    {
        HashTable<int, int> table(10);
        table.add(1, 100);
        table.add(2, 200);
        table.add(3, 300);

        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        int count = 0;
        while (it->try_get_current_item(element)) {
            // Проверяем, что элемент правильный
            assert((element.key == 1 && element.value == 100) ||
                   (element.key == 2 && element.value == 200) ||
                   (element.key == 3 && element.value == 300));
            count++;
            // Переходим к следующему, если его нет, next() вернёт false
            if (!it->next()) {
                // Значит достигнут конец
                break;
            }
        }
        assert(count == 3);
        delete it;
    }

    {
        HashTable<int, int> table(10);
        table.add(1, 100);
        table.add(2, 200);
        table.remove(1);

        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        assert(it->try_get_current_item(element));
        assert(element.key == 2 && element.value == 200);
        assert(!it->has_next());
        assert(!it->next());
        delete it;
    }

    {
        HashTable<int, int> table(10);
        for (int i = 0; i < 100; ++i) {
            table.add(i, i * 10);
        }

        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        int count = 0;
        while (it->try_get_current_item(element)) {
            assert(element.value == element.key * 10);
            count++;
            if (!it->next()) {
                // Достигнут конец
                break;
            }
        }
        assert(count == 100);
        delete it;
    }

    {
        HashTable<int, int> table(10);
        table.add(1, 100);

        auto it = table.get_iterator();
        KeyValuePair<int, int> element{};
        assert(it->try_get_current_item(element));
        assert(element.key == 1 && element.value == 100);
        // Переходим дальше, нет следующего элемента
        assert(!it->next());
        // Теперь итератор снова указывает на элемент
        assert(element.key == 1 && element.value == 100);
        delete it;
    }

}

void pair_tests_hash_table() {
    {
        // Ключи – именно Pair<int,int>
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

        // Добавляем p3 (эквивалентен p1 по ==), значение должно обновиться
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
}

void start_tests(){
    test_hash_table_compression_expansion();
    int_tests_hash_table();
    test_hash_table_iterator();
    pair_tests_hash_table();
    std::cout << "All tests passed successfully!" << std::endl;
}
