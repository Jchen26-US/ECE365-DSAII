#include "hash.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cctype>


bool isValidChar(char c){
    return (
        ('A' <= c  && 'Z' >= c) ||
        ('a' <= c && 'z' >= c) ||
        ('0' <= c && '9' >= c)||
        c == '-' ||
        c == '\'' 
    );
}

bool containsDigit(std::string s){
    for(char c : s){
        if('0' <= c && '9' >= c){
            return true;
        }
    }
    return false;
}




bool loadDictionary(const std::string &inputDictPath, hashTable &table){ //returns true if dictionary opens else returns false
    //std::cout << "loading dict";
    
    std::ifstream inputFile;
    inputFile.open(inputDictPath);
    if(!inputFile.is_open()){
        std::cerr << "failed to open input dictionary\n";
        return false;
    }
    std::string cur_word;
    while(inputFile >> cur_word){
        for(char&c : cur_word){ //lower cases the word
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        int r = table.insert(cur_word);
        if(r == 2){
            std::cerr << "failed to rehash\n";
            return false;
        }
    }
    inputFile.close();
    return true;
} //LoadDict is working 10:41 9/21 spellCheckNotwriting to output

bool spellCheckDocument(const std::string &inputFilePath, const std::string &outputFilePath, hashTable &table){
    std::ifstream inputFile;
    std::ofstream outputFile;
    inputFile.open(inputFilePath);
    outputFile.open(outputFilePath);
    if(!inputFile.is_open()){
        std::cerr << "error opening input file";
        return false;
    }
    if(!outputFile.is_open()){
        std::cerr << "error opening output file";
        inputFile.close();
        return false;
    }
    int lineNum = 0;
    std::string line;
    while(std::getline(inputFile, line)){
        lineNum++;
        std::string cur_word = "";
        for (size_t i = 0; i <= line.length(); ++i) {
            char c = (i < line.length()) ? line[i] : ' ';
            if (isValidChar(c)) { //convert char to lower case
                cur_word += std::tolower(static_cast<unsigned char>(c));
            } 
            else {
                if (!cur_word.empty()) {
                    if (cur_word.length() > 20) {
                        outputFile << "Long word at line " << lineNum  << ", starts: " << cur_word.substr(0, 20) << "\n";
                    } 
                    else if (!containsDigit(cur_word)) { //look at words that don't have digits
                        if (!table.contains(cur_word)) {
                            outputFile << "Unknown word at line " << lineNum << ": " << cur_word << "\n";
                        }
                }
                cur_word = ""; // Clear buffer for next word
                }
            }
        }
    }
    inputFile.close();
    outputFile.close();
    return true;
}

int main() {
    std::string dictPath, inputPath, outputPath;

    std::cout << "Enter name of dictionary: ";
    std::cin >> dictPath;

    hashTable dictionary(50000); // Initial capacity estimate

    // Time dictionary loading
    clock_t startDict = clock();
    if (!loadDictionary(dictPath, dictionary)) {
        std::cerr << "Error opening dictionary file.\n";
        return 1;
    }
    clock_t endDict = clock();
    double dictTime = double(endDict - startDict) / CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to load dictionary: " << dictTime << "\n";

    std::cout << "Enter name of input file: ";
    std::cin >> inputPath;
    std::cout << "Enter name of output file: ";
    std::cin >> outputPath;

    // Time document spell checking
    clock_t startCheck = clock();
    spellCheckDocument(inputPath, outputPath, dictionary);
    clock_t endCheck = clock();
    double checkTime = double(endCheck - startCheck) / CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to check document: " << checkTime << "\n";

    return 0;
}

