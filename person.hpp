#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>

struct person {
    std::string first_name;
    std::string last_name;
    int birth_year{};
    float height{};
    float weight{};
    int salary{};

    bool operator==(const person& other) const {
        return first_name == other.first_name &&
               last_name == other.last_name &&
               birth_year == other.birth_year &&
               height == other.height &&
               weight == other.weight &&
               salary == other.salary;
    }
};

#endif // PERSON_HPP

