#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <omp.h>

using namespace std;

// 7.013

const string targetString = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce vestibulum justo eu libero dapibus, vel convallis nunc malesuada. Integer nec Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce vestibulum justo eu libero dapibus quam ac nisi dictum sollicitudin. Proin at lectus in odioim"; 

int main() {
    srand(static_cast<unsigned>(time(0)));

    const int populationSize = 10000000;
    const double mutationRate = 0.01;
    
    double start_pop = omp_get_wtime();
    vector<string> population(populationSize);
    for (string& individual : population) {
        for (size_t i = 0; i < targetString.size(); ++i) {
            individual += static_cast<char>(rand() % 128);
        }
    }
    double duration_pop = omp_get_wtime() - start_pop;
 
    cout << " Duration of Population: " << duration_pop << "\n";
}