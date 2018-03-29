//
//  Environnement.hpp
//  tabou
//
//  Created by Valérian Acier on 27/03/2018.
//  Copyright © 2018 Valérian Acier. All rights reserved.
//

#ifndef Environnement_hpp
#define Environnement_hpp

#include "../lib/variable.hpp"
#include <iostream>
#include <vector>

class Evaluateur {
public:
    Evaluateur(std::vector<std::vector<double>> &constants);
    
    double evaluate(std::vector<Variable*>& variables);
    double evaluate(std::vector<Variable>& variables);
    
private:
    std::vector<std::vector<double>> constants;
};

#endif /* Evaluateur */
