#include "tests.hpp"
#include "person.hpp"
#include "iostream"
#include "Pair.hpp"
#include "csv_actions.hpp"
#include "histogram.hpp"
#include "data_structures/ArraySequence.h"
#include "histogram_storage.hpp"

int read_int_in_range(const char* prompt, int min_val, int max_val) {
    while (true) {
        std::cout << prompt;

        int value = 0;
        if (!(std::cin >> value)) {
            //  ввод не int, очищаем флаги и буфер
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Некорректный ввод, попробуйте снова.\n";
            continue;
        }

        std::cin.ignore(10000, '\n');

        if (value < min_val || value > max_val) {
            std::cout << "Значение должно быть в диапазоне ["
                      << min_val << ", " << max_val << "]. Попробуйте снова.\n";
            continue;
        }

        return value;
    }
}

int main() {
    start_tests();

    int start_year = 0;
    int end_year   = 0;

    start_year = read_int_in_range("Введите начало диапазона (не менее 1950): ",1950,2010);

    while (true) {
        end_year = read_int_in_range("Введите конец диапазона (не более 2010, но больше начала): ",1950,2010);
        if (end_year <= start_year) {
            std::cout << "Конец диапазона должен быть больше начала!\n";
            continue;
        }
        break;
    }

    int range = end_year - start_year;
    int intervals_count = 0;

    while (true) {
        std::cout << "Введите желаемое количество интервалов (<= " << range << "): ";
        if (!(std::cin >> intervals_count)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Некорректный ввод, попробуйте снова.\n";
            continue;
        }

        std::cin.ignore(10000, '\n'); // очистка буфера

        if (intervals_count <= 0) {
            std::cout << "Количество интервалов должно быть положительным!\n";
            continue;
        }
        if (intervals_count > range) {
            std::cout << "Количество интервалов не может превышать " << range << ".\n";
            continue;
        }
        break;
    }

    ArraySequence<person> persons = read_csv("../csv/data.csv");

    HashTable<Pair<int,int>, int> histogram = build_histogram(persons, start_year, end_year, intervals_count);

    IIterator<KeyValuePair<Pair<int,int>, int>>* it = histogram.get_iterator();
    KeyValuePair<Pair<int,int>, int> current{};

    std::cout << "\nПостроенная гистограмма:\n";
    while (it->try_get_current_item(current)) {
        std::cout << "Count in [" << current.key.value1 << ", " << current.key.value2 << "): " << current.value << "\n";
        it->next();
    }
    delete it;

    save_histogram_text(histogram, "../histogram_data.txt");
    save_histogram_binary(histogram, "../histogram_data.bin");
    std::cout << "\nГистограмма сохранена в:\n" << "  ../histogram_data.txt (текстовый формат)\n"
    << "  ../histogram_data.bin (бинарный формат)\n";

    return 0;
}
