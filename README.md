## Metaheuristic algorithms for the influence maximization problem in the social network
This code implements a greedy algorithm, four metaheuristic algorithms, and a improvement metaheuristic algorithm to solve the influence maximization problem (IMP) in the social network. Four metaheuristic algorithms include simulated annealing (SA), genetic algorithm (GA), coral reefs optimization (CRO), and search economics (SE). An improvement metaheuristic algorithm is simulated annealing with search partition (SASP). And the greedy algorithm is hill climbing (HC). Two important algorithms are SE and SASP.

### Introduction
[Search economics (SE)](https://doi.org/10.1109/SMC.2015.447) is a novel metaheuristic algorithm proposed in 2015. The two main differences between SE and traditional metaheuristic are dividing the search space and investing computation resource depending on the potential of each subspace. The simulated results show that the [SEIM](https://doi.org/10.1016/j.future.2018.08.033) is able to find a better solution than the HC, and SA. [SASP](https://doi.org/10.1016/j.procs.2017.08.306) is based on a search space partitioning machism to improve result of SA. Search space partition divides the solution space into many regions, and then search in the region or in all solution space in different period.


### Compile
Install GCC 4.7 or newer version in the [link](https://sourceforge.net/projects/mingw-w64/files/). Execute the command "make" in the path of the Makefile to compile the code.

    make

### Usage
Six parameters are required for all algorithms to execute the program. Each algorithm requires specific parameters for itself.
1. Dataset path
2. Number of iterations
3. Number of evaluations
4. Number of evaluations per output
5. Number of runs
6. Number of seeds

#### Hill climbing
No other parameter is required to execute the HC. For example, dataset path is "dataset.txt", number of iterations is 10000, number of evaluations is 10000, number of evaluations per output is 400, number of runs is 2, and number of seeds is 200.

    main.exe hc dataset.txt 10000 10000 400 2 200

#### Simulated annealing
Three parameters are required to execute the SA.
1. Number of neighbors
2. Initial temperature
3. Minimum temperature

For example, number of neighbors is 7, initial temperature is 10, and minimum temperature is 0.01.
 
    main.exe sa dataset.txt 10000 10000 400 2 200 7 10 0.01

#### Genetic algorithm
Three parameters are required to execute the GA.
1. Number of chromsomes
2. Crossover rate
3. Mutation rate

For example, number of chromsomes is 20, crossover rate is 0.6, and mutation rate is 0.9.
 
    main.exe ga dataset.txt 10000 10000 400 2 200 20 0.6 0.9

#### Coral reefs optimization
Eight parameters are required to execute the CRO.
1. The width of the grid (n1)
2. The length of the grid (n2)
3. The probability of the initial production
4. The fraction of the broadcast spawners
5. Number of steps for the reef formation phase
6. The fraction of the asexual reproduction
7. The fraction of the depredation 
8. The probability of the depredation

For example, n1 and n2 are 4, the probability of the initial production is 0.8, the fraction of the broadcast spawners is 0.1, number of steps for the reef formation phase is 3, the fraction of the asexual reproduction is 0.01, the fraction of the depredation is 0.1, and the probability of the depredation is 0.1

    main.exe cro dataset.txt 10000 10000 400 2 200 4 4 0.8 0.1 3 0.01 0.1 0.1

#### Search economics
Four parameters are required to execute the SE.
1. Number of searchers
2. Number of regions
3. Number of samples
4. Number of players

For example, number of searchers is 4, number of regions is 4, number of samples is 2, and number of players is 3.
 
    main.exe se dataset.txt 10000 10000 400 2 200 4 4 2 3

#### Simulated annealing with search partition
Four parameters are required to execute the SASP.
1. Number of partitions
2. Number of neighbors
3. Initial temperature
4. Minimum temperature

For example, number of partitions is 4, number of neighbors is 7, initial temperature is 10, and minimum temperature is 0.01.
 
    main.exe sasp dataset.txt 10000 10000 400 2 200 4 7 10 0.01
