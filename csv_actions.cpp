#include "csv_actions.hpp"
#include "data_structures/ArraySequence.h"
#include "person.hpp"
#include <string>
#include "HeapSorter.hpp"
#include "compare.hpp"
#include <fstream>
#include <sstream>
#include <random>
#include <iostream>

ArraySequence<std::string> load_names_from_file(const std::string& filename) {
    ArraySequence<std::string> names;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return names;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            names.append(line);
        }
    }
    file.close();
    return names;
}

std::string generate_random_name(const ArraySequence<std::string>& names) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, names.get_length() - 1);
    return names.get(distr(gen));
}

int generate_random_int(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

float generate_random_float(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distr(min, max);
    return distr(gen);
}

void write_csv(const std::string& filename, const ArraySequence<person>& persons) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file << "first_name,last_name,birth_year,height,weight,salary\n";

    for (int i = 0; i < persons.get_length(); ++i) {
        const auto& person = persons.get(i);
        file << person.first_name << ","
             << person.last_name << ","
             << person.birth_year << ","
             << person.height << ","
             << person.weight << ","
             << person.salary << "\n";
    }

    file.close();
}

void generate_and_write_persons_to_file(int number_of_persons) {
    ArraySequence<std::string> first_names = load_names_from_file("../names/first_names.txt");
    ArraySequence<std::string> last_names = load_names_from_file("../names/last_names.txt");

    ArraySequence<person> persons;

    for (int i = 0; i < number_of_persons; ++i) {
        person person;
        person.first_name = generate_random_name(first_names);
        person.last_name = generate_random_name(last_names);
        person.birth_year = generate_random_int(1950, 2010);
        person.height = generate_random_float(150.0, 200.0);
        person.weight = generate_random_float(50.0, 100.0);
        person.salary = generate_random_int(30000, 80000);
        persons.append(person);
    }

    write_csv("../csv/data.csv", persons);
}

ArraySequence<person> read_csv(const std::string& filename) {
    ArraySequence<person> sequence;

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    std::getline(file, line); // Пропускаем заголовок

    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string first_name, last_name, birth_year, height, weight, salary;

        std::getline(stream, first_name, ',');
        std::getline(stream, last_name, ',');
        std::getline(stream, birth_year, ',');
        std::getline(stream, height, ',');
        std::getline(stream, weight, ',');
        std::getline(stream, salary, ',');

        person person;
        person.first_name = first_name;
        person.last_name = last_name;
        person.birth_year = std::stoi(birth_year);
        person.height = std::stof(height);
        person.weight = std::stof(weight);
        person.salary = std::stoi(salary);

        sequence.append(person);
    }

    file.close();
    return sequence;
}

void generate_and_write_sorted_persons_to_file(int number_of_persons) {
    generate_and_write_persons_to_file(number_of_persons);
    ArraySequence<person> sequence = read_csv("../csv/data.csv");
    HeapSorter<person> sorter;
    sorter.Sort(sequence, compare_person_salary);
    write_csv("../csv/sorted.csv", sequence);
}

void generate_and_write_reverse_sorted_persons_to_file(int number_of_persons) {
    generate_and_write_persons_to_file(number_of_persons);
    ArraySequence<person> sequence = read_csv("../csv/data.csv");
    HeapSorter<person> sorter;
    sorter.Sort(sequence, compare_person_salary);

    // Обратный порядок
    int len = sequence.get_length();
    for (int i = 0; i < len / 2; ++i) {
        sequence.swap(i, len - i - 1);
    }

    write_csv("../csv/reverse_sorted.csv", sequence);
}
