#pragma once
#include <string>
#include <vector>
#include "Trie.h"

class HashTable {

private:

    std::vector<std::vector<std::pair<std::string,Book>>> table;
    int size;
    int capacity;
    size_t retrieveDuration;
    size_t parsingDuration;


public:
    HashTable();

    void resize(int newCapacity);

    int hashFunction(std::string& key, int size);

    void insert(std::string& key, const Book& value);

    Book* retrieve(std::string& key);

    void remove(std::string& key);

    bool contains(std::string& key);

    std::string titleCleanup(std::string& title);

    void parseCSVHash(const std::string& filePath);

    size_t getRetrieveDuration();

    size_t getParsingDuration();


};

