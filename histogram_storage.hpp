#ifndef LAB3_HISTOGRAM_STORAGE_HPP
#define LAB3_HISTOGRAM_STORAGE_HPP

#include <string>
#include "HashTable.hpp"
#include "Pair.hpp"


static void save_histogram_generic(const HashTable<Pair<int,int>, int>& histogram, std::ostream& out, bool is_binary);
static HashTable<Pair<int,int>, int> load_histogram_generic(std::istream& in, bool is_binary);
void save_histogram_text(const HashTable<Pair<int,int>, int>& histogram, const std::string& filename);
HashTable<Pair<int,int>, int> load_histogram_text(const std::string& filename);
void save_histogram_binary(const HashTable<Pair<int,int>, int>& histogram, const std::string& filename);
HashTable<Pair<int,int>, int> load_histogram_binary(const std::string& filename);

#endif //LAB3_HISTOGRAM_STORAGE_HPP
