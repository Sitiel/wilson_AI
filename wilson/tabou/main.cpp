#include "../performances.hpp"
#include "../lib/csv.hpp"
#include "../evaluateur/evaluateur.hpp"
#include <thread>

#define SIZE_LTABOU 10

using namespace std;

double bestTabou = -1;


pair<double, vector<Variable>> solutions[8];


double tabou(Evaluateur env, vector<Variable> variables, int id = 0)
{
    vector<vector<Variable>> ltabou;
    double current = -1;

    int ltabouCmp = 0;
    double best = -1;
    int i = 0, j = 0;
    vector<Variable> bVariables;
    double newCurrent;

    double previousDiff = -1;
    int indexDiff = -1;

    int nbNotProgressing = 0;

    while (i < 200)
    {
        i++;
        j = 0;
        newCurrent = -1;

        int stuckCount = 0;
        while (j < 200)
        {

            if (stuckCount > 100)
            {
                //Random walk
                for (int k = 1; k < variables.size(); k++)
                {
                    variables[k].randomise();
                }
                break;
            }

            int r = -1;
            if (indexDiff == -1)
            {
                r = (rand() % (variables.size()-1)) +1;
                variables[r].randomise();
            }
            else
            {
                r = indexDiff;
                variables[r].add(previousDiff);
            }

            bool found = false;
            for (int k = 0; k < ltabou.size(); k++)
            {
                double diff = 0;
                for (int q = 0; q < ltabou[k].size(); q++)
                {
                    diff += variables[q].compare(ltabou[k][q]);
                }
                if (diff <= 0.0001)
                {
                    found = true;
                    stuckCount++;
                    break;
                }
                

                if (found)
                    break;
            }
            if (found)
                continue;

            double tmpEval = env.evaluate(variables);
            if (tmpEval < newCurrent || newCurrent == -1)
            {
                newCurrent = tmpEval;
                //indexDiff = r;
                //previousDiff = variables[r].calculDiff();
            }
            else
            {
                variables[r].revert();
                indexDiff = -1;
                previousDiff = -1;
            }
            j++;
        }

        vector<Variable> v = variables;
        if (ltabou.size() < SIZE_LTABOU)
        {
            ltabou.push_back(v);
        }
        else
        {
            ltabou[ltabouCmp] = v;
        }

        ltabouCmp++;
        if (ltabouCmp >= SIZE_LTABOU)
            ltabouCmp = 0;

        current = newCurrent;
        if (current < best || best == -1)
        {
            best = current;
            bVariables = variables;
            nbNotProgressing = 0;
        }
        else
        {
            nbNotProgressing++;
        }
        if (nbNotProgressing >= 10)
        {
            //RandomWalk, we move from this solution, we are stuck
            for (int k = 1; k < variables.size(); k++)
            {
                variables[k].randomise();
            }
            nbNotProgressing = 0;
        }
    }

    //cout << "My best (" << id << ") is " << best << endl;
    if (best < bestTabou || bestTabou == -1)
        bestTabou = best;
    solutions[id] = make_pair(best, bVariables);
    return best;
}

int main(int argc, const char *argv[])
{

    //CSVReader csv("FNL.csv");
    //csv.read(content);
    /*
    for(int i = 0 ; i < content.size() ; i++){
        for (int j = 0 ; j < content[i].size() ; j++){
            cout << content[i][j] << " ";
        }
        cout << endl;
    }
    */
    srand(5);

    //vector<double> content;

    /*#define DELAY_LIVRAISON 0
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
#define VIRTUAL_STOCK 11*/
    /* vector<double> content;

    content.push_back(4);
    content.push_back(1456);
    content.push_back(68);
    content.push_back(2.8);
    content.push_back(30);
    content.push_back(1);
    content.push_back(0.3);
    content.push_back(0.3);
    content.push_back(0.4);
    content.push_back(2500);
    content.push_back(268);

      vector<Variable> variables;
    variables.push_back(Variable(1,1.9999999));
    variables.push_back(Variable(1,50000, 4000));
    variables.push_back(Variable(1,10000, 1));
    variables.push_back(Variable(1,10000, 4));*/

    /*content.push_back(21);
    content.push_back(36279);
    content.push_back(524);
    content.push_back(-1.4);
    content.push_back(30);
    content.push_back(1);
    content.push_back(0.3);
    content.push_back(0.2);
    content.push_back(0.4);
    content.push_back(400);
    content.push_back(100);
    
    vector<Variable> variables;
    variables.push_back(Variable(0,0.9999,0));
    variables.push_back(Variable(1,10000,1771));
    variables.push_back(Variable(1,10000,8347));
    
    
    /*Variable v1(1, 10000, 59);
    Variable v2(1, 10000, 60);*/

    CSVReader csv("./sample01-20productsEN.csv");
    vector<vector<double>> content;
    vector<vector<Variable>> retour;
    csv.read(content);
    double total = 0;
    
    vector<vector<Variable>> allSolutions;
    for (int t = 0 ; t < content.size() ; t++)
    {
        vector<double> cont = content[t];
        cout << "LIGNE CSV " << t << endl;
        thread threads[8];
        for (int i = 0; i < 8; i++)
        {
            vector<Variable> variables;
            if (i % 2) {
                variables.push_back(Variable(0, 0.9999999));
                variables.push_back(Variable(1, 10000));
                variables.push_back(Variable(1, 10000));
            }
            else {
                variables.push_back(Variable(1, 1.9999999));
                variables.push_back(Variable(1, 1000000));
                variables.push_back(Variable(1, 261));
                variables.push_back(Variable(1, 150));
            }
                
            Evaluateur env = Evaluateur(cont);
            if (i == 0 || i == 1)
                env.setSafe();
            else if (i == 2 || i == 3)
                env.setRandom();
            else if (i == 4 || i == 5)
                env.setMedium();
            else if (i == 6 || i == 7)
                env.setRisky();
            
            env.setMedium();

            threads[i] = thread(tabou, env, variables, i);
        }
        for (int i = 0; i < 8; i++)
        {
            threads[i].join();
        }
        
        double bestSolution = solutions[0].first;
        int index = 0;
        
        double secondBest = solutions[1].first;
        int indexSecond = 1;
        
        if(secondBest < bestSolution){
            double tmp = bestSolution;
            int tmpI = index;
            bestSolution = secondBest;
            index = indexSecond;
            
            indexSecond = tmpI;
            secondBest = tmp;
        }
        
        for(int i = 2 ; i < 8 ; i++){
            if(solutions[i].first < bestSolution){
                secondBest = bestSolution;
                indexSecond = index;
                index = i;
                bestSolution = solutions[i].first;
            }else{
                if (solutions[i].first < secondBest){
                    secondBest = solutions[i].first;
                    indexSecond = i;
                }
            }
        }
        
        
        for(int i = 0 ; i < 8 ; i++){
            cout << "Value : " << solutions[i].first << endl;
            for(int j = 0 ; j < solutions[i].second.size() ; j++){
                cout << solutions[i].second[j].value << " ";
            }
            cout << endl;
        }
        
        
        Evaluateur testeur(cont);
        testeur.setMedium();
        double result = testeur.evaluate(solutions[index].second);
        double diff = result - bestSolution;
        /*cout << "Result " << result << " vs " << bestSolution << endl;
        cout << "Perte si choix 2 : " <<  secondBest - bestSolution << endl;
        cout << "Perte potentiel : " << diff  << endl;
        
        if (diff >= secondBest - bestSolution){
            cout << "Risk is too important " << bestSolution << " vs " << secondBest << endl;
            bestSolution = secondBest;
            index = indexSecond;
        }
        else{
            cout << "Risk is valuable for " << bestSolution << " vs " << secondBest << endl;
        }*/
        
        allSolutions.push_back(solutions[index].second);
        
        cout << "Tabou : ";

        cout << fixed << bestSolution;
        cout << endl;
        total += bestTabou;
        bestTabou = -1;
    }
    cout << "Total Finale " << fixed << total << endl;

    
    csv.write(allSolutions, "tabou");
   /* vector<Variable> variables;
    variables.push_back(Variable(0,0.9999,0));
    variables.push_back(Variable(1,10000,5081));
    variables.push_back(Variable(1,10000,4349));
    variables.push_back(Variable(1,10000,174780));

    Evaluateur eval(content[0]);
    eval.setMedium();
    cout << "Res " << eval.evaluate(variables) << endl;*/

    return 0;
}
