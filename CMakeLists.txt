cmake_minimum_required(VERSION 3.26)
project(P3)

set(CMAKE_CXX_STANDARD 23)

add_executable(P3 main.cpp
        ChainingHashTable.cpp
        hash_functions.h
        HashTable.h
        OpenAddrHashTable.h
        OpenAddrHashTable.cpp
        HashTable.cpp)
