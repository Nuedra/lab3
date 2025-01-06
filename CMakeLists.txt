cmake_minimum_required(VERSION 3.27)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS iostreams)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(lab3
        main.cpp
        tests.cpp
        tests.hpp
        IDictionary.hpp
        IIterator.hpp
        csv/person.hpp
        csv/csv_actions.hpp
        csv/csv_actions.cpp
        Pair.hpp
        sorter/compare.cpp
        histogram.cpp
        sorter/HeapSorter.hpp
        histogram_storage.cpp
        histogram_storage.hpp
        graphics.cpp
)

target_link_libraries(lab3 ${Boost_LIBRARIES})