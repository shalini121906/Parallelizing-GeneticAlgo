#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <omp.h>

using namespace std;

// 8.478

const string targetString = "HI this is a TExt krish krishna krishna krishna krinsha krishna krishna dfghjfgh sdfghj sdfghj sdfghj werty ertyukmn rtyuiogf fghjkre dfghjekrtghd fdhsjk gcxhjm dgshjm e gchjm vghjk gvchjm  fghjwkm gvhcjk,  gvhckl,hvokjnr yuekm"; 

int calculateFitness(const string& individual) {
    int fitness = 0;
    for (size_t i = 0; i < targetString.size(); ++i) {
        if (individual[i] != targetString[i]) {
            fitness++;
        }
    }
    return fitness;
}

string crossover(const string& parent1, const string& parent2) {
    size_t size = targetString.size();

    vector<size_t> crossoverPoints(3);
    
    for (int i = 0; i < 3; ++i) {
        crossoverPoints[i] = rand() % size;
        for (int j = 0; j < i; ++j) {
            while (crossoverPoints[i] == crossoverPoints[j]) {
                crossoverPoints[i] = rand() % size;
            }
        }
    }

    sort(crossoverPoints.begin(), crossoverPoints.end());

    string child = parent1.substr(0, crossoverPoints[0]) +
                   parent2.substr(crossoverPoints[0], crossoverPoints[1] - crossoverPoints[0]) +
                   parent1.substr(crossoverPoints[1], crossoverPoints[2] - crossoverPoints[1]) +
                   parent2.substr(crossoverPoints[2]);

    return child;
}


void mutate(string& individual, double mutationRate) {
    for (char& gene : individual) {
        if (rand() % 100 < mutationRate * 100) {
            gene = static_cast<char>(rand() % 128);
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    const int populationSize = 1000000;
    const double mutationRate = 0.01;

    vector<string> population(populationSize);
    for (string& individual : population) {
        for (size_t i = 0; i < targetString.size(); ++i) {
            individual += static_cast<char>(rand() % 128);
        }
    }

    int generation = 1;
    
        vector<int> fitnessValues(populationSize);
        double start_fit = omp_get_wtime();
        for (int i = 0; i < populationSize; ++i) {
            fitnessValues[i] = calculateFitness(population[i]);
        }
        double duration_fit = omp_get_wtime() - start_fit;

        int bestIndex = distance(fitnessValues.begin(), min_element(fitnessValues.begin(), fitnessValues.end()));
        
        vector<string> selectedParents;
        double start_select = omp_get_wtime();
        for (int i = 0; i < populationSize; ++i) {
            int index1 = rand() % populationSize;
            int index2 = rand() % populationSize;
            selectedParents.push_back(fitnessValues[index1] < fitnessValues[index2] ? population[index1] : population[index2]);
        }
        double duration_select = omp_get_wtime() - start_select;

    double start_cross = omp_get_wtime();
        vector<string> newPopulation;
        for (int i = 0; i < populationSize; i += 2) {
            string child1 = crossover(selectedParents[i], selectedParents[i + 1]);
            string child2 = crossover(selectedParents[i + 1], selectedParents[i]);
            mutate(child1, mutationRate);
            mutate(child2, mutationRate);
            newPopulation.push_back(child1);
            newPopulation.push_back(child2);
        }
    double duration = omp_get_wtime() - start_cross;
    cout << " Duration of cross: " << duration << "\n";
    return 0;
}