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


class CSVReader{
public:
    CSVReader(char *filename);
    void read(std::vector<std::vector<double>> &content);
    void read(std::vector<std::vector<std::string>> &content);
    void read(std::vector<std::vector<int>> &content);
    void errorOpeningFile();
private:
    char* filename;
};

#endif /* csv_hpp */
