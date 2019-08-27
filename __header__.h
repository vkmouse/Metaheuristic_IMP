#ifndef _HEADER_H_
#define _HEADER_H_

#include <iostream>
#include <fstream>
#include <vector>

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
    int ObjectValue(v1d<int> Seedset, int MaxObjectValue);
    bool determination(v1d<int> Seedset, int ObjectValue);
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
    double mObjectValue;
    void init();
};
#endif