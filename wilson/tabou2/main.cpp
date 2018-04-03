#include "../performances.hpp"
#include "../lib/csv.hpp"
#include "../evaluateur/evaluateur.hpp"
#include <thread>
#include <vector>

using namespace std;

int maxSizeList = 1000;
vector<vector<Variable>> results;
vector<double> resultsValue;

int isSame(vector<Variable> a, vector<Variable> b)
{
    for (int i = 0; i < a.size(); i++)
    {
        if (b[i].value <= a[i].value - 1 || b[i].value >= a[i].value + 1)
        {
            return 0;
        }
    }
    return 1;
}

int inList(vector<vector<Variable>> visitedList, vector<Variable> voisin)
{
    for (vector<Variable> visited : visitedList)
    {
        if (isSame(visited, voisin))
        {
            return 1;
        }
    }
    return 0;
}

void addInList(vector<vector<Variable>> list, int index, vector<Variable> toAdd)
{
    if (list.size() < maxSizeList)
    {
        list.push_back(toAdd);
        return;
    }
    else
    {
        list[index] = toAdd;
    }
}

void tabou(Evaluateur env, vector<Variable> variables, int id)
{
    int turnWithoutAmelioration = 0;

    srand(5);
    vector<vector<Variable>> visitedList;
    visitedList.push_back(variables);
    int indexList = 1;

    double bestValue = env.evaluate(variables);
    vector<Variable> best = variables;
    vector<Variable> current = variables;

    while (turnWithoutAmelioration < 200)
    {

        double bestVoisinValue = 999999999;
        vector<Variable> bestVoisin = current;
        for (int i = 0; i < 100; i++)
        {
            vector<Variable> voisin = current;
            int r = (rand() % (variables.size() - 1)) + 1;
            voisin[r].randomise();
            double valueVoisin = env.evaluate(voisin);
            if (!inList(visitedList, voisin) && valueVoisin < bestVoisinValue)
            {
                bestVoisin = voisin;
                bestVoisinValue = valueVoisin;
            }
        }

        if (bestVoisinValue < bestValue)
        {
            bestValue = bestVoisinValue;
            best = bestVoisin;
            turnWithoutAmelioration = 0;
        }
        else
            turnWithoutAmelioration++;
        current = bestVoisin;
        addInList(visitedList, indexList, current);
        indexList++;
        if (indexList == maxSizeList)
        {
            indexList = 0;
        }
    }
    results.push_back(best);
    resultsValue.push_back(bestValue);
}

int main(int argc, const char *argv[])
{
    vector<vector<Variable>> allSolutions;
    srand(5);

    CSVReader csv("./sample01-20productsEN.csv");
    vector<vector<double>> content;
    csv.read(content);

    double total = 0;

    for (int j = 0; j < content.size(); j++)
    {
        cout << "Ligne " << j+1 << endl;
        double averageDemande = (content[j][3]) + (content[j][3] + content[j][2] * 261) / 2;
        double minCommande = averageDemande / 2;
        double maxCommande = averageDemande * content[j][DELAY_LIVRAISON] * 1.5;

        Evaluateur env = Evaluateur(content[j]);
        env.setMedium();

        thread threads[2];

        for (int i = 0; i < 2; i++)
        {

            vector<Variable> variables;
            if (i % 2)
            {

                variables.push_back(Variable(0, 0.9999999));
                variables.push_back(Variable(minCommande, maxCommande));
                variables.push_back(Variable(minCommande, maxCommande));
            }
            else
            {
                variables.push_back(Variable(1, 1.9999999));
                variables.push_back(Variable(1, maxCommande));
                variables.push_back(Variable(1, 261));
                variables.push_back(Variable(1, 120));
            }

            // Start for Method 1
            threads[i] = thread(tabou, env, variables, i);
        }
        for (int i = 0; i < 2; i++)
        {
            threads[i].join();
        }
        if (resultsValue[0] < resultsValue[1])
        {
            cout << "Best Result " << resultsValue[0] << endl;
            allSolutions.push_back(results[0]);
        }
        else
        {
            cout << "Best Result " << resultsValue[1] << endl;
            allSolutions.push_back(results[1]);
        }
        resultsValue.clear();
    }

    csv.write(allSolutions, "tabou");

    return 0;
}
