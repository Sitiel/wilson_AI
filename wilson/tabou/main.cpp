#include "../performances.hpp"
#include "../lib/csv.hpp"
#include "../evaluateur/evaluateur.hpp"


using namespace std;


double tabou(Evaluateur* env, vector<Variable*>& variables){
    
    double current = -1;
    vector<vector<Variable>> ltabou;
    int ltabouCmp = 0;
    double best = -1;
    int i = 0, j = 0;
    double newCurrent;
    while(i < 100){
        i++;
        
        j = 0;
        newCurrent = -1;
        
        while(j < 1000){
            
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

    vector<double> content;

    content.push_back(0);
    content.push_back(21);
    content.push_back(36279);
    content.push_back(524);
    content.push_back(1.4);
    content.push_back(30);
    content.push_back(1);
    content.push_back(0.3);
    content.push_back(0.2);
    content.push_back(0.4);
    content.push_back(400);
    content.push_back(100);

    vector<Variable> variables;
    variables.push_back(Variable(1,2,1));
    variables.push_back(Variable(1,10000,1771));
    variables.push_back(Variable(1,10000,8347)); 
    
    Evaluateur* env = new Evaluateur(content);

    cout << "Cout totale: " <<  env->evaluate(variables) << endl;
    
    //cout << "Environnement value : " << env->evaluate(variables) << endl;
    
    return 0;
}
