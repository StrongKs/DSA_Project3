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
    string inputFiled = "";

    // When user searches
    bool isSearching = false;

    if (isSearching and usingHash) {

        // Create a hash table
        HashTable ht;

        // Parse the book CSV file and insert into the hash table
        ht.parseCSVHash("GoogleBookAPIDataset.txt");

        // Test getting book by title
        std::string searchTitle = "A Second Chance at Eden";
        std::string lowerCaseSearchTitle = ht.titleCleanup(searchTitle);
        Book* book = ht.retrieve(lowerCaseSearchTitle);
        if (book) {
            std::cout << "Book found: " << book->title << std::endl;
        } else {
            std::cout << "Book not found" << std::endl;
        }

    } else if (isSearching and usingTrie) {
        // create Trie object
        Trie booksTest;

        booksTest.parseBookCSV(filePath);

        // check if prefix exist (bool)
        bool harryExist = booksTest.isPrefixExist(inputFiled);
        cout << "harry prefix exists: " << harryExist << endl << endl;

        if (harryExist) {
            cout << "PRINTING BOOKS" << endl;
            vector<Book*> b = booksTest.prefixSearch(inputFiled);

            for (auto *book : b)
                book->print();
        } else {
            cout << "NO BOOKS WITH PREFIX: " << inputFiled << endl;
        }

    }

//    // create Trie object
//    Trie booksTest;
//
//    // parse txt file by Book Title (tab delimiter)
//    // Could implement parsing by other attributes ex. author or id
//    string filePath = "GoogleBookAPIDataset.txt";
//
//    booksTest.parseBookCSV(filePath);
//
//    // check if prefix exist (bool)
//    string pre = "harry";
//    bool harryExist = booksTest.isPrefixExist(pre);
//    cout << "harry prefix exists: " << harryExist << endl << endl;
//
//    if (harryExist) {
//        cout << "PRINTING BOOKS" << endl;
//        vector<Book*> b = booksTest.prefixSearch(pre);
//
//        for (auto *book : b)
//            book->print();
//    } else {
//        cout << "NO BOOKS WITH PREFIX: " << pre << endl;
//    }

    return 0;
}
