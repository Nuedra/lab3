#include "HashTable.hpp"

void int_tests_hash_table(){
    {
        HashTable<int, int> table(10);

        table.Add(42, 100);
        assert(table.GetCount() == 1);
        assert(table.ContainsKey(42));
        assert(table.Get(42) == 100);

        table.Add(10, 200);
        assert(table.GetCount() == 2);
        assert(table.ContainsKey(10));
        assert(table.Get(10) == 200);

        table.Add(42, 300);
        assert(table.GetCount() == 2);
        assert(table.Get(42) == 300);

        table.Add(15, 300);
        assert(table.GetCount() == 3);
        assert(table.ContainsKey(15));
        assert(table.Get(15) == 300);

        table.Remove(10);
        assert(table.GetCount() == 2);
        assert(!table.ContainsKey(10));

        bool exceptionThrown = false;
        try {
            table.Get(10);
        }
        catch (const std::out_of_range&) {
            exceptionThrown = true;
        }
        assert(exceptionThrown);
    }
}

void test_hash_table_compression_expansion(){
    HashTable<int, std::string> table(4);
    table.Add(1, "one");
    table.Add(2, "two");
    assert(table.GetCount() == 2);
    assert(table.ContainsKey(1));
    assert(table.ContainsKey(2));
    assert(table.Get(1) == "one");
    assert(table.Get(2) == "two");

    table.Add(1, "ONE");
    assert(table.GetCount() == 2);
    assert(table.Get(1) == "ONE");

    table.Add(3, "three");
    assert(table.GetCount() == 3);
    assert(table.GetCapacity() == 4);
    table.Add(4, "four");

    assert(table.GetCapacity() == 8);
    assert(table.GetCount() == 4);

    assert(table.Get(1) == "ONE");
    assert(table.Get(2) == "two");
    assert(table.Get(3) == "three");
    assert(table.Get(4) == "four");

    table.Remove(4);
    assert(table.GetCapacity() == 4);
    assert(table.GetCount() == 3);

    assert(table.ContainsKey(1));
    assert(table.ContainsKey(2));
    assert(table.ContainsKey(3));

    table.Remove(1);
    table.Remove(2);
    table.Remove(3);

    assert(table.GetCount() == 0);
    assert(table.GetCapacity() == 1);

    assert(!table.ContainsKey(1));
}

void start_tests(){
    test_hash_table_compression_expansion();
    int_tests_hash_table();
    std::cout << "All tests passed successfully!" << std::endl;
}