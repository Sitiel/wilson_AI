#include "performances.hpp"
#include "csv.hpp"
#include "environnement.hpp"


using namespace std;


double tabou(Environnement* env, vector<Variable*>& variables){
    
    double current = -1;
    vector<vector<Variable>> ltabou;
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
            for(int k = 0 ; k < ltabou.size() ; k++){
                double diff = 0;
                for(int q = 0 ; q < ltabou[k].size() ; q++){
                    diff += variables[q]->compare(ltabou[k][q]);
                }
                diff /= ltabou[k].size();
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
        
        if(ltabou.size() < 10){
            vector<Variable> v;
            for(int i = 0 ; i < variables.size() ; i++){
                v.push_back(*variables[i]);
            }
            ltabou.push_back(v);
        }
        else{
            vector<Variable> v;
            for(int i = 0 ; i < variables.size() ; i++){
                v.push_back(*variables[i]);
            }
            ltabou[ltabouCmp] = v;
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
    
    cout << "Tabou found : " << tabou(env, variables) << endl;
    
    return 0;
}
