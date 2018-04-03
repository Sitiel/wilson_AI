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
#include <sys/time.h>


using namespace std;


void CSVReader::read(std::vector<std::vector<double>> &content) {
    string line;
    ifstream csv(filename);
    
    if(!csv.good()){
        this->errorOpeningFile();
    }
    
    getline(csv, line);
    while (getline(csv, line))
    {
        istringstream iss(line);
        string value;
        vector<double> line;
        getline(iss, value, ',');
        while (getline(iss, value, ',')){
            line.push_back(stod(value));
        }
        content.push_back(line);
    }
    csv.close();
}

void CSVReader::read(std::vector<pair<std::vector<Variable>, string>> &content) {
    string line;
    ifstream csv(filename);
    
    if(!csv.good()){
        this->errorOpeningFile();
    }
    
    getline(csv, line);
    while (getline(csv, line))
    {
        istringstream iss(line);
        string value;
        vector<Variable> line;
        getline(iss, value, ';');
        string type = "merge";
        while (getline(iss, value, ';')){
            if(value.empty() || value.find("merge") != string::npos){
                continue;
            }
            if(value.find("tabou") != string::npos){
                type = value;
                continue;
            }
            if(value.find("recuit") != string::npos || value.find("Recuit") != string::npos){
                type = value;
                continue;
            }
            if(value == "Replenishment"){
                line.push_back(Variable(1));
            }
            else if(value == "OrderPoint"){
                line.push_back(Variable(0));
            }
            else{
                line.push_back(Variable(stod(value)));
            }
            
        }
        content.push_back(make_pair(line, type));
    }
    csv.close();
}


void CSVReader::read(std::vector<std::vector<Variable>> &content) {
    string line;
    ifstream csv(filename);
    
    if(!csv.good()){
        this->errorOpeningFile();
    }
    
    getline(csv, line);
    while (getline(csv, line))
    {
        istringstream iss(line);
        string value;
        vector<Variable> line;
        getline(iss, value, ';');
        string type = "merge";
        while (getline(iss, value, ';')){
            if(value.empty() || value.find("merge") != string::npos){
                continue;
            }
            if(value.find("tabou") != string::npos){
                type = value;
                continue;
            }
            if(value.find("recuit") != string::npos || value.find("Recuit") != string::npos){
                type = value;
                continue;
            }
            if(value == "Replenishment"){
                line.push_back(Variable(1));
            }
            else if(value == "OrderPoint"){
                line.push_back(Variable(0));
            }
            else{
                line.push_back(Variable(stod(value)));
            }
            
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
    
    getline(csv, line);
    while (getline(csv, line))
    {
        istringstream iss(line);
        string value;
        vector<int> line;
        getline(iss, value, ';');
        while (getline(iss, value, ';')){
            line.push_back(stoi(value));
        }
        content.push_back(line);
    }
    
    csv.close();
}

//void CSVReader::write(vector<vector<string>> &content, string name){
//    ofstream csv;
//    if(name.empty()){
//        ofstream csv(name);
//    }else{
//        struct timeval tv;
//        gettimeofday(&tv, NULL);
//        int sec = (int) tv.tv_sec;
//        int usec = (int) tv.tv_usec % 0x100000;
//        char newname[255];
//        sprintf(newname,"%08x%05x", sec, usec);
//        ofstream csv(newname);
//    }

//    for(unsigned int i = 0; i <content.size();i++){
//        for(unsigned int j = 0; j<content[i].size();j++){
//            csv << content[i][j];
//            if(j != (content[i].size())-1){
//                csv << ";";
//            }
//        }
//        csv << endl;
//    }

//    csv.flush();
//    csv.close();
//}

void CSVReader::write(std::vector<std::pair<std::vector<Variable>, std::string>> &content, std::string name){
    ofstream csv;
    if(!name.empty()){
        csv.open(name,ofstream::app);
    }else{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        int sec = (int) tv.tv_sec;
        int usec = (int) tv.tv_usec % 0x100000;
        char newname[255];
        sprintf(newname,"%08x%05x.csv", sec, usec);
        csv.open(newname,ofstream::app);
    }
    
    csv << "Name;Method;Quantité Maximum;Pt Commande/Période;1er Commande;Metaheuristique\n";
    csv.flush();
    for(unsigned int i = 0; i <content.size();i++){
        if(i+1<10){
            csv << "A0"<< i+1 << ";";
        }else{
            csv << "A" << i+1 << ";";
        }
        csv.flush();
        for(unsigned int j = 0; j<content[i].first.size();j++){
            if(j == 0){
                if(content[i].first[j].value == 0){
                    csv << "OrderPoint";
                }else{
                    csv << "Replenishment";
                }
            }else{
                csv << content[i].first[j].value;
            }
            csv << ";";
            csv.flush();
        }
        if(content[i].first.size() != 4)
            csv << ";";
        csv << content[i].second << endl;
        csv.flush();
    }
    
    csv.close();
}

void CSVReader::write(vector<vector<Variable>> &content,string meta, string name){
    ofstream csv;
    if(!name.empty()){
        csv.open(name,ofstream::app);
    }else{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        int sec = (int) tv.tv_sec;
        int usec = (int) tv.tv_usec % 0x100000;
        char newname[255];
        sprintf(newname,"%08x%05x.csv", sec, usec);
        csv.open(newname,ofstream::app);
    }

    csv << "Name;Method;Quantité Maximum;Pt Commande/Période;1er Commande;Metaheuristique\n";
    csv.flush();
    for(unsigned int i = 0; i <content.size();i++){
        if(i+1<10){
            csv << "A0"<< i+1 << ";";
        }else{
            csv << "A" << i+1 << ";";
        }
        csv.flush();
        for(unsigned int j = 0; j<content[i].size();j++){
            if(j == 0){
                if(content[i][j].value == 0){
                    csv << "OrderPoint";
                }else{
                    csv << "Replenishment";
                }
            }else{
                csv << content[i][j].value;
            }
            csv << ";";
            csv.flush();
        }
        if(content[i].size() != 4)
            csv << ";";
        csv << meta << endl;
        csv.flush();
    }

    csv.close();

}

CSVReader::CSVReader(string filename) {
    this->filename = filename;
}

void CSVReader::errorOpeningFile() { 
    cout << "Le fichier " << filename << " est introuvable." << endl;
    exit(0);
}


