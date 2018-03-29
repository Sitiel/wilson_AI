//
//  csv.cpp
//  tabou
//
//  Created by Valérian Acier on 27/03/2018.
//  Copyright © 2018 Valérian Acier. All rights reserved.
//

#include "csv.hpp"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>


using namespace std;


void CSVReader::read(std::vector<std::vector<double>> &content) {
    string line;
    ifstream csv(filename);
    
    if(!csv.good()){
        this->errorOpeningFile();
    }
    
    while (getline(csv, line))
    {
        istringstream iss(line);
        string value;
        vector<double> line;
        while (getline(iss, value, ';')){
            line.push_back(stod(value));
        }
        content.push_back(line);
    }
    csv.close();
}

void CSVReader::read(std::vector<std::vector<std::string>> &content) {
    string line;
    ifstream csv(filename);
    
    if(!csv.good()){
        this->errorOpeningFile();
    }
    
    
    while (getline(csv, line))
    {
        istringstream iss(line);
        string value;
        vector<string> line;
        while (getline(iss, value, ';')){
            line.push_back(value);
        }
        content.push_back(line);
    }
    csv.close();
}

void CSVReader::read(std::vector<std::vector<int>> &content) {
    string line;
    ifstream csv(filename);
    
    if(!csv.good()){
        this->errorOpeningFile();
    }
    
    
    while (getline(csv, line))
    {
        istringstream iss(line);
        string value;
        vector<int> line;
        while (getline(iss, value, ';')){
            line.push_back(stoi(value));
        }
        content.push_back(line);
    }
    
    csv.close();
}


CSVReader::CSVReader(char *filename) { 
    this->filename = filename;
}

void CSVReader::errorOpeningFile() { 
    cout << "Le fichier " << filename << " est introuvable." << endl;
    exit(0);
}


