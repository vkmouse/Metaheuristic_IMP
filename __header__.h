#ifndef _HEADER_H_
#define _HEADER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using std::cout;
using std::endl;
using std::string;

template <typename T>
using v1d = std::vector<T>;
template <typename T>
using v2d = std::vector<std::vector<T>>;
template <typename T>
using v3d = std::vector<std::vector<std::vector<T>>>;
template <typename T>
using v4d = std::vector<std::vector<std::vector<std::vector<T>>>>;

class search_algorithm
{
public:
    struct Dataset
    {
        // The element is edges from a node to all itself node. It is not sorted.
        v2d<int> data;

        // Record the index in the data of the node.
        // For example, index[3] = 6 means that data[6] record all the edges of the "node 3".
        v1d<int> index;
        v1d<int> useful;
    };

    // Parameter for every algorithm
    string dataset_path;
    int iteration;
    int MaxOV;
    int Numrun;
    int NumSeed;

    struct Dataset mDataset;
    v1d<int> BestSeedset;
    int MaxObjectValue;
    int ObjectValueCount = 0;

    // method
    virtual void run() = 0;
    void run_all();
    void init(string dataset_path, int iteration, int MaxOV, int PerOV, int Numrun, int NumSeed);
    v1d<int> CreateSeedset(v1d<int> useful, int NumSeed);
    v1d<int> transition(v1d<int> useful, v1d<int> Seedset);
    int ObjectValue(v1d<int> Seedset);
    bool determination(v1d<int> Seedset, int ObjectValue);
    int random(int max);
    int random(int min, int max);
    bool checkRepeatElement(v1d<int> vec, int element);
    void crossoverTwoSolution(v1d<int> &sol1, v1d<int> &sol2, double changeRate);
    // int ObjectValue(v1d<int> Seedset, v1d<int> &useful, int MaxObjectValue);
    // int ObjectValue(v1d<int> Seedset, int times, int MaxObjectValue);

private:
    int PerOV;

    struct Dataset ReadDataset(string FileName);
    v1d<string> split(string str, string pattern);
};
class hc : public search_algorithm
{
public:
    virtual void run();

private:
    v1d<int> mSeedset;
    int mObjectValue;
    void init();
};
class sa : public search_algorithm
{
public:
    sa(int neighbor, double maxTemperature, double minTemperature);
    virtual void run();

private:
    int neighbor;
    double maxTemperature;
    double minTemperature;
    double nowTemperature;
    v1d<int> mSeedset;
    int mObjectValue;
    void init();
    void createNeighbor();
};
class ga : public search_algorithm
{
public:
    ga(int NumCms, double Pc, double Pm);
    virtual void run();

private:
    int NumCms;
    double Pc;
    double Pm;
    v2d<int> CmsSet;
    v1d<int> CmsObjectValue;

    void init();
    void selection();
    void mutation();
    void fitness();
    void crossover(double changeRate);
};
class se : public search_algorithm
{
public:
    se(int num_searcher, int num_region, int num_sample, int num_player);
    virtual void run();

private:
    int num_searcher;
    int num_region;
    int num_sample;
    int num_player;

    // 0.3 search algorithm
    v2d<int> searcher_sol;    // [searcher, num_bit_sol]
    v3d<int> sample_sol;      // [region, sample, num_bit_sol]
    v2d<int> sample_sol_best; // [region, num_bit_sol]
    v4d<int> sampleV_sol;     // [searcher, region, sample, num_bit_sol]

    v1d<double> searcher_sol_fitness;
    v2d<double> sample_sol_fitness;
    v1d<double> sample_sol_best_fitness;
    v3d<double> sampleV_sol_fitness;

    v1d<double> region_it;
    v1d<double> region_hl;

    v2d<double> expected_value;
    v1d<double> T_j;
    v2d<double> V_ij;
    v1d<double> M_j;

    // IM
    v2d<int> useful;

    void init();
    void resource_arrangement();
    void vision_search();
    void transit();
    void compute_expected_value();
    int evaluate_fitness(const v1d<int> &sol);
    void vision_selection(int player);
    void marketing_survey();
};
class cro : public search_algorithm
{
public:
    cro(int n1, int n2, double Pi, double Fb, int K, double Fa, double Fd, double Pd);
    virtual void run();

private:
    // input parameter
    int n1;
    int n2;
    double Pi;
    double Fb;
    int K;
    double Fa;
    double Fd;
    double Pd;
    //
    struct Reef
    {
        v1d<int> sol;
        int ObjectValue;
        int n1;
        int n2;
    };
    v1d<struct Reef> ReefSet;
    v2d<int> Map;
    v2d<int> childReef;
    v1d<int> rank;

    void init();
    v2d<int> transitionCRO();
    void LarvaeSetting(); //幼蟲設置
    void ReefSorting(v1d<struct Reef> ReefSet, v1d<int> &rank);
    void AsexualReproduction(v1d<struct Reef> &ReefSet, v1d<int> rank, v2d<int> &Map, double Fa);
    void Depredation(v1d<struct Reef> &ReefSet, v1d<int> rank, v2d<int> &Map, double Fd, double Pd);
};
class sasp : public search_algorithm
{
public:
    sasp(int numPartition, int neighbor, double maxTemperature, double minTemperature);
    virtual void run();

private:
    int neighbor;
    double maxTemperature;
    double minTemperature;
    double nowTemperature;
    v1d<int> mSeedset;
    int mObjectValue;
    int numPartition;      // sasp
    v2d<int> useful;       // sasp
    int numOVPerPartition; // sasp
    void init();
    void createNeighbor();
    void createNeighbor(int partition);
};
#endif