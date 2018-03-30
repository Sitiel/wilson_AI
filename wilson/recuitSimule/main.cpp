#include "../performances.hpp"
#include "../lib/csv.hpp"
#include "../evaluateur/evaluateur.hpp"
#include <cmath>
#include <thread>
#include <pthread.h>

using namespace std;
#define nbThread 3

vector<vector<Variable>> retour;
vector<double> bestDiffs;
//Tableau de booléen des threads
bool execute[nbThread];
thread threads[nbThread];

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

    //250000
    while(i <= 250000 && j<50000 && DiffMin != 0){
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
    return bestDiff;
}


void recuitThread(Evaluateur* eva, vector<Variable> variables, int indiceRetour, int indiceThread){
    double bestDiff;
    //Appel de recuit
    bestDiff = recuit(eva,variables);
    //Ecriture dans un fichier

//    string cheminFichier = "lignes/"+indiceRetour;
//    CSVReader csv(cheminFichier);
//    csv.write(tabFichier);
    if(variables[0].value == 1){
        if(bestDiff<bestDiffs[indiceRetour]){
            retour[indiceRetour]=variables;
            bestDiffs[indiceRetour]=bestDiff;
            cout << "A0" << indiceRetour+1 << "Solution Améliorée : " << bestDiff << endl;
        }
    }else{
        retour[indiceRetour]=variables;
        bestDiffs[indiceRetour]=bestDiff;
        cout << "A0" << indiceRetour+1 << "Solution trouvée : " << bestDiff << endl;
    }

    //On libère la place
    execute[indiceThread] = false;
    threads[indiceThread].detach();
}

int verifThread(){
    int i=0;
    while(execute[i]&&i<nbThread){
        i++;
    }
    if(i==nbThread){
        return -1;
    }
    return i;
}

int main() {


    int indiceThread = 0;
    CSVReader csv("../../sample01-20productsEN.csv");
    vector<vector<double>> content;
    //vector<double> content;
//    CSVReader csv("FNL.csv");
//    vector<vector<double>> content;
    //vector<vector<Variable>> retour;
    csv.read(content);

    vector<Variable> tmp;
    for(unsigned int i = 0; i<content.size();i++){
        retour.push_back(tmp);
    }
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

    //PointContrôle
    unsigned int i = 0;
    while(i<content.size()*2){
        //std::cout << "variable numero = "<<i<<std::endl;
        // verif de thread libre
        indiceThread = verifThread();
        if(indiceThread!=-1){
            i++;
            //cout <<" lancement de thread n: "<<i<<endl;
            vector<Variable> variables;
            if (i>=content.size()){
                variables.push_back(Variable(1, 1.9999));
            }else{
                variables.push_back(Variable(0, 0.9999));
            }
            for(int i = 0 ; i < 3 ; i++){
                variables.push_back(Variable(1, 100000));
            }
            Evaluateur* eva = new Evaluateur(content[(i-1)%content.size()]);
            eva->setMedium();
            //On bloque la place
            execute[indiceThread] = true;
            threads[indiceThread] = thread(recuitThread, eva, variables, (i-1)%content.size(), indiceThread);
        }
        //recuit(eva, variables);
    }

    //retour.push_back(variables);
//        Evaluateur* eva = new Evaluateur(content);
//        recuit(eva, variables);
//        retour.push_back(variables);
    for(int i = 0; i<nbThread;i++){
        if (execute[i]){
          threads[i].join();
        }

    }

    // TODO func qui recupere les fichier des threads pour les reecrire dans un seul fichier


    csv.write(retour,"Recuit");

    return 0;
}
