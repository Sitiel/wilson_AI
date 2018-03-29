//
//  variables.hpp
//  tabou
//
//  Created by Valérian Acier on 27/03/2018.
//  Copyright © 2018 Valérian Acier. All rights reserved.
//

#ifndef variable_hpp
#define variable_hpp

class Variable{
public:
    Variable(double lowerBound, double upperBound, double value = -1);
    void randomise();
    void gaussianRandomise(double k);
    void revert();
    void removeEntierConstraint();
    double compare(Variable &b);
    double compare(Variable *b);
    double value;
    double lowerBound, upperBound;
    
private:
    double previous;
    bool entier;
};


#endif /* variables_hpp */
