#include "../performances.hpp"
#include "../lib/csv.hpp"
#include "../evaluateur/evaluateur.hpp"
#include <cmath>
#include <thread>
#include <pthread.h>

using namespace std;

vector<vector<Variable>> retour;

double getFirstTemp(double delta, double pourcentage){
    return (-delta/log(pourcentage));
}

double calculeRecuit(double delta, double temperature){
    return exp(-delta/temperature);
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

double calculeTemperature(double temp){
    if(temp<=1){
        return 1;
    }
    return temp*0.99999;
}

double recuit(Evaluateur* eva, vector<Variable>& variables){

    vector<Variable> vBest;
    double bestDiff = -1;
    double DiffMin = -1;
    int i = 0, j = 0;
    double Diff = 0;
    double temp = getFirstTemp(10000,0.6);
    int indice = 0;

    while(i <= 1000 && j<50000 && DiffMin != 0){
        Diff = 0;
        //$K$2*PUISSANCE(A2;$K$3)+$L$2*PUISSANCE(B2;$L$3)+$M$2*PUISSANCE(C2;$M$3)+$N$2*PUISSANCE(D2;$N$3)+$O$2*PUISSANCE(E2;$O$3)
        Diff = eva->evaluate(variables);

        if(Diff<DiffMin || DiffMin == -1){
            if(bestDiff>Diff || bestDiff == -1){
                bestDiff = Diff;
                vBest = variables;
                //cout << bestDiff << " | Temperature : " << temp <<" | " << i<<endl;
            }
            DiffMin = Diff;
            //cout << bestDiff << " | " << DiffMin << " Temperature : " << temp <<" | " << i<<endl;
            j = 0;
        }else{
            //Calcul du recuit
            double k = fRand(0,1);
            if(k<calculeRecuit(Diff-DiffMin,temp)){
                DiffMin = Diff;
                //cout << bestDiff << " | " << DiffMin << " Temperature : " << temp <<" | " << i<<endl;
                j = 0;
            }else{
                variables[indice].revert();
            }
        }
        //On change le Sprim avec un voisin de Setoile
        //On change une des variables aléatoirement
        indice = (rand() % (variables.size()-1))+1;
        variables[indice].randomise();

        j++;
        i++;
        temp = calculeTemperature(temp);
    }
    cout << "Methode : "<< variables[0].value <<"Solution trouvée : " << bestDiff << " | Temperature : " << temp <<" | "<<endl;
    variables = vBest;
    return bestDiff;
}


double recuitThread(Evaluateur* eva, vector<Variable>& variables, int indiceRetour){

    vector<Variable> vBest;
    double bestDiff = -1;
    double DiffMin = -1;
    int i = 0, j = 0;
    double Diff = 0;
    double temp = getFirstTemp(10000,0.6);
    int indice = 0;

    while(i <= 1000 && j<50000 && DiffMin != 0){
        Diff = 0;
        //$K$2*PUISSANCE(A2;$K$3)+$L$2*PUISSANCE(B2;$L$3)+$M$2*PUISSANCE(C2;$M$3)+$N$2*PUISSANCE(D2;$N$3)+$O$2*PUISSANCE(E2;$O$3)
        Diff = eva->evaluate(variables);

        if(Diff<DiffMin || DiffMin == -1){
            if(bestDiff>Diff || bestDiff == -1){
                bestDiff = Diff;
                vBest = variables;
                //cout << bestDiff << " | Temperature : " << temp <<" | " << i<<endl;
            }
            DiffMin = Diff;
            //cout << bestDiff << " | " << DiffMin << " Temperature : " << temp <<" | " << i<<endl;
            j = 0;
        }else{
            //Calcul du recuit
            double k = fRand(0,1);
            if(k<calculeRecuit(Diff-DiffMin,temp)){
                DiffMin = Diff;
                //cout << bestDiff << " | " << DiffMin << " Temperature : " << temp <<" | " << i<<endl;
                j = 0;
            }else{
                variables[indice].revert();
            }
        }
        //On change le Sprim avec un voisin de Setoile
        //On change une des variables aléatoirement
        indice = (rand() % (variables.size()-1))+1;
        variables[indice].randomise();

        j++;
        i++;
        temp = calculeTemperature(temp);
    }
    variables = vBest;
    cout << "Methode : "<< vBest[0].value <<"Solution trouvée : " << bestDiff << " | Temperature : " << temp <<" | "<<endl;
    return bestDiff;
}



int main() {


    /*thread threads[8];
    bool execute[8];
    int indiceThread = 0;*/
    CSVReader csv("../../sample01-20productsEN.csv");
    vector<vector<double>> content;
    //vector<double> content;
//    CSVReader csv("FNL.csv");
//    vector<vector<double>> content;
    //vector<vector<Variable>> retour;
    csv.read(content);



//    for(int i = 0 ; i < content.size() ; i++){
//        for (int j = 0 ; j < content[i].size() ; j++){
//            cout << content[i][j] << " ";
//        }
//        cout << endl;
//    }

    srand(time(NULL));

//    vector<Variable*> variables;
//    for(int i = 0 ; i < 5 ; i++){
//        variables.push_back(new Variable(-100, 100));
//    }

//    for(int i = 0 ; i < 5 ; i++){
//        variables.push_back(new Variable(1, 10));
//    }

//    for (int j = 0 ; j < variables.size() ; j++){
//        cout << variables[j]->value << ";";
//    }



    for(unsigned int i = 0; i<content.size(); i++){
        //std::cout << "variable numero = "<<i<<std::endl;

        vector<Variable> variables;
        variables.push_back(Variable(0, 1.9999));
        for(int i = 0 ; i < 3 ; i++){
            variables.push_back(Variable(1, 100000));
        }

        Evaluateur* eva = new Evaluateur(content[i]);
        threads[indiceThread] = thread(recuit, eva, variables);
        //recuit(eva, variables);
        retour.push_back(variables);
    }
//        Evaluateur* eva = new Evaluateur(content);
//        recuit(eva, variables);
//        retour.push_back(variables);



    csv.write(retour);

    return 0;
}
