# Parallelizing-GeneticAlgo
﻿


The problem at hand is a simple string matching genetic algorithm. The objective is to evolve a population of strings to match a target string. This involves generating an initial population, evaluating the fitness of each individual in the population, selecting parents for reproduction based on their fitness, performing crossover and mutation operations, and repeating this process over multiple generations while parallelizing each generations function to achieve maximum compute speed to perform the string matching.

**Approach:**

Our approach involves parallelizing certain functions within the genetic algorithm to improve performance, particularly as the size of the problem (N) increases. Since the generation of entire populations cannot be parallelized due to dependencies between individuals, we focus on parallelizing individual functions involved in the generation process.

We utilize OpenMP, a popular API for parallel programming, to parallelize these functions. OpenMP provides simple and efficient mechanisms for parallelizing loops, which allows us to distribute the workload across multiple processing units effectively.

**Performance Metrics:**

**Problem size (N) = [100000, 500000, 1000000, 5000000, 10000000]**

**Processing elements (p) = [1, 2, 4, 8, 16, 32]** 

**Speed up Inferences:** 

![Picture1](https://github.com/shalini121906/Parallelizing-GeneticAlgo/assets/116372155/deec82a1-7b81-4900-819a-89c59f2dd4cc)


![Picture2](https://github.com/shalini121906/Parallelizing-GeneticAlgo/assets/116372155/97f321a3-5e40-4378-8005-1613cd037031)

- **Population Initialization:** As the problem size increases, the parallelization of population initialization leads to significant speedup, especially when utilizing multiple processing units. However, the speedup tends to plateau as the number of processing units increases beyond a certain point.

![Picture3](https://github.com/shalini121906/Parallelizing-GeneticAlgo/assets/116372155/b1fee9ff-07a1-4093-9cf4-947af275a991)

![Picture4](https://github.com/shalini121906/Parallelizing-GeneticAlgo/assets/116372155/2d509cee-96b6-4549-8409-f8d3e8fdcfba)

- **Fitness Calculation:** Parallelizing the fitness calculation also results in substantial speedup, particularly for larger problem sizes. However, the speedup may vary depending on the number of processing units and the characteristics of the fitness function. We can also notice that the speed up reaches a maximum at 8 processing unit before dipping down. Therefore we can conclude that fitness function can achieve sufficient speed up around 10-15 processing units.


![Picture5](https://github.com/shalini121906/Parallelizing-GeneticAlgo/assets/116372155/a6723640-d527-4a91-871e-95c6f2df211a)

![Picture6](https://github.com/shalini121906/Parallelizing-GeneticAlgo/assets/116372155/08e9389d-bd81-4471-ab64-e1acc9c7a7ab)


- **Crossover and Mutation:** Parallelizing crossover and mutation operations yields moderate speedup, with variations based on the problem size and the number of processing units. These operations may not exhibit as significant speedup as population initialization and fitness calculation, but they still contribute to overall performance improvement.






**Parallel efficiency Inferences:**

![Picture7](https://github.com/shalini121906/Parallelizing-GeneticAlgo/assets/116372155/87a2aacc-397c-4b3c-b218-b58dd3ce072d)


**Population Initialization:**

The graph demonstrates the parallel efficiency of the population initialization process. As the number of processing units increases, we observe a gradual decrease in parallel efficiency across different problem sizes. Initially, with a small problem size (N), the parallel efficiency is relatively high, indicating efficient utilization of resources. However, as the problem size grows, the parallel efficiency decreases. 

![Picture8](https://github.com/shalini121906/Parallelizing-GeneticAlgo/assets/116372155/e567cecc-bae8-4801-9b08-78896369aea0)


**Fitness Calculation:**

In the graph depicting the parallel efficiency of fitness calculation, we notice a similar pattern to that of population initialization. Initially, with smaller problem sizes, the parallel efficiency is relatively high. However, as the problem size increases, the parallel efficiency diminishes, indicating that the scalability of fitness calculation becomes more challenging with larger datasets. Despite this decrease, the parallel efficiency remains above a certain threshold, indicating that parallelization still provides benefits
![Picture9](https://github.com/shalini121906/Parallelizing-GeneticAlgo/assets/116372155/bc736c10-b120-40b5-a5c1-364ce116604f)


**Crossover and Mutation:**

The parallel efficiency graph for crossover and mutation reveals a more consistent pattern compared to the other functions. Across different problem sizes, the parallel efficiency remains relatively stable, with slight fluctuations observed. This suggests that the crossover and mutation process is inherently more amenable to parallelization compared to population initialization and fitness calculation. However, similar to other functions, there is a slight decrease in parallel efficiency as the problem size increases

**Serial vs Parallel:**

![Picture10](https://github.com/shalini121906/Parallelizing-GeneticAlgo/assets/116372155/e10a27d7-1b4a-4833-8c54-5b27bbcaa8aa)


Although parallelizing may not fully utilize computer resources efficiently, it significantly reduces the runtime compared to serial execution. As evident from the plot, parallel execution (red line) consistently outperforms serial execution (blue line) across different population sizes. This demonstrates the effectiveness of parallelization in reducing overall runtime, especially for larger problem sizes.

