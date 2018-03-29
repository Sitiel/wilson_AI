//
//  Environnement.hpp
//  tabou
//
//  Created by Valérian Acier on 27/03/2018.
//  Copyright © 2018 Valérian Acier. All rights reserved.
//

#ifndef Environnement_hpp
#define Environnement_hpp

#define NAME 0
#define DELAY_LIVRAISON 1
#define STOCK 2
#define INITIAL_DEMANDE 3
#define INCREASE 4
#define VARIATION 5
#define MONTH_SAISONALITY 6
#define PERCENT_OF_SAISONALITY 7
#define OWNERSHIP_RATE 8
#define OUT_STOCK_PERCENT 9
#define ORDER_COST 10
#define PURCHASE_PRICE 11
#define VIRTUAL_STOCK 12


#include "../lib/variable.hpp"
#include <iostream>
#include <vector>
#include <map>

class Evaluateur {
public:
    Evaluateur(std::vector<double> &constants);
    
    double evaluate(std::vector<Variable*>& variables);
    double evaluate(std::vector<Variable>& variables);

    double evaluatePointControle(std::vector<Variable>& variables, std::vector<double> state, int day);
    
    
private:
    std::vector<double> constants;
    std::vector<double> state;
    std::map<int, int> dayToMonth;
    double calculOrder(int day);
    void initDayToMonth();
};

#endif /* Evaluateur */














