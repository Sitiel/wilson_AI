//
//  evaluateCSV.cpp
//  wilson
//
//  Created by Valérian Acier on 31/03/2018.
//  Copyright © 2018 Valérian Acier. All rights reserved.
//

#include "evaluateCSV.hpp"
#include <vector>
#include "../wilson/evaluateur/evaluateur.hpp"
#include "../wilson/lib/csv.hpp"
#include "../wilson/lib/variable.hpp"

using namespace std;



double evaluateCSV(string name){
    CSVReader csv(name);
    vector<vector<Variable>> variables;
    csv.read(variables);
    CSVReader csvConstants("./sample02-160productsEN.csv");
    vector<vector<double>> content;
    csvConstants.read(content);
    double score = 0;
    for(int i = 0 ; i < content.size() ; i++){
        double lineScore = 0;
        for(int a = 0 ; a < 100 ; a++){
            Evaluateur env(content[i]);
            env.setRandom();
            lineScore += env.evaluate(variables[i]);
        }
        score += (lineScore/100);
        cout << "Le score moyen de la ligne " << i+1 << " est " << fixed << lineScore/100 << endl;
    }


    return score;
}

void mergeCSV(vector<string> names, int mergeType){
    vector<vector<vector<Variable>>> variables;
    string filename = "merge";
    for(int i = 0 ; i < names.size() ; i++){
        vector<vector<Variable>> v;
        CSVReader csv(names[i]);
        csv.read(v);
        variables.push_back(v);
        filename += "_" + names[i];
    }

    CSVReader csvConstants("./sample02-160productsEN.csv");
    vector<vector<double>> content;
    csvConstants.read(content);
    vector<vector<Variable>> bestVariables;

    for(int i = 0 ; i < content.size() ; i++){
        vector<double> scoreVariables;
        for(int j = 0 ; j < variables.size() ; j++){
            double score = 0;
            for(int a = 0 ; a < 100 ; a++){
                Evaluateur env(content[i]);
                env.setRandom();
                score += env.evaluate(variables[j][i]);
            }
            cout << "Score of line " << i << " for file " << names[j] << " : " << score/100 << endl;
            scoreVariables.push_back(score / 100);
        }
        double minScore = -1;
        int minIndex = -1;
        for(int x = 0 ; x < scoreVariables.size() ; x++){
            if(scoreVariables[x] < minScore || minScore == -1){
                minIndex = x;
                minScore = scoreVariables[x];
            }
        }

        cout << "Best for line " << i+1 << " is " << fixed << minScore << endl;
        bestVariables.push_back(variables[minIndex][i]);
    }

    CSVReader csv;
    csv.write(bestVariables, "merge", filename);
    cout << "Files merged" << endl;


}


int main(int argc, char *argv[]){
    if(argc == 1){
        cout << "Erreur veuillez passer des fichiers en paramétre" << endl;
        exit(0);
    }

    if(strcmp(argv[1], "aleamerge") == 0){
        vector<string> names;
        for(int i = 2 ; i < argc ; i++){
            names.push_back(string(argv[i]));
        }
        mergeCSV(names, 1);
        exit(0);
    }

    if(argc == 2){
        cout << "CSV : " << argv[1] << endl;
        double score = evaluateCSV(argv[1]);
        cout << "Total score is " << fixed << score << endl;
        exit(0);
    }

    for(int i = 1 ; i < argc ; i++){
        cout << "CSV : " << argv[i] << endl;
        double score = evaluateCSV(argv[i]);
        cout << "Total score is " << fixed << score << endl;
    }
    return 0;
}

