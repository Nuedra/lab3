#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <string>
#include "person.hpp"

template<typename T>
int compare_default(const T& a, const T& b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int compare_person_first_name(const person& a, const person& b);
int compare_person_last_name(const person& a, const person& b);
int compare_person_birth_year(const person& a, const person& b);
int compare_person_height(const person& a, const person& b);
int compare_person_weight(const person& a, const person& b);
int compare_person_salary(const person& a, const person& b);
int compare_person_salary_lastname(const person& a, const person& b);
int compare_person_salary_lastname_height(const person& a, const person& b);

#endif // COMPARE_HPP
