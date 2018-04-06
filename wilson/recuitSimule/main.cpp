#include "../performances.hpp"
#include "../lib/csv.hpp"
#include "../evaluateur/evaluateur.hpp"
#include <cmath>
#include <thread>
#include <pthread.h>

using namespace std;
#define nbThread 5

vector<vector<Variable>> retour;
vector<double> bestDiffs;
//Tableau de booléen des threads
bool execute[nbThread];
thread threads[nbThread];
int maxIteration = 50000;
int maxStagn = 500;
bool maxIterationAug = false;
double variationTemp = 0.999961;


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
    return temp*variationTemp;
}

double tempInit(Evaluateur* eva){
    return getFirstTemp(eva->scoreWilson(),0.36)/10;
}

void initVariationTemp(double tempinit){
    variationTemp = abs(pow(tempinit,1/maxIteration)-2);
}

double recuit(Evaluateur* eva, vector<Variable>& variables){

    vector<Variable> vBest;
    double bestDiff = -1;
    double DiffMin = -1;
    int i = 0, j = 0;
    double Diff = 0;
    double temp = -1;
    int indice = 0;

    if(variables[0].value == 1 && maxIterationAug==false){
        maxIteration = maxIteration*1.5;
        maxStagn = maxStagn*1.5;
        maxIterationAug = true;
    }

    //Initialisation temperature
    temp = tempInit(eva);
    initVariationTemp(temp);

    //250000
    while(i <= maxIteration && j<maxStagn){
        Diff = 0;
        Diff = eva->evaluate(variables);

        if(Diff<DiffMin || DiffMin == -1){
            if(bestDiff>Diff || bestDiff == -1){
                bestDiff = Diff;
                vBest = variables;
                //cout << bestDiff << " | Temperature : " << temp <<" | " << i<<endl;
                j = 0;
            }
            DiffMin = Diff;
            //cout << bestDiff << " | " << DiffMin << " Temperature : " << temp <<" | " << i<<endl;
        }else{
            //Calcul du recuit
            double k = fRand(0,1);
            if(k<calculeRecuit(Diff-DiffMin,temp)){
                DiffMin = Diff;
                //cout << bestDiff << " | " << DiffMin << " Temperature : " << temp <<" | " << i<<endl;
                //j = 0;
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
            cout << "A0" << indiceRetour+1 << "Solution Améliorée : " << fixed << bestDiff << endl;
        }else{
            cout << "A0" << indiceRetour+1 << "Solution non Améliorée" << endl;
        }
    }else{
        retour[indiceRetour]=variables;
        bestDiffs[indiceRetour]=bestDiff;
        cout << "A0" << indiceRetour+1 << "Solution trouvée : " << fixed << bestDiff << endl;
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

//    Evaluateur* eva = new Evaluateur(content[1]);
//    eva->setMedium();

//    cout << "Replennishement : " << endl;
//    cout << "Total commande dans l'année : " << eva->calcCommandeWilson() << endl;
//    cout << "Jours commande dans l'année : " << eva->calcPeriodiciteWilson() << endl;
//    cout << "Premier jours : " << eva->minDayBeforeRupture() << endl;
//    cout << "Score wilson : " << eva->scoreWilson() <<endl;
    //Initialisation tableau pour recuit
    vector<Variable> tmp;
    for(unsigned int i = 0; i<content.size();i++){
        retour.push_back(tmp);
        bestDiffs.push_back(0.0);
    }


    srand(time(NULL));

    //PointContrôle
    unsigned int i = 0;
    while(i<content.size()*2){
        //std::cout << "variable numero = "<<i<<std::endl;
        // verif de thread libre
        indiceThread = verifThread();
        if(indiceThread!=-1){
            i++;
            //cout <<" lancement de thread n: "<<i<<endl;
            Evaluateur* eva = new Evaluateur(content[(i-1)%content.size()]);
            eva->setAverage(10);

            vector<Variable> variables;
            if (i>content.size()){
                variables.push_back(Variable(1, 1.9999));
                variables.push_back(Variable(1, eva->calcCommandeWilson()));
                variables.push_back(Variable(1, eva->calcPeriodiciteWilson()));
                variables.push_back(Variable(1, eva->minDayBeforeRupture()));
            }else{
                variables.push_back(Variable(0, 0.9999));
                variables.push_back(Variable(1, eva->calcMaximumProd()));
                variables.push_back(Variable(1, eva->calcPtControle()));
            }

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

    csv.write(retour,"Recuit");

    return 0;
}
