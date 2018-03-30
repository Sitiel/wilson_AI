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

Evaluateur::Evaluateur(vector<double> &constants)
{
    this->constants = constants;
    riskMode = RISK_SAFE;
    initDayToMonth();
}

double Evaluateur::evaluate(std::vector<Variable *> &variables)
{
    double totalDelta = 0;

    return totalDelta;
}

double Evaluateur::evaluate(vector<Variable> &variables)
{
    double totalCost = 0;
    map<int, double> livraison;
    this->state = this->constants;
    this->state.push_back(state[STOCK]);

    for (int i = 1; i < 262; i++)
    {
        
        /*if(livraison[i] > 0){
            cout << "Livraison jour " << i << endl;
        }*/
        this->state[STOCK] = (int)(state[STOCK] - calculOrder(i) + livraison[i]);
        //cout << "Stock jour " << i << " : " << state[STOCK] << endl;
        
        this->state[VIRTUAL_STOCK] = (int)(state[VIRTUAL_STOCK] - calculOrder(i));
        if (this->state[STOCK] < 0)
        {
            // Cas Rupture
            totalCost -= state[STOCK] * constants[PURCHASE_PRICE] * constants[OUT_STOCK_PERCENT];
        }
        else
        {
            if (i % 5 == 0)
            {
                // Cas stockage Vendredi
                totalCost += (state[STOCK] * constants[PURCHASE_PRICE] * constants[OWNERSHIP_RATE] * 3) / 365;
            }
            else
            {
                // Cas stockage Semaine
                totalCost += (state[STOCK] * constants[PURCHASE_PRICE] * constants[OWNERSHIP_RATE]) / 365;
            }
        }
        
        double delivery = variables[0].value == 0 ? evaluatePointControle(variables, state) : evaluateRecomplete(variables, state, i);
        
        livraison[i + constants[DELAY_LIVRAISON]] = delivery;
        state[VIRTUAL_STOCK] += delivery;
        if (delivery > 0){
            //cout << "Commande jour " << i << " de " << delivery << endl;
            totalCost += constants[ORDER_COST];
        }
    }

    return totalCost;
}

void Evaluateur::setRisky(){
    this->riskMode = RISK_RISKY;
}
void Evaluateur::setSafe(){
    this->riskMode = RISK_SAFE;
}
void Evaluateur::setMedium(){
    this->riskMode = RISK_MEDIUM;
}
void Evaluateur::setRandom(){
    this->riskMode = RISK_RANDOM;
}

double Evaluateur::evaluatePointControle(std::vector<Variable> &variables, std::vector<double> state)
{
    //Variable 0 = Evaluateur
    //Variable 1 = Quantitée à commander
    //Variable 2 = Point de controle
    
    
    if (state[VIRTUAL_STOCK] >= variables[2].value)
        return 0;
    return variables[1].value;
}

double Evaluateur::evaluateRecomplete(std::vector<Variable> &variables, std::vector<double> state, int day)
{
    
    //Variable 0 = Evaluateur
    //Variable 1 = Point de controle
    //Variable 2 = Interval
    //Variable 3 = Jour de début
    
    
    if (state[VIRTUAL_STOCK] >= variables[1].value){
        return 0;
    }
    
    if(day < variables[3].value){
        return 0;
    }
    
    if(((int)(day-variables[3].value) % (int)variables[2].value) != 0){
        return 0;
    }
    
    return variables[1].value - state[VIRTUAL_STOCK];
}

double Evaluateur::variation(){
    if(riskMode == RISK_SAFE){
        return constants[VARIATION]/2;
    }
    if(riskMode == RISK_MEDIUM){
        return 0;
    }
    if(riskMode == RISK_RISKY){
        return -constants[VARIATION]/2;
    }
    return (rand()% (int)(constants[VARIATION]*2)) - constants[VARIATION];
}

double Evaluateur::calculOrder(int day)
{
    double res = (this->constants[INITIAL_DEMANDE] + (day * this->constants[INCREASE])) * (dayToMonth[day] == constants[MONTH_SAISONALITY] ? 1 + constants[PERCENT_OF_SAISONALITY] : 1) + variation();
    return res;
}

void Evaluateur::initDayToMonth() {
    for (int i = 0; i < 24; i++)
    {
        dayToMonth[i] = 1;
    }
    for (int i = 24; i < 44; i++)
    {
        dayToMonth[i] = 2;
    }
    for (int i = 44; i < 66; i++)
    {
        dayToMonth[i] = 3;
    }
    for (int i = 66; i < 87; i++)
    {
        dayToMonth[i] = 4;
    }
    for (int i = 87; i < 110; i++)
    {
        dayToMonth[i] = 5;
    }
    for (int i = 110; i < 131; i++)
    {
        dayToMonth[i] = 6;
    }
    for (int i = 131; i < 153; i++)
    {
        dayToMonth[i] = 7;
    }
    for (int i = 153; i < 176; i++)
    {
        dayToMonth[i] = 8;
    }
    for (int i = 176; i < 196; i++)
    {
        dayToMonth[i] = 9;
    }
    for (int i = 196; i < 219; i++)
    {
        dayToMonth[i] = 10;
    }
    for (int i = 219; i < 241; i++)
    {
        dayToMonth[i] = 11;
    }
    for (int i = 241; i < 262; i++)
    {
        dayToMonth[i] = 12;
    }
}
