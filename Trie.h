#include <iostream>
#include <vector>

using namespace std;
#define ALPHABET_SIZE 26

struct Book {
    string title;
    string description;
    string authors;
    string id, categories, publishDate;

    int pageCount;
    float avgRating;
    bool isMature;

    void print() {
        cout << "Title: " << title << endl;
        cout << "Description: " << description << endl;
        cout << "Authors: " << authors << endl;
        cout << "Pages: " << pageCount << endl;
        cout << "ID: " << id << endl;
        cout << "Categories: " << categories << endl;
        cout << "Published Date: " << publishDate << endl;
        cout << "AvgRating: " << avgRating << endl;
        cout << "isMature: " << isMature << endl;
        cout << endl;
    }
};

struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];

    int wordCount = 0;

    vector<Book*> books;

    TrieNode() {
        wordCount = 0;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

class Trie {
public:
    TrieNode* root = new TrieNode();

    void insert(string& key, Book *book);   // insert key
    bool isPrefixExist(string& key);    // check if prefix is there
    bool search_key(string& key);   // search is work is actually there
    vector<Book*> prefixSearch(string& key);

    void parseBookCSV(string& filePath);   // iterates through CSV
    void print();
    Trie() {}
};

