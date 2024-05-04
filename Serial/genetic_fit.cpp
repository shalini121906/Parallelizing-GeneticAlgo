#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <omp.h>

using namespace std;

// 1.465

const string targetString = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce vestibulum justo eu libero dapibus, vel convallis nunc malesuada. Integer nec Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce vestibulum justo eu libero dapibus quam ac nisi dictum sollicitudin. Proin at lectus in odioim";
int calculateFitness(const string& individual) {
    int fitness = 0;
    
    for (size_t i = 0; i < targetString.size(); ++i) {
        if (individual[i] != targetString[i]) {
            fitness++;
        }
    }
    return fitness;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    const int populationSize = 10000000;
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
    cout << " Duration of fitness: " << duration_fit << "\n";
    
    return 0;
}