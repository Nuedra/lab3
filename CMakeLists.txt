cmake_minimum_required(VERSION 3.29)
project(lab3)

set(CMAKE_CXX_STANDARD 14)

# Поиск Boost с необходимыми компонентами
find_package(Boost REQUIRED COMPONENTS iostreams)

if(Boost_FOUND)
    message(STATUS "Boost найден!")
    message(STATUS "Boost Include Dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost не найден!")
endif()

# Включение заголовочных файлов Boost
include_directories(${Boost_INCLUDE_DIRS})

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
        histogram.cpp
        HeapSorter.hpp
        histogram_storage.cpp
        histogram_storage.hpp
)

target_link_libraries(lab3 ${Boost_LIBRARIES})