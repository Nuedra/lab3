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
};

#endif // PERSON_HPP

