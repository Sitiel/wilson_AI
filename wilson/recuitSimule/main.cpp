#include "/home/imerir/Prog/A2/Sa/Optimisation/Projet-Jeudi/GitProj/wilson_AI/wilson/performances.hpp"
#include "/home/imerir/Prog/A2/Sa/Optimisation/Projet-Jeudi/GitProj/wilson_AI/wilson/lib/csv.hpp"
#include "/home/imerir/Prog/A2/Sa/Optimisation/Projet-Jeudi/GitProj/wilson_AI/wilson/lib/environnement.hpp"


using namespace std;


double recuit(Environnement* env, vector<Variable*>& variables){

    double current = -1;
    vector<vector<Variable>> lrecuit;
    int ltabouCmp = 0;
    double best = -1;
    int i = 0, j = 0;
    double newCurrent;
    while(i < 1000){
        i++;

        j = 0;
        newCurrent = -1;

        while(j < 100000){

            int r = rand()%variables.size();
            variables[r]->randomise();


            bool found = false;
            for(unsigned int k = 0 ; k < lrecuit.size() ; k++){
                double diff = 0;
                for(unsigned int q = 0 ; q < lrecuit[k].size() ; q++){
                    diff += variables[q]->compare(lrecuit[k][q]);
                }
                diff /= lrecuit[k].size();
                if(diff < 0.01){
                    found = true;
                    break;
                }
            }
            if (found)
                continue;

            double tmpEval = env->evaluate(variables);
            if(tmpEval < newCurrent || newCurrent == -1){
                newCurrent = tmpEval;
            }
            else{
                variables[r]->revert();
            }
            j++;
        }

        if(lrecuit.size() < 10){
            vector<Variable> v;
            for(unsigned int i = 0 ; i < variables.size() ; i++){
                v.push_back(*variables[i]);
            }
            lrecuit.push_back(v);
        }
        else{
            vector<Variable> v;
            for(unsigned int i = 0 ; i < variables.size() ; i++){
                v.push_back(*variables[i]);
            }
            lrecuit[ltabouCmp] = v;
        }

        ltabouCmp++;
        if(ltabouCmp > 10)
            ltabouCmp = 0;



        current = newCurrent;
        if(current < best || best == -1)
            best = current;
    }
    return best;
}


int main(int argc, const char * argv[]) {


    CSVReader csv("FNL.csv");
    vector<vector<double>> content;
    csv.read(content);
    /*
    for(int i = 0 ; i < content.size() ; i++){
        for (int j = 0 ; j < content[i].size() ; j++){
            cout << content[i][j] << " ";
        }
        cout << endl;
    }
    */
    srand(5);

    vector<Variable*> variables;
    for(int i = 0 ; i < 5 ; i++){
        variables.push_back(new Variable(-100, 100));
    }

    for(int i = 0 ; i < 5 ; i++){
        variables.push_back(new Variable(1, 10));
    }

    Environnement* env = new Environnement(content);

    //cout << "Environnement value : " << env->evaluate(variables) << endl;

    cout << "Recuit found : " << recuit(env, variables) << endl;

    return 0;
}
