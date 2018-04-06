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
    averageModeValue = 1;
}

void Evaluateur::setAverage(int averageModeValue){
    riskMode = RISK_RANDOM;
    this->averageModeValue = averageModeValue;
}

double Evaluateur::evaluate(vector<Variable> &variables)
{
    double retour = 0;
    for(int z = 0 ; z < averageModeValue ; z++){
        double totalCost = 0;
        map<int, double> livraison;
        this->state = this->constants;
        this->state.push_back(state[STOCK]);
        
        for (int i = 1; i < 262; i++)
        {
            
            /*if(livraison[i] > 0){
             cout << "Livraison jour " << i << " de " << livraison[i] << endl;
             }*/
            this->state[STOCK] = (int)(state[STOCK] - calculOrder(i) + livraison[i]);
            //cout << "Stock jour " << i << " : " << state[STOCK] << endl;
            
            this->state[VIRTUAL_STOCK] = (int)(state[VIRTUAL_STOCK] - calculOrder(i));
            if (this->state[STOCK] < 0)
            {
                //cout << "Rupture Jour " << i << " de " << state[STOCK] << endl;
                // Cas Rupture
                totalCost += -state[STOCK] * constants[PURCHASE_PRICE] * constants[OUT_STOCK_PERCENT];
                state[STOCK] = 0;
            }
            else
            {
                if (i % 5 == 0)
                {
                    //cout << "Stock Fin Semaine " << i << " de " << state[STOCK] << endl;
                    // Cas stockage Vendredi
                    totalCost += (state[STOCK] * constants[PURCHASE_PRICE] * constants[OWNERSHIP_RATE] * 3) / 365;
                }
                else
                {
                    //cout << "Stock " << i << " de " << state[STOCK] << endl;
                    // Cas stockage Semaine
                    totalCost += (state[STOCK] * constants[PURCHASE_PRICE] * constants[OWNERSHIP_RATE]) / 365;
                }
            }
            
            //cout << "Total Cost " << i << " de " << totalCost << endl;
            
            double delivery = variables[0].value == 0 ? evaluatePointControle(variables, state) : evaluateRecomplete(variables, state, i);
            
            livraison[i + constants[DELAY_LIVRAISON]] = delivery;
            state[VIRTUAL_STOCK] += delivery;
            if (delivery > 0){
                //cout << "Commande jour " << i << " de " << delivery << endl;
                totalCost += constants[ORDER_COST];
            }
        }
        if(totalCost > retour)
            retour = totalCost;
        totalCost = 0;
    }
    

    return retour;
}

void Evaluateur::setRisky(){
    this->riskMode = RISK_RISKY;
    this->averageModeValue = 1;
}
void Evaluateur::setSafe(){
    this->riskMode = RISK_SAFE;
    this->averageModeValue = 1;
}
void Evaluateur::setMedium(){
    this->riskMode = RISK_MEDIUM;
    this->averageModeValue = 1;
}
void Evaluateur::setRandom(){
    this->riskMode = RISK_RANDOM;
    this->averageModeValue = 1;
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
    if(constants[VARIATION] == 0){
        return 0;
    }
    return (rand()% (int)(constants[VARIATION]*2)) - constants[VARIATION];
}

double Evaluateur::calculOrder(int day)
{
    double res = ((this->constants[INITIAL_DEMANDE] + (day * this->constants[INCREASE]))+ variation()) * (dayToMonth[day] == constants[MONTH_SAISONALITY] ? 1 + constants[PERCENT_OF_SAISONALITY] : 1);
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

double Evaluateur::prodAnnee(){

    double total = 0;
    for(int day = 0; day<262; day++){
        total+=calculOrder(day);
    }
    return total;

}

double Evaluateur::calcCommandeWilson(){
    double N = prodAnnee();
    return (sqrt((2*N*this->constants[ORDER_COST])/(this->constants[PURCHASE_PRICE]*this->constants[OWNERSHIP_RATE])));
}

double Evaluateur::calcPeriodiciteWilson(){
    double N = prodAnnee();
    double nbCommandeAnnee = calcCommandeWilson();
    int retour = (261/(N/nbCommandeAnnee))+1;
    return (double)retour;
}

double Evaluateur::minDayBeforeRupture(){
    int day = 0;
    double stock = this->constants[STOCK];
    while(stock > 0){
        stock -= calculOrder(day++);
    }
    return day;
}

//Point de controle
double Evaluateur::calcMaximumProd(){
    return calcCommandeWilson()*5;
}

double Evaluateur::calcPtControle(){
    return calcCommandeWilson()*3;
}

double Evaluateur::calcTemperature(){
    return 0.0;
}

double Evaluateur::calcVariationTemp(){
    return 0.0;
}

double Evaluateur::scoreWilson(){
    //Variable 1 = Point de controle
    //Variable 2 = Interval
    //Variable 3 = Jour de début
    vector<Variable> results;
    results.push_back(Variable(1, 1.9999));//methode
    results.push_back(Variable(1, 1.9999));//ptControle
    results.push_back(Variable(1, 1.9999));//interval
    results.push_back(Variable(1, 1.9999));//jrDebut

    results[1].value = calcPtControle();
    results[2].value = calcMaximumProd();
    results[3].value = minDayBeforeRupture();

    return evaluate(results);
}
