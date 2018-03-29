//
//  variables.cpp
//  tabou
//
//  Created by Valérian Acier on 27/03/2018.
//  Copyright © 2018 Valérian Acier. All rights reserved.
//

#include "variable.hpp"
#include <stdlib.h>
#include <time.h>
#include <random>
#include <iostream>

using namespace std;

Variable::Variable(double lowerBound, double upperBound, double value) {
    this->lowerBound = lowerBound;
    this->upperBound = upperBound;
    this->value = value;
    this->entier = true;
    if(value == -1)
        randomise();
}

void Variable::randomise() {
    previous = this->value;
    if(this->entier){
        this->value = ((int)(rand())/ RAND_MAX ) * (this->upperBound - lowerBound) + lowerBound;
    }else{
        this->value = ((double)(rand())/ RAND_MAX ) * (this->upperBound - lowerBound) + lowerBound;
    }
}

void Variable::revert(){
    this->value = previous;
}

void Variable::gaussianRandomise(double k) {
    previous = this->value;
    std::default_random_engine generator;
    if(this->entier){
        std::normal_distribution<double> distribution(this->value,k);
        int generated = -1;
        do{
            generated = distribution(generator);
        }while(generated < lowerBound || generated > upperBound);

        this->value = generated;
    }else{
        std::normal_distribution<double> distribution(this->value,k);
        double generated = -1;
        do{
            generated = distribution(generator);
        }while(generated < lowerBound || generated > upperBound);

        this->value = generated;
    }

}

double Variable::compare(Variable &b) { 
    return  (abs(value - b.value) - lowerBound) / (upperBound - lowerBound);
}

double Variable::compare(Variable *b) { 
    return  (abs(value - b->value) - lowerBound) / (upperBound - lowerBound);
}

void Variable::removeEntierConstraint(){
    this->entier = false;
}

