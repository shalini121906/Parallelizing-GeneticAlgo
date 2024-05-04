#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <random>
#include <omp.h>

using namespace std;

const string targetString = "HI this is a TExt krish krishna krishna krishna krinsha krishna krishna dfghjfgh sdfghj sdfghj sdfghj werty ertyukmn rtyuiogf fghjkre dfghjekrtghd fdhsjk gcxhjm dgshjm e gchjm vghjk gvchjm fghjwkm gvhcjk, gvhckl,hvokjnr yuekm";

int calculateFitness(const string& individual) {
    int fitness = 0;
    #pragma omp parallel for reduction(+:fitness)
    for (size_t i = 0; i < targetString.size(); ++i) {
        if (individual[i] != targetString[i]) {
            fitness++;
        }
    }
    return fitness;
}

string crossover(const string& parent1, const string& parent2) {
    size_t size = targetString.size();
    random_device rd;
    mt19937 rng(rd());
    vector<size_t> crossoverPoints(3);

    for (int i = 0; i < 3; ++i) {
        crossoverPoints[i] = rng() % size;
        for (int j = 0; j < i; ++j) {
            while (crossoverPoints[i] == crossoverPoints[j]) {
                crossoverPoints[i] = rng() % size;
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
    random_device rd;
    mt19937 rng(rd());
    for (char& gene : individual) {
        if (rng() % 100 < mutationRate * 100) {
            gene = static_cast<char>(rng() % 128);
        }
    }
}

double measureCrossoverTime(int populationSize, int numThreads, double mutationRate) {
    vector<string> population(populationSize);

    #pragma omp parallel for
    for (int i = 0; i < populationSize; ++i) {
        for (size_t j = 0; j < targetString.size(); ++j) {
            population[i] += static_cast<char>(' ' + rand() % 95);
        }
    }

    vector<int> fitnessValues(populationSize);
    #pragma omp parallel for
    for (int i = 0; i < populationSize; ++i) {
        fitnessValues[i] = calculateFitness(population[i]);
    }

    vector<string> selectedParents(populationSize);
    #pragma omp parallel for
    for (int i = 0; i < populationSize; ++i) {
        int index1 = rand() % populationSize;
        int index2 = rand() % populationSize;
        selectedParents[i] = fitnessValues[index1] < fitnessValues[index2] ? population[index1] : population[index2];
    }

    vector<string> newPopulation(populationSize);
    double start_cross = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < populationSize; i += 2) {
        newPopulation[i] = crossover(selectedParents[i], selectedParents[i + 1]);
        newPopulation[i + 1] = crossover(selectedParents[i + 1], selectedParents[i]);
        mutate(newPopulation[i], mutationRate);
        mutate(newPopulation[i + 1], mutationRate);
    }
    return omp_get_wtime() - start_cross;
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    const double mutationRate = 0.01;
    vector<int> populationSizes = {100000, 500000, 1000000, 5000000, 10000000};
    vector<int> numThreads = {1, 2, 4, 8, 16, 32};

    for (int N : populationSizes) {
        cout << "Serial time for N = " << N << ": " << measureCrossoverTime(N, 1, mutationRate) << " seconds." << endl;
        
        vector<double> speedUps(numThreads.size(), 0.0);

        for (size_t i = 0; i < numThreads.size(); ++i) {
            int p = numThreads[i];
            double serialTime = measureCrossoverTime(N, 1, mutationRate);
            double parallelTime = measureCrossoverTime(N, p, mutationRate);
            double speedUp = serialTime / parallelTime;
            speedUps[i] = speedUp;
            cout << "Parallel time for N = " << N << ", p = " << p << ": " << parallelTime << " seconds, Speed Up: " << speedUp << endl;
        }

        // Print speed ups as an array
        cout << "Speed Ups for N = " << N << ": [";
        for (size_t j = 0; j < speedUps.size(); ++j) {
            cout << speedUps[j] << (j < speedUps.size() - 1 ? ", " : "");
        }
        cout << "]" << endl << endl;
    }

    return 0;
}
