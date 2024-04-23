#include <SFML/Graphics.hpp>
#include <chrono>
#include "TextureManager.h"
#include "HashTable.h"
#include "Trie.h"
using namespace std;

void setText(sf::Text &text, float x, float y, const string& anchorPoint = "middle") {
    sf::FloatRect textRect = text.getLocalBounds();
    if (anchorPoint == "middle") {
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
    } else if (anchorPoint == "northwest") {
        text.setOrigin(textRect.left,
                       textRect.top);
    }
    text.setPosition(sf::Vector2f(x, y));
}

void removeExtraChars(string& input) {
    string result;
    for (char c : input) {
        if (c != '"' && c != '\'' && c != '[' && c != ']') {
            result += c;
        }
    }
    input = result;
}

string getBookInfo(const Book& book) {
    string str;
    str += "Title: " + book.title + "\n";
    auto authors_str = book.authors;
    removeExtraChars(authors_str);
    str += "Author: " + authors_str + "\n";
    str += "Description: " + book.description + "\n";
    str += "Page Count: " + to_string(book.pageCount) + "\n";
    str += "Publish Date: " + book.publishDate + "\n";
    auto categories_str = book.categories;
    removeExtraChars(categories_str);
    str += "Categories: " + categories_str + "\n";
    str += "\n\n"; // newline at end
    return str;
}

void formatResultsStr(string& input) {
    string result;
    int charCount = 0;
    for (char c : input) {
        if (c == '\n') {
            result += c;
            charCount = 0; // Reset character count for new line
        } else {
            result += c;
            charCount++;

            if (charCount > 80) {
                string extra;
                while (result.back() != ' ') {
                    extra.insert(extra.begin(), result.back());
                    result.pop_back();
                }
                result += "\n\t" + extra;
                charCount = 0; // Reset character count after inserting newline and tab
            }
        }
    }
    input = result;
}

void displayResults(string userInput, bool usingHash, bool searchTitle) {
    sf::Font font;
    font.loadFromFile("files/fonts/font.ttf");

    sf::Font oswaldFont;
    oswaldFont.loadFromFile("files/fonts/oswald/Oswald-ExtraLight.ttf");

    string resultsStr;
    string userInputOG = userInput;

    size_t parsingDuration;
    size_t retrieveDuration;
    int resultCnt = 0;

    string filepath = "files/GoogleBookAPIDataset.txt";
    if (usingHash) {
        // Create a hash table
        HashTable ht;

        // Parse the book CSV file and insert into the hash table
        ht.parseCSVHash(filepath, searchTitle);

        vector<Book*> b = ht.retrieve(userInput);

        for (auto book : b) {
            resultsStr += getBookInfo(*book);
            resultCnt++;
        }

        parsingDuration = ht.getParsingDuration();
        retrieveDuration = ht.getRetrieveDuration();

    } else {
        // create Trie object
        Trie booksTest;

        booksTest.parseBookCSV(filepath, searchTitle);

        // check if prefix exist (bool)
        bool inputExists = booksTest.isPrefixExist(userInput);

        if (inputExists) {
            vector<Book*> b = booksTest.prefixSearch(userInput);

            int bookCnt = 1;
            for (auto book : b) {
                //book->print();
                resultsStr += "Book #" + to_string(bookCnt) + "\n";
                bookCnt++;
                resultsStr += getBookInfo(*book);

                resultCnt++;
            }
        }

        parsingDuration = booksTest.getParsingDuration();
        retrieveDuration = booksTest.getRetrieveDuration();
    }

    int width = 540;
    int height = 610;
    int minorVertSep = 24; // Vertical separation between text
    int majorVertSep = 42; // Vertical separation between text

    sf::RenderWindow resultsWindow(sf::VideoMode(width, height), "Results", sf::Style::Close);
    resultsWindow.setVerticalSyncEnabled(true); // call it once, after creating the window

    sf::Sprite background;
    background.setTexture(TextureManager::getTexture("results_background1"));
    background.setPosition(0,0);
    resultsWindow.draw(background);

    // Data Structure Text
    string dataStruct;
    if (usingHash) {
        dataStruct = "Hash Table";
    } else {
        dataStruct = "Trie";
    }
    // Data Structure Used Text
    sf::Text dataStructUsedText("Data Structure Used: " + dataStruct, font, 16);
    dataStructUsedText.setFillColor(sf::Color::Black);
    dataStructUsedText.setOutlineColor(sf::Color::White);
    dataStructUsedText.setOutlineThickness(2);
    dataStructUsedText.setStyle(sf::Text::Bold);
    setText(dataStructUsedText, width * 0.08, height * 0.04, "northwest");

    // Parsing Duration Text
    sf::Text parseText("Parsing Duration: " + to_string(parsingDuration) + " milliseconds", font, 16);
    parseText.setFillColor(sf::Color::Black);
    parseText.setOutlineColor(sf::Color::White);
    parseText.setOutlineThickness(2);
    parseText.setStyle(sf::Text::Bold);
    setText(parseText, width * 0.08, dataStructUsedText.getGlobalBounds().top + minorVertSep, "northwest");

    // Retrieve Duration Text
    sf::Text retrieveText("Retrieval Duration: " + to_string(retrieveDuration) + " microseconds", font, 16);
    retrieveText.setFillColor(sf::Color::Black);
    retrieveText.setOutlineColor(sf::Color::White);
    retrieveText.setOutlineThickness(2);
    retrieveText.setStyle(sf::Text::Bold);
    setText(retrieveText, width * 0.08, parseText.getGlobalBounds().top + minorVertSep, "northwest");

    /*
    // Search Field Text
    string searchField;
    if (searchTitle) {
        searchField = "Title";
    } else {
        searchField = "Author";
    }
    sf::Text searchFieldText("Search Field: " + searchField, font, 16);
    searchFieldText.setFillColor(sf::Color::Black);
    searchFieldText.setStyle(sf::Text::Bold);
    setText(searchFieldText, width * 0.08, height * 0.17, "northwest");

    sf::Text searchInputText("Search Input: " + userInputOG, font, 16);
    searchInputText.setFillColor(sf::Color::Black);
    searchInputText.setStyle(sf::Text::Bold);
    setText(searchInputText, width * 0.08, height * 0.20, "northwest");
     */

    sf::Text resultsCountText("Number of Results: " + to_string(resultCnt), font, 16);
    resultsCountText.setFillColor(sf::Color::Black);
    resultsCountText.setOutlineColor(sf::Color::White);
    resultsCountText.setOutlineThickness(2);
    resultsCountText.setStyle(sf::Text::Bold);
    setText(resultsCountText, width * 0.08, retrieveText.getGlobalBounds().top + minorVertSep, "northwest");

    // Results Header Text
    sf::Text headerText("Search Results", font, 24);
    headerText.setFillColor(sf::Color::Black);
    headerText.setOutlineColor(sf::Color::White);
    headerText.setOutlineThickness(4);
    headerText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(headerText, width / 2, resultsCountText.getGlobalBounds().top + majorVertSep);

    // Results Text
    if (resultsStr.empty()) {
        resultsStr = "No Books Found";
    } else {
        formatResultsStr(resultsStr);
    }
    int resultHeight = 170;
    sf::Text resultsText(resultsStr, oswaldFont, 12);
    resultsText.setFillColor(sf::Color::Black);
    resultsText.setOutlineColor(sf::Color::White);
    resultsText.setOutlineThickness(1);
    resultsText.setStyle(sf::Text::Bold);
    setText(resultsText, width * 0.08, resultHeight, "northwest");

    sf::Sprite rect;
    rect.setTexture(TextureManager::getTexture("results_background1_top"));
    rect.setPosition(0, 0);
    resultsWindow.draw(rect);

    while (resultsWindow.isOpen()) {
        resultsWindow.clear(sf::Color::White);
        resultsWindow.draw(background);

        sf::Event event;
        while (resultsWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                resultsWindow.close();
            }
            if (event.type == sf::Event::MouseWheelMoved)
            {
                if (resultsText.getGlobalBounds().contains(event.mouseWheel.x, event.mouseWheel.y)) {
                    setText(resultsText, resultsText.getGlobalBounds().left, resultsText.getGlobalBounds().top + (event.mouseWheel.delta), "northwest");
                    if (resultsText.getGlobalBounds().top + resultsText.getGlobalBounds().height < height * 0.9) {
                        setText(resultsText, resultsText.getGlobalBounds().left, (height * 0.9) - resultsText.getGlobalBounds().height, "northwest");
                    }
                    if (resultsText.getGlobalBounds().top > resultHeight) {
                        setText(resultsText, resultsText.getGlobalBounds().left, resultHeight, "northwest");
                    }
                }
            }
        }

        // Results text displayed first, so it can be hidden by rectangle
        resultsWindow.draw(resultsText);
        resultsWindow.draw(rect);
        resultsWindow.draw(dataStructUsedText);
        resultsWindow.draw(parseText);
        resultsWindow.draw(retrieveText);
        //resultsWindow.draw(searchFieldText);
        //resultsWindow.draw(searchInputText);
        resultsWindow.draw(resultsCountText);
        resultsWindow.draw(headerText);
        resultsWindow.display();
    }
}

int main() {
    int width = 640;
    int height = 480;

    sf::RenderWindow window(sf::VideoMode(width, height), "BOOK SEARCH ENGINE", sf::Style::Close);
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

    sf::Sprite background;
    background.setTexture(TextureManager::getTexture("background1"));
    sf::FloatRect backRect = background.getLocalBounds();
    backRect = background.getGlobalBounds();
    background.setOrigin(backRect.left + backRect.width / 2.0f,
                         backRect.top + backRect.height / 2.0f);
    background.setPosition(sf::Vector2f(width / 2, height / 2));
    window.draw(background);

    sf::Font font;
    font.loadFromFile("files/fonts/font.ttf");

    // Header Text
    sf::Text headerText("BOOK SEARCH ENGINE", font, 32);
    headerText.setFillColor(sf::Color::Black);
    headerText.setOutlineColor(sf::Color::White);
    headerText.setOutlineThickness(5);
    headerText.setStyle(sf::Text::Bold);
    setText(headerText, width / 2, height * 0.15);

    // Enter Input Text
    sf::Text enterInputText("Enter Input:", font, 20);
    enterInputText.setFillColor(sf::Color::White);
    enterInputText.setOutlineColor(sf::Color::Black);
    enterInputText.setOutlineThickness(5);
    enterInputText.setStyle(sf::Text::Bold);
    setText(enterInputText, width / 2, height * 0.3);

    // Input text
    sf::Text userInputText("", font, 18);
    userInputText.setFillColor(sf::Color::White);
    userInputText.setOutlineColor(sf::Color::Black);
    userInputText.setOutlineThickness(5);
    userInputText.setStyle(sf::Text::Bold);

    // Data Structure Text
    sf::Text dataStructText("Data Structure", font, 20);
    dataStructText.setFillColor(sf::Color::White);
    dataStructText.setOutlineColor(sf::Color::Black);
    dataStructText.setOutlineThickness(5);
    dataStructText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(dataStructText, width * 0.25, height * 0.5);

    // Hash Table Text
    sf::Text hashText("Hash Table", font, 20);
    hashText.setFillColor(sf::Color::White);
    hashText.setOutlineColor(sf::Color::Black);
    hashText.setOutlineThickness(5);
    hashText.setStyle(sf::Text::Bold);
    setText(hashText, width * 0.18, height * 0.57, "northwest");

    // Trie Text
    sf::Text trieText("Trie", font, 20);
    trieText.setFillColor(sf::Color::White);
    trieText.setOutlineColor(sf::Color::Black);
    trieText.setOutlineThickness(5);
    trieText.setStyle(sf::Text::Bold);
    setText(trieText, width * 0.18, height * 0.67, "northwest");

    // Search Field Text
    sf::Text searchFieldText("Search Field", font, 20);
    searchFieldText.setFillColor(sf::Color::White);
    searchFieldText.setOutlineColor(sf::Color::Black);
    searchFieldText.setOutlineThickness(5);
    searchFieldText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(searchFieldText, width * 0.75, height * 0.5);

    // Title Text
    sf::Text titleText("Title", font, 20);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(5);
    titleText.setStyle(sf::Text::Bold);
    setText(titleText, width * 0.7, height * 0.57, "northwest");

    // Author Text
    sf::Text authorText("Author", font, 20);
    authorText.setFillColor(sf::Color::White);
    authorText.setOutlineColor(sf::Color::Black);
    authorText.setOutlineThickness(5);
    authorText.setStyle(sf::Text::Bold);
    setText(authorText, width * 0.7, height * 0.67, "northwest");

    std::string userInput;

    auto startTime = chrono::high_resolution_clock::now();
    auto pauseTime = chrono::high_resolution_clock::now();
    int elapsedTime = 0;

    // Choose data structure
    bool usingHash = false;
    bool usingTrie = true;

    // Choose Search Field
    bool searchTitle = true;
    bool searchAuthor = false;

    sf::Sprite hashButton;
    sf::Sprite trieButton;
    sf::Sprite titleButton;
    sf::Sprite authorButton;
    sf::Sprite amanButton;
    int amanRotVal = 0;

    sf::Sprite stickManButton;
    stickManButton.setTexture(TextureManager::getTexture("stick_man"));
    stickManButton.setPosition(width * 0.438, height * 0.8);

    while (window.isOpen()) {
        window.clear(sf::Color::White);
        window.draw(background);

        string hashButtonTexture;
        string trieButtonTexture;
        if (usingHash) {
            hashButtonTexture = "pressed";
            trieButtonTexture = "unpressed";
        } else {
            hashButtonTexture = "unpressed";
            trieButtonTexture = "pressed";
        }

        hashButton.setTexture(TextureManager::getTexture(hashButtonTexture));
        hashButton.setPosition(width*0.1, height*0.56);
        window.draw(hashButton);

        trieButton.setTexture(TextureManager::getTexture(trieButtonTexture));
        trieButton.setPosition(width*0.1, height*0.66);
        window.draw(trieButton);

        string titleButtonTexture;
        string authorButtonTexture;
        if (searchTitle) {
            titleButtonTexture = "pressed";
            authorButtonTexture = "unpressed";
        } else {
            titleButtonTexture = "unpressed";
            authorButtonTexture = "pressed";
        }

        titleButton.setTexture(TextureManager::getTexture(titleButtonTexture));
        titleButton.setPosition(width*0.62, height*0.56);
        window.draw(titleButton);

        authorButton.setTexture(TextureManager::getTexture(authorButtonTexture));
        authorButton.setPosition(width*0.62, height*0.66);
        window.draw(authorButton);

        amanButton.setTexture(TextureManager::getTexture("aman"));
        sf::FloatRect amanRect = amanButton.getLocalBounds();
        amanButton.setOrigin(amanRect.left + amanRect.width / 2.0f,
                             amanRect.top + amanRect.height / 2.0f);
        amanButton.setPosition(width / 2, height*0.8);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            if (event.type == sf::Event::TextEntered) {
                char typedChar = static_cast<char>(event.text.unicode);
                if (typedChar == 8) {  // Backspace key
                    // Remove the last character
                    if (!userInput.empty()) {
                        userInput.pop_back();
                    }
                } else {
                    userInput += typedChar;
                }
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                // Check if at least one character has been input
                if (!userInput.empty()) {
                    // Display Results Window
                    displayResults(userInput, usingHash, searchTitle);
                    // Reset userInput to empty string
                    userInput = "";
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (hashButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        usingHash = true;
                        usingTrie = false;
                    } else if (trieButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        usingHash = false;
                        usingTrie = true;
                    } else if (titleButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        searchTitle = true;
                        searchAuthor = false;
                    } else if (authorButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        searchTitle = false;
                        searchAuthor = true;
                    } else if (amanButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        amanRotVal = 360;
                    }
                }
            }
        }
        window.draw(headerText); // inside the main loop, between window.clear() and window.display()
        window.draw(enterInputText);
        setText(userInputText, width / 2, height * 0.38);
        userInputText.setString(userInput + "|");\
        window.draw(userInputText);
        window.draw(dataStructText);
        window.draw(hashText);
        window.draw(trieText);
        window.draw(searchFieldText);
        window.draw(titleText);
        window.draw(authorText);
        window.draw(stickManButton);
        window.draw(amanButton);
        if (amanRotVal > 0) {
            amanRotVal -= 5;
            amanButton.setRotation(amanRotVal);
        }
        window.display();
    }
}