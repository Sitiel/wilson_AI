//
//  Environnement.cpp
//  tabou
//
//  Created by Valérian Acier on 27/03/2018.
//  Copyright © 2018 Valérian Acier. All rights reserved.
//

#include "evaluateur.hpp"
#include <cmath>

using namespace std;

Evaluateur::Evaluateur(vector<vector<double>> &constants){
    this->constants = constants;
}

double Evaluateur::evaluate(std::vector<Variable*>& variables) {
    double totalDelta = 0;
    
    for(unsigned int i = 0 ; i < constants.size() ; i++)
    {
        double z = constants[i][5];
        
        
        double a = variables[0]->value * pow(constants[i][0], variables[5]->value);
        double b = variables[1]->value * pow(constants[i][1], variables[6]->value);
        double c = variables[2]->value * pow(constants[i][2], variables[7]->value);
        double d = variables[3]->value * pow(constants[i][3], variables[8]->value);
        double e = variables[4]->value * pow(constants[i][4], variables[9]->value);
        
        double z2 = a + b + c + d + e;
        
        totalDelta += abs(z-z2);
        
    }
    
    
    return totalDelta;
}


double Evaluateur::evaluate(vector<Variable>& variables){
    double totalDelta = 0;
    
    for(unsigned int i = 0 ; i < constants.size() ; i++)
    {
        double z = constants[i][5];
        
        
        double a = variables[0].value * pow(constants[i][0], variables[5].value);
        double b = variables[1].value * pow(constants[i][1], variables[6].value);
        double c = variables[2].value * pow(constants[i][2], variables[7].value);
        double d = variables[3].value * pow(constants[i][3], variables[8].value);
        double e = variables[4].value * pow(constants[i][4], variables[9].value);
        
        double z2 = a + b + c + d + e;
        
        totalDelta += abs(z-z2);
        
    }
    
    
    return totalDelta;
}

