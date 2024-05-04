#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <omp.h>

using namespace std;

const string targetString = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce vestibulum justo eu libero dapibus, vel convallis nunc malesuada. Integer nec Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce vestibulum justo eu libero dapibus quam ac nisi dictum sollicitudin. Proin at lectus in odioim"; 

// Function to generate population
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

int main() {
    srand(static_cast<unsigned>(time(0)));

    vector<int> populationSizes = {100000, 500000, 1000000, 5000000, 10000000};
    vector<int> numThreads = {1, 2, 4, 8, 16, 32};

    for (int N : populationSizes) {
        double serialTime = 0.0;
        if (numThreads[0] == 1) {
            // Run the serial version only once if the first thread count is 1
            serialTime = generatePopulation(N);
            cout << "Serial time for N = " << N << ": " << serialTime << " seconds." << endl;
        }

        vector<double> speedUps(numThreads.size());

        for (size_t i = 0; i < numThreads.size(); ++i) {
            int p = numThreads[i];
            omp_set_num_threads(p);
            double parallelTime = generatePopulation(N);
            
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
