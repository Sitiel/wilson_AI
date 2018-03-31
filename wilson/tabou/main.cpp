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
    srand(5);
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

    while (i < 100)
    {
        i++;
        j = 0;
        newCurrent = -1;
        

        int stuckCount = 0;
        while (j < 100)
        {

            if (stuckCount > 50)
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
                indexDiff = r;
                previousDiff = variables[r].calculDiff();
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
        if (nbNotProgressing >= 5)
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
    srand(5);

    CSVReader csv("./sample01-20productsEN.csv");
    vector<vector<double>> content;
    vector<vector<Variable>> retour;
    csv.read(content);
    double total = 0;
    
    vector<vector<Variable>> allSolutions;
    for (int t = 0 ; t < content.size() ; t++)
    {
        vector<double> cont = content[t];
        cout << "LIGNE CSV " << t+1 << endl;
        thread threads[8];
        
        double averageDemande = (cont[INITIAL_DEMANDE] + (cont[INITIAL_DEMANDE] + cont[INCREASE]*261))/2;
        
        
        double minCommande = averageDemande/2;
        double maxCommande = averageDemande * cont[DELAY_LIVRAISON] * 1.5;
        for (int i = 0; i < 8; i++)
        {
            vector<Variable> variables;
            if (i % 2) {
                
                variables.push_back(Variable(0, 0.9999999));
                variables.push_back(Variable(minCommande, maxCommande));
                variables.push_back(Variable(minCommande, maxCommande));
            }
            else {
                variables.push_back(Variable(1, 1.9999999));
                variables.push_back(Variable(1, maxCommande));
                variables.push_back(Variable(1, 261));
                variables.push_back(Variable(1, 120));
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
        /*
        
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
        }*/
        
        
        for(int i = 0 ; i < 8 ; i++){
            cout << "Value : " << solutions[i].first << " for solution " << i << endl;
            for(int j = 0 ; j < solutions[i].second.size() ; j++){
                cout << solutions[i].second[j].value << " ";
            }
            cout << endl;
        }
        
        //TAKE THE BEST VERSUS THE RANDOM
        Evaluateur testeur(cont);
        testeur.setRandom();
        int bestIndex = -1;
        double currentBestVsRandom = 0;
        
        for(int i = 0 ; i < 8 ; i++){
            //CALCULATE THE AVERAGE VS RANDOM
            double result = 0;
            for(int a = 0 ; a < 1000 ; a++){
                result += testeur.evaluate(solutions[i].second);
            }
            result /= 1000;
            if(result < currentBestVsRandom || bestIndex == -1){
                currentBestVsRandom = result;
                bestIndex = i;
            }
        }
        cout << "Best versus random is solution " << bestIndex << " with " << fixed << currentBestVsRandom << endl;
        /*
        Evaluateur testeur(cont);
        testeur.setMedium();
        double result = testeur.evaluate(solutions[index].second);
        double diff = result - bestSolution;
        cout << "Result " << result << " vs " << bestSolution << endl;
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
        
        allSolutions.push_back(solutions[bestIndex].second);
        
        cout << "Tabou : ";

        cout << fixed << currentBestVsRandom << endl;
        total += currentBestVsRandom;
        bestTabou = -1;
    }
    cout << "Total Finale " << fixed << total << endl;

    
    csv.write(allSolutions, "tabou");

    return 0;
}
