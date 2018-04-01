#include "../performances.hpp"
#include "../lib/csv.hpp"
#include "../evaluateur/evaluateur.hpp"
#include <thread>

#define SIZE_LTABOU 10

using namespace std;


pair<double, vector<Variable>> *solutions;


bool parallelisedLines[8];
thread threads[8];

double tabou(Evaluateur env, vector<Variable> variables, int core_id, int id = 0)
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

    while (nbNotProgressing < 1000)
    {
        
        i++;
        j = 0;
        newCurrent = -1;

        int stuckCount = 0;
        while (j < 100)
        {

            if (stuckCount > 100)
            {
                //Random walk
                for (int k = 1; k < variables.size(); k++)
                {
                    variables[k].randomise();
                }
                newCurrent = env.evaluate(variables);
                break;
            }

            int r = -1;
            if (indexDiff == -1)
            {
                r = (rand() % (variables.size()-1)) + 1;
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
                if (diff <= 0.000001)
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
        if (nbNotProgressing > 100 && nbNotProgressing % 10 == 1)
        {
            //RandomWalk, we move from this solution, we are stuck
            for (int k = 1; k < variables.size(); k++)
            {
                variables[k].randomise();
            }
        }
    }

    cout << "My best (" << id << ") is " << best << endl;
    solutions[id] = make_pair(best, bVariables);
    parallelisedLines[core_id] = false;
    threads[core_id].detach();
    return best;
}


int main(int argc, const char *argv[])
{
    srand(5);

    CSVReader csv("./sample01-20productsEN.csv");
    vector<vector<double>> content;
    csv.read(content);
    
    solutions = new pair<double, vector<Variable>>[content.size() * 2];
    
    for(int i = 0 ; i < 8 ; i++){
        parallelisedLines[i] = false;
    }
    
    
    for (int t = 0 ; t < content.size() ;)
    {
        int freeCore = -1;
        for(int j = 0 ; j < 8 ; j+=2){
            if(!parallelisedLines[j] && !parallelisedLines[j+1]){
                freeCore = j;
                break;
            }
            
        }
        
        if(freeCore == -1){
            this_thread::sleep_for(chrono::seconds(10));
            continue;
        }
        
        
        
        parallelisedLines[freeCore] = true;
        parallelisedLines[freeCore+1] = true;
        vector<double> cont = content[t];
        
        double averageDemande = (cont[INITIAL_DEMANDE] + (cont[INITIAL_DEMANDE] + cont[INCREASE]*261))/2;
        
        double minCommande = averageDemande/2;
        double maxCommande = averageDemande * cont[DELAY_LIVRAISON] * 1.5;
        

        vector<Variable> variablesOrderPoint, variablesReplenishment;
        variablesOrderPoint.push_back(Variable(0, 0.9999999));
        variablesOrderPoint.push_back(Variable(minCommande, maxCommande, 5781));
        variablesOrderPoint.push_back(Variable(minCommande, maxCommande, 32195));
        
        variablesReplenishment.push_back(Variable(1, 1.9999999));
        variablesReplenishment.push_back(Variable(1, maxCommande));
        variablesReplenishment.push_back(Variable(1, 261));
        variablesReplenishment.push_back(Variable(1, 120));
        
                
        Evaluateur env = Evaluateur(cont);

        env.setAverage(50);
        
        threads[freeCore] = thread(tabou, env, variablesOrderPoint, freeCore, t*2);
        threads[freeCore+1] = thread(tabou, env, variablesReplenishment, freeCore+1, t*2+1);
        cout << "Starting " << freeCore << " for " << t*2 << " and " << freeCore+1 << " for " << t*2 +1 << endl;
        t++;
    }
    
    for(int i = 0 ; i < 8 ;){
        if(parallelisedLines[i]){
            this_thread::sleep_for(chrono::seconds(10));
            continue;
        }
        i++;
    }
    
    vector<vector<Variable>> allSolutions;
    
    double total = 0;
    
    for(int i = 0 ; i < content.size() *2 ; i+=2){
        if(solutions[i].first < solutions[i+1].first){
            total += solutions[i].first;
            allSolutions.push_back(solutions[i].second);
        }
        else{
            allSolutions.push_back(solutions[i+1].second);
            total += solutions[i+1].first;
        }
    }
    
    
    
    cout << "Total Finale " << fixed << total << endl;

    
    csv.write(allSolutions, "tabou");

    return 0;
}
