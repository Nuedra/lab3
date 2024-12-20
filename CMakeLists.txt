cmake_minimum_required(VERSION 3.29)
project(lab3)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab3
        main.cpp
        tests.cpp
        tests.hpp
        IDictionary.hpp
        IIterator.hpp
        person.hpp
        csv_actions.hpp
        csv_actions.cpp
        Pair.hpp
        compare.cpp
        histogram.cpp)
