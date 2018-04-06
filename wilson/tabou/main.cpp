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
    double progression = 1;

    int ltabouCmp = 0;
    double best = env.evaluate(variables);
    int i = 0, j = 0;
    vector<Variable> bVariables;
    double newCurrent;

    double previousDiff = -1;
    int indexDiff = -1;

    int nbNotProgressing = 0;
    
    vector<double> data;

    while (nbNotProgressing < 100)
    {
        
        i++;
        j = 0;
        newCurrent = -1;
        
        //progression *= 0.999;

        int stuckCount = 0;
        while (j < 10)
        {

            if (stuckCount > 1000)
            {
                //Random walk
                for (int k = 1; k < variables.size(); k++)
                {
                    variables[k].randomise();
                }
                progression = 1;
                newCurrent = env.evaluate(variables);
                nbNotProgressing++;
                break;
            }

            int r = -1;
            if (indexDiff == -1)
            {
                r = (rand() % (variables.size()-1)) + 1;
                variables[r].randomise(0.2);
            }
            else
            {
                r = indexDiff;
                variables[r].add(previousDiff);
            }

            bool found = false;
            for (int k = 0; k < ltabou.size(); k++)
            {
                found = true;
                for (int q = 0; q < ltabou[k].size(); q++)
                {
                    if(!variables[q].isEqual(ltabou[k][q]))
                    {
                        found = false;
                        break;
                    }
                }
                if (found)
                {
                    stuckCount++;
                    break;
                }
                
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
        //data.push_back(current);
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
        if (nbNotProgressing > 200 && nbNotProgressing % 200 == 1)
        {
            //RandomWalk, we move from this solution, we are stuck
            for (int k = 1; k < variables.size(); k++)
            {
                variables[k].randomise();
            }
            progression = 1;
        }
    }
    
    /*if(id == 0){
        CSVReader c;
        c.write(data);
    }*/

    cout << "My best (" << id << ") is " << best << endl;
    solutions[id] = make_pair(best, bVariables);
    parallelisedLines[core_id] = false;
    threads[core_id].detach();
    return best;
}


int main(int argc, const char *argv[])
{
    srand(5);
 
    CSVReader csv("./sample02-160productsEN.csv");
    vector<vector<double>> content;
    csv.read(content);
    
    solutions = new pair<double, vector<Variable>>[content.size() * 2];
    
    for(int i = 0 ; i < 8 ; i++){
        parallelisedLines[i] = false;
    }
    
    vector<vector<Variable>> starter;
    
    if(argc == 2){
        CSVReader csv(argv[1]);
        csv.read(starter);
    }
    
    for (int t = 0 ; t < 60*2 ;)
    {
        int freeCore = -1;
        for(int j = 0 ; j < 8 ; j++){
            if(!parallelisedLines[j]){
                freeCore = j;
                break;
            }
            
        }
        
        if(freeCore == -1){
            this_thread::sleep_for(chrono::seconds(10));
            continue;
        }
        
        parallelisedLines[freeCore] = true;
        int line = t/2;
        vector<double> cont = content[line];
        
        Evaluateur env = Evaluateur(cont);
        
        
        double averageDemande = (cont[INITIAL_DEMANDE] + (cont[INITIAL_DEMANDE] + cont[INCREASE]*261))/2;
        /*double minCommande = 100;
        double maxCommande = 15000;*/
        //double minCommande = averageDemande/2;
        //double maxCommande = averageDemande * cont[DELAY_LIVRAISON] * 1.5;
        double minCommande = (int)(env.calcCommandeWilson()/3);
        double maxCommande = (int)(env.calcCommandeWilson()*3);
        
        double minPeriode = (int)(env.calcPeriodiciteWilson()/3);
        double maxPeriode = (int)(env.calcPeriodiciteWilson()*3);
        
        vector<Variable> variables;
        if(argc != 2){
            
            if(t % 2){
                variables.push_back(Variable(1, 1.9999999));
                
                variables.push_back(Variable(minCommande,maxCommande));
                variables.push_back(Variable(minPeriode, maxPeriode));
                variables.push_back(Variable(1, 120));
            }
            else{
                variables.push_back(Variable(0, 0.9999999));
                variables.push_back(Variable(minCommande, maxCommande));
                variables.push_back(Variable(minCommande, maxCommande));
            }
        }
        else{
            variables = starter[t/2];
            if(variables[0].value < 1){
                variables[1].rebound(minCommande, maxCommande);
                variables[2].rebound(minCommande, maxCommande);
            }
            else{
                variables[1].rebound(minCommande, maxCommande);
                variables[2].rebound(minPeriode, maxPeriode);
                variables[3].rebound(1, 120);
            }
        }
        

        env.setAverage(10);
        //env.setMedium();
        
        
        threads[freeCore] = thread(tabou, env, variables, freeCore, t);
        cout << "Starting " << freeCore << " for " << t << endl;
        t++;
        if(argc == 2)
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
    
    if(argc == 2){
        for(int i = 0 ; i < content.size() * 2 ; i+=2){
            allSolutions.push_back(solutions[i].second);
            total += solutions[i].first;
        }
    }
    else{
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
    }
    
    
    cout << "Total Finale " << fixed << total << endl;

    
    csv.write(allSolutions, "tabou");

    return 0;
}
