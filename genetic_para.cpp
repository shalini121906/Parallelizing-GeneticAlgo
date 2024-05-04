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

double generatePopulation(int populationSize) {
    double start_pop = omp_get_wtime();
    vector<string> population(populationSize);
    #pragma omp parallel for
    for (int i = 0; i < populationSize; ++i) {
        for (size_t j = 0; j < targetString.size(); ++j) {
            population[i] += static_cast<char>(rand() % 128);
        }
    }
    return omp_get_wtime() - start_pop;
}

double measureFitnessCalculationTime(int populationSize) {
    vector<string> population(populationSize);
    vector<int> fitnessValues(populationSize);
    unsigned seed = static_cast<unsigned>(time(0));
    mt19937 rng(seed);

    #pragma omp parallel for
    for (int i = 0; i < populationSize; ++i) {
        string& individual = population[i];
        for (size_t j = 0; j < targetString.size(); ++j) {
            char randChar = static_cast<char>(' ' + rng() % 95); // Generate printable characters
            individual += randChar;
        }
    }

    double startFit = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < populationSize; ++i) {
        fitnessValues[i] = 0; // Stub fitness calculation
    }
    return omp_get_wtime() - startFit;
}

double measureCrossoverTime(int populationSize) {
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
        fitnessValues[i] = 0; // Stub fitness calculation
    }

    vector<string> selectedParents(populationSize);
    #pragma omp parallel for
    for (int i = 0; i < populationSize; ++i) {
        selectedParents[i] = population[i]; // Stub selection process
    }

    vector<string> newPopulation(populationSize);
    double start_cross = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < populationSize; i += 2) {
        newPopulation[i] = population[i]; // Stub crossover process
        newPopulation[i + 1] = population[i + 1]; // Stub crossover process
    }
    return omp_get_wtime() - start_cross;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    vector<int> populationSizes = {100000, 500000, 1000000, 5000000, 10000000};

    for (int N : populationSizes) {
        double totalDuration = 0.0;
        for (int i = 0; i < 3; ++i) {
            double populationTime = generatePopulation(N);
            double fitnessTime = measureFitnessCalculationTime(N);
            double crossoverTime = measureCrossoverTime(N);
            totalDuration += populationTime + fitnessTime + crossoverTime;
        }
        cout << "Total Duration for N = " << N << ": " << totalDuration << " seconds." << endl;
    }

    return 0;
}
