#include "compare.hpp"

int compare_person_first_name(const person& a, const person& b) {
    return compare_default(a.first_name, b.first_name);
}

int compare_person_last_name(const person& a, const person& b) {
    return compare_default(a.last_name, b.last_name);
}

int compare_person_birth_year(const person& a, const person& b) {
    return compare_default(a.birth_year, b.birth_year);
}

int compare_person_height(const person& a, const person& b) {
    return compare_default(a.height, b.height);
}

int compare_person_weight(const person& a, const person& b) {
    return compare_default(a.weight, b.weight);
}

int compare_person_salary(const person& a, const person& b) {
    return compare_default(a.salary, b.salary);
}

int compare_person_salary_lastname(const person& a, const person& b) {
    int result = compare_person_salary(a, b);
    if (result != 0) return result;
    return compare_person_last_name(a, b);
}

int compare_person_salary_lastname_height(const person& a, const person& b) {
    int result = compare_person_salary(a, b);
    if (result != 0) return result;
    result = compare_person_last_name(a, b);
    if (result != 0) return result;
    return compare_person_height(a, b);
}
