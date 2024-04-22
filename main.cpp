#include <iostream>

#include "Trie.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    string filePath = "GoogleBookAPIDataset.txt";

    // Choose data structure
    bool usingHash = false;
    bool usingTrie = false;

    // Choose Search Field (maybe implement)
    bool searchTitle = false;
    bool searchAuthor = false;

    // input from user
    string inputField = "";

    // When user searches
    bool isSearching = false;

    if (isSearching and usingHash) {

    } else if (isSearching and usingTrie) {
        // create Trie object
        Trie booksTest;

        booksTest.parseBookCSV(filePath);

        // check if prefix exist (bool)
        bool harryExist = booksTest.isPrefixExist(inputField);
        cout << "harry prefix exists: " << harryExist << endl << endl;

        if (harryExist) {
            cout << "PRINTING BOOKS" << endl;
            vector<Book*> b = booksTest.prefixSearch(inputField);

            for (auto *book : b)
                book->print();
        } else {
            cout << "NO BOOKS WITH PREFIX: " << inputField << endl;
        }

    }

    return 0;
}
