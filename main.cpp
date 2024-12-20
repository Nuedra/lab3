#include "tests.hpp"
#include "person.hpp"
#include "iostream"
#include "Pair.hpp"
#include "csv_actions.hpp"
#include "histogram.hpp"
#include "data_structures/ArraySequence.h"

void histogram_person_tests() {
    ArraySequence<person> persons = read_csv("../csv/data.csv");

    int start_year = 1950;
    int end_year = 2010;
    int step = 10;

    auto histogram = build_histogram(persons, start_year, end_year, step);

    auto it = histogram.get_iterator();
    Pair<Pair<int, int>, int> current{};

    do {
        current = it->get_current_item();
        // current.key - это Pair<int,int>, current.value - int
        std::cout << "Count in [" << current.key.key << ", " << current.key.value << "): "
                  << current.value << "\n";
    } while (it->next());

    delete it;
}

int main() {
    generate_and_write_persons_to_file(100);
    histogram_person_tests();
    start_tests();
    return 0;
}
