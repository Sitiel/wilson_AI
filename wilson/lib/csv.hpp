//
//  csv.hpp
//  tabou
//
//  Created by Valérian Acier on 27/03/2018.
//  Copyright © 2018 Valérian Acier. All rights reserved.
//

#ifndef csv_hpp
#define csv_hpp

#include <iostream>
#include <vector>
#include "variable.hpp"

class CSVReader{
public:
    CSVReader(std::string filename = "");
    void read(std::vector<std::vector<double>> &content);
    void read(std::vector<std::vector<std::string>> &content);
    void read(std::vector<std::vector<int>> &content);
//    void write(std::vector<std::vector<double>> &content, std::string name = NULL);
    //void write(std::vector<std::vector<std::string>> &content, std::string name = "");
    void write(std::vector<std::vector<Variable>> &content, std::string meta,std::string name = "");
//    void write(std::vector<std::vector<int>> &content);

    void errorOpeningFile();
private:
    std::string filename;
};

#endif /* csv_hpp */
