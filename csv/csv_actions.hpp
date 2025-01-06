#ifndef CSV_ACTIONS_HPP
#define CSV_ACTIONS_HPP

#include "person.hpp"
#include "../data_structures/ArraySequence.h"
#include <string>

ArraySequence<std::string> load_names_from_file(const std::string& filename);
std::string generate_random_name(const ArraySequence<std::string>& names);
int generate_random_int(int min, int max);
float generate_random_float(float min, float max);
void write_csv(const std::string& filename, const ArraySequence<person>& persons);
void generate_and_write_persons_to_file(int number_of_persons);
ArraySequence<person> read_csv(const std::string& filename);
void generate_and_write_sorted_persons_to_file(int number_of_persons);
void generate_and_write_reverse_sorted_persons_to_file(int number_of_persons);

#endif // CSV_ACTIONS_HPP
