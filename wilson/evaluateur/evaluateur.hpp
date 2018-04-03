//
//  Environnement.hpp
//  tabou
//
//  Created by Valérian Acier on 27/03/2018.
//  Copyright © 2018 Valérian Acier. All rights reserved.
//

#ifndef Environnement_hpp
#define Environnement_hpp

#define DELAY_LIVRAISON 0
#define STOCK 1
#define INITIAL_DEMANDE 2
#define INCREASE 3
#define VARIATION 4
#define MONTH_SAISONALITY 5
#define PERCENT_OF_SAISONALITY 6
#define OWNERSHIP_RATE 7
#define OUT_STOCK_PERCENT 8
#define ORDER_COST 9
#define PURCHASE_PRICE 10
#define VIRTUAL_STOCK 11


#define RISK_SAFE 0
#define RISK_MEDIUM 1
#define RISK_RISKY 2
#define RISK_RANDOM 3

#include "../lib/variable.hpp"
#include <iostream>
#include <vector>
#include <map>

class Evaluateur {
public:
    Evaluateur(std::vector<double> &constants);
    
    double evaluate(std::vector<Variable*>& variables);
    double evaluate(std::vector<Variable>& variables);
    
    void setRisky();
    void setSafe();
    void setMedium();
    void setRandom();
    void setAverage(int averageModeValue = 100);
    
    double prodAnnee();
    double calcCommandeWilson();
    double calcPeriodiciteWilson();
    double minDayBeforeRupture();
    double variation();

    double evaluatePointControle(std::vector<Variable>& variables, std::vector<double> state);
    
    double evaluateRecomplete(std::vector<Variable> &variables, std::vector<double> state, int day);
    
private:
    std::vector<double> constants;
    std::vector<double> state;
    std::map<int, int> dayToMonth;
    double calculOrder(int day);
    void initDayToMonth();
    int riskMode;
    int averageModeValue;
};

#endif /* Evaluateur */














