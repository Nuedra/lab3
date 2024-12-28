#include "histogram_storage.hpp"
#include <fstream>
#include <stdexcept>
#include "IIterator.hpp"

// функция сохранения в поток (text/binary).
static void save_histogram_generic(const HashTable<Pair<int,int>, int>& histogram, std::ostream& out, bool is_binary) {

    int item_count = histogram.get_count();
    if (is_binary) {
        out.write(reinterpret_cast<const char*>(&item_count), sizeof(item_count));
    }
    else {
        out << item_count << "\n";
    }

    HashTable<Pair<int,int>, int>::HashTableIterator it = histogram.get_iterator();
    KeyValuePair<Pair<int,int>, int> kvp;

    while (it.try_get_current_item(kvp)) {
        int start = kvp.key.value1;
        int end   = kvp.key.value2;
        int count = kvp.value;

        if (is_binary) {
            // Пишем три int
            out.write(reinterpret_cast<const char*>(&start), sizeof(start));
            out.write(reinterpret_cast<const char*>(&end),   sizeof(end));
            out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        }
        else {
            // Пишем в текстовом формате
            out << start << " " << end << " " << count << "\n";
        }

        it.next();
    }
}

//  функция загрузки из потока (text/binary).
static HashTable<Pair<int,int>, int> load_histogram_generic(std::istream& in, bool is_binary){

    int item_count = 0;
    if (is_binary) {
        in.read(reinterpret_cast<char*>(&item_count), sizeof(item_count));
        if (!in.good()) {
            throw std::runtime_error("Error reading item_count (binary)");
        }
    }
    else {
        in >> item_count;
        if (!in.good()) {
            throw std::runtime_error("Error reading item_count (text)");
        }
    }

    HashTable<Pair<int,int>, int> histogram(item_count * 2);

    for (int i = 0; i < item_count; i++) {
        int start = 0, end = 0, count = 0;
        if (is_binary) {
            in.read(reinterpret_cast<char*>(&start), sizeof(start));
            in.read(reinterpret_cast<char*>(&end),   sizeof(end));
            in.read(reinterpret_cast<char*>(&count), sizeof(count));
            if (!in.good()) {
                throw std::runtime_error("Error reading item data (binary)");
            }
        } else {
            in >> start >> end >> count;
            if (!in.good()) {
                throw std::runtime_error("Error reading item data (text)");
            }
        }

        Pair<int,int> interval{start, end};
        histogram.add(interval, count);
    }

    return histogram;
}

void save_histogram_text(const HashTable<Pair<int,int>, int>& histogram, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    save_histogram_generic(histogram, out, false);

    out.close();
}

HashTable<Pair<int,int>, int> load_histogram_text(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        throw std::runtime_error("Failed to open file for reading: " + filename);
    }

    HashTable<Pair<int,int>, int> histogram = load_histogram_generic(in, false);

    in.close();
    return histogram;
}

void save_histogram_binary(const HashTable<Pair<int,int>, int>& histogram, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open file for writing (binary): " + filename);
    }

    save_histogram_generic(histogram, out, true);

    out.close();
}

HashTable<Pair<int,int>, int> load_histogram_binary(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        throw std::runtime_error("Failed to open file for reading (binary): " + filename);
    }

    HashTable<Pair<int,int>, int> histogram = load_histogram_generic(in, true);

    in.close();
    return histogram;
}
