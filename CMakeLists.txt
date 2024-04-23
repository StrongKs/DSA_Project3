cmake_minimum_required(VERSION 3.14)
project(DSA_P3)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)
add_executable(DSA_P3 main.cpp
        TextureManager.cpp
        TextureManager.h
        HashTable.cpp
        HashTable.h
        Trie.cpp
        Trie.h)
include_directories(/usr/local/include)

find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)
include_directories(/usr/local/Cellar/sfml/2.6.1/include)
target_link_libraries(DSA_P3 sfml-system sfml-window sfml-graphics sfml-audio sfml-network)