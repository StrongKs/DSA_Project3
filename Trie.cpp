//
// Created by Kent Phipps on 4/17/24.
//
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>  // For std::tolower
#include <queue>
#include <chrono>

#include "Trie.h"

// Converts string into only 26 char word for Trie ex. "Harry Potter" -> "harrypotter"
std::string toLowerAndRemoveNonLetters(const std::string& input) {
    std::string output = input;

    // Transform all chars to lowercase
    std::transform(output.begin(), output.end(), output.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // Remove chars that are not lowercase letters
    output.erase(std::remove_if(output.begin(), output.end(),
                                [](char c) { return !std::islower(c); }),
                 output.end());

    return output;
}

void Trie::insert(string &key, Book *book) {
    // Initialize currentNode
    TrieNode* currentNode = this->root;

    // Iterate over each letter in key
    for (auto letter : key) {

        // Check if node exist for the current letter
        if (currentNode->children[letter - 'a'] == nullptr) {

            // Make a new node
            TrieNode* newNode = new TrieNode();
            currentNode->children[letter - 'a'] = newNode;
        }
        // Move to next node
        currentNode = currentNode->children[letter - 'a'];
    }

    // Increment word count and add book
    currentNode->wordCount++;
    currentNode->books.push_back(book);
}

void Trie::parseBookCSV(string& filePath, bool searchByTitle) {
    // Start time parsing
    auto start = std::chrono::high_resolution_clock::now();

    ifstream file(filePath);
    string line;

    // Skip header line
    getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Book* book = new Book();
        std::string temp;

        // Assign attributes to book
        std::getline(ss, temp, '\t');
        std::getline(ss, temp, '\t');
        std::getline(ss, book->id, '\t');
        std::getline(ss, book->title, '\t');
        std::getline(ss, book->description, '\t');
        std::getline(ss, book->authors, '\t');
        std::getline(ss, book->categories, '\t');
        std::getline(ss, temp, '\t');
        book->avgRating = stof(temp);
        std::getline(ss, temp, '\t');
        book->isMature = (temp == "MATURE");
        std::getline(ss, book->publishDate, '\t');
        std::getline(ss, temp, '\r');
        book->pageCount = stoi(temp);

//        book->print();

        if (searchByTitle) {
            string lowerCaseBookTitle = toLowerAndRemoveNonLetters(book->title);
            // Insert Book object into Trie
            insert(lowerCaseBookTitle, book);

        } else {
            string lowerCaseBookTitle = toLowerAndRemoveNonLetters(book->authors);
            // Insert Book object into Trie
            insert(lowerCaseBookTitle, book);
        }

    }

    if (searchByTitle)
        cout << "Trie made using book Titles" << endl;
    else
        cout << "Trie made using book authors" << endl;

    // End time parsing
    auto stop = std::chrono::high_resolution_clock::now();
    parsingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Parsing Duration: " << parsingDuration << " milliseconds" << std::endl;
}

bool Trie::isPrefixExist(string& key) {
    key = toLowerAndRemoveNonLetters(key);

    // Initialize currNode
    TrieNode* currentNode = this->root;

    // Iterate over each letter in key
    for (auto letter : key) {

        // Check if node exists
        if (currentNode->children[letter - 'a'] == nullptr) {
            return false;
        }

        // Move to next node
        currentNode = currentNode->children[letter - 'a'];
    }

    // Prefix exist in Trie
    return true;
}

vector<Book*> Trie::prefixSearch(string& key) {
    key = toLowerAndRemoveNonLetters(key);

    // Start time search
    auto start = std::chrono::high_resolution_clock::now();

    // Initialize currNode
    TrieNode* currentNode = this->root;

    vector<Book*> resBooks;

    // Iterate over each char
    for (auto letter : key) {

        // Check if node exists
        if (currentNode->children[letter - 'a'] == nullptr) {

            return resBooks;    // return empty vector
        }

        // Move to next node
        currentNode = currentNode->children[letter - 'a'];
    }

    // Make queue to bfs every book from currentNode
    queue<TrieNode*> qBooks;
    qBooks.push(currentNode);

    while (!qBooks.empty()) {
        currentNode = qBooks.front();
        qBooks.pop();

        // print current books
        if (currentNode->wordCount > 0) {
            for (auto *book : currentNode->books)
                resBooks.push_back(book);
        }
        for (auto & i : currentNode->children) {
            if (i != nullptr)
                qBooks.push(i);
        }
    }

    // Start time search
    auto stop = std::chrono::high_resolution_clock::now();
    retreiveDuration = std::chrono::duration_cast<std::chrono::microseconds >(stop - start).count();
    std::cout << "Retreive Duration: " << retreiveDuration << " microseconds" << std::endl;

    return resBooks;
}

size_t Trie::getRetreiveDuration() {
    return retreiveDuration;
}

size_t Trie::getParsingDuration() {
    return parsingDuration;
}