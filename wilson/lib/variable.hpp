//
//  variables.hpp
//  tabou
//
//  Created by Valérian Acier on 27/03/2018.
//  Copyright © 2018 Valérian Acier. All rights reserved.
//

#ifndef variable_hpp
#define variable_hpp

#include <random>

class Variable{
public:
    Variable(double lowerBound, double upperBound, double value = -1);
    Variable(double value = -1);
    void randomise();
    void randomise(double modifPercent);
    void rebound(double lowerBound, double upperBound);
    
    void gaussianRandomise(double k);
    void revert();
    void removeEntierConstraint();
    double compare(Variable &b);
    double compare(Variable *b);
    bool isEqual(Variable &b);
    bool isEqual(Variable *b);
    void add(double value);
    double calculDiff();
    double value;
    double lowerBound, upperBound;
    
private:
    double previous;
    bool entier;
    std::default_random_engine generator;
};


#endif /* variables_hpp */
