#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include <algorithm>
#include <omp.h>

using namespace std;

const string targetString = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce vestibulum justo eu libero dapibus, vel convallis nunc malesuada.";

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

double measureFitnessCalculationTime(int populationSize) {
    vector<string> population(populationSize);
    vector<int> fitnessValues(populationSize);
    unsigned seed = static_cast<unsigned>(time(0));
    mt19937 rng(seed);

    // Populate the initial population
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
        fitnessValues[i] = calculateFitness(population[i]);
    }
    return omp_get_wtime() - startFit;
}

int main() {
    vector<int> populationSizes = {100000, 500000, 1000000, 5000000, 10000000};
    vector<int> numThreads = {1, 2, 4, 8, 16, 32};

    for (int N : populationSizes) {
        double serialTime = 0.0;
        if (numThreads[0] == 1) {
            // Run the serial version only once if the first thread count is 1
            serialTime = measureFitnessCalculationTime(N);
            cout << "Serial time for N = " << N << ": " << serialTime << " seconds." << endl;
        }

        vector<double> speedUps(numThreads.size());

        for (size_t i = 0; i < numThreads.size(); ++i) {
            int p = numThreads[i];
            omp_set_num_threads(p);
            double parallelTime = measureFitnessCalculationTime(N);

            if (p == 1 && numThreads[0] != 1) {  // Recapture serial time if not already done
                serialTime = parallelTime;
                cout << "Serial time for N = " << N << ": " << serialTime << " seconds." << endl;
            }

            double speedUp = serialTime / parallelTime;
            speedUps[i] = speedUp;
            cout << "Parallel time for N = " << N << ", p = " << p << ": " << parallelTime << " seconds, Speed Up: " << speedUp << endl;
        }

        // Print speed ups as an array
        cout << "Speed Ups for N = " << N << ": [";
        for (size_t j = 0; j < speedUps.size(); ++j) {
            cout << speedUps[j] << (j < speedUps.size() - 1 ? ", " : "");
        }
        cout << "]" << endl;
    }

    return 0;
}
