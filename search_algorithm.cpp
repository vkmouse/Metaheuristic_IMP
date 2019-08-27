#include "__header__.h"
#include <time.h>

using std::ifstream;
using std::ios;

#define inactive 0
#define active 1
#define newactive 2

void search_algorithm::run_all()
{
    srand(time(0));
    for (int now_run = 0; now_run < Numrun; now_run++)
    {
        ObjectValueCount = 0;
        MaxObjectValue = -1;
        run();
    }
}
void search_algorithm::init(string dataset_path, int iteration, int MaxOV, int PerOV, int Numrun, int NumSeed)
{
    this->dataset_path = dataset_path;
    this->iteration = iteration;
    this->MaxOV = MaxOV;
    this->PerOV = PerOV;
    this->Numrun = Numrun;
    this->NumSeed = NumSeed;

    mDataset = ReadDataset(dataset_path);
}
v1d<int> search_algorithm::CreateSeedset(v1d<int> useful, int NumSeed)
{
    v1d<int> Seedset;

    for (int i = 0; i < NumSeed; i++)
    {
        int tmp = rand() % useful.size();
        Seedset.push_back(useful[tmp]);
        useful.erase(useful.begin() + tmp);
    }

    return Seedset;
}
v1d<int> search_algorithm::transition(v1d<int> useful, v1d<int> Seedset)
{
    int tmp;
    do
    {
        tmp = useful[rand() % (useful.size())];
        for (int i = 0; i < Seedset.size(); i++)
            if (tmp == Seedset[i])
                tmp = -1;
    } while (tmp == -1);

    Seedset[rand() % (Seedset.size())] = tmp;
    return Seedset;
}
int search_algorithm::ObjectValue(v1d<int> Seedset, int MaxObjectValue)
{
    v1d<int> Status;
    v1d<int> newActiveSeedset;
    int Seed, subSeed, SeedIndex, subSeedIndex; //Seed 母種子 、 subSeed 子種子 ->  13:1,2,3 ，13是Seed、1,2,3是 子種子
    int numActive = 0;

    Status.assign(mDataset.data.size(), inactive);

    for (int i = 0; i < Seedset.size(); i++)
    {
        Seed = Seedset[i];
        SeedIndex = mDataset.index[Seed];
        if (Status[SeedIndex] == inactive)
        {
            newActiveSeedset.push_back(Seed);
            Status[SeedIndex] = newactive;
        }
    }
    while (newActiveSeedset.size() != numActive)
    {
        SeedIndex = mDataset.index[newActiveSeedset[numActive]];
        int numEdges = mDataset.data[SeedIndex].size();
        for (int j = 0; j < numEdges; j++)
        {
            subSeed = mDataset.data[SeedIndex][j];
            subSeedIndex = mDataset.index[subSeed];
            if (Status[subSeedIndex] == inactive)
            {
                newActiveSeedset.push_back(subSeed);
                Status[subSeedIndex] = newactive;
            }
        }
        Status[SeedIndex] = active;
        numActive++;
    }

    ObjectValueCount++;

    if (ObjectValueCount == 1)
        cout << ObjectValueCount << "," << numActive << endl;
    if (ObjectValueCount % PerOV == 0)
        cout << ObjectValueCount << "," << MaxObjectValue << endl;

    return numActive;
}
bool search_algorithm::determination(v1d<int> Seedset, int ObjectValue)
{
    if (ObjectValue > MaxObjectValue || MaxObjectValue == -1)
    {
        MaxObjectValue = ObjectValue;
        BestSeedset = Seedset;
        return true;
    }
    return false;
}
struct search_algorithm::Dataset search_algorithm::ReadDataset(string FileName)
{
    ifstream myfile;
    myfile.open(FileName.c_str());
    struct Dataset dataset;
    v1d<int> tmp;

    string line;
    int p1, p2;

    while (getline(myfile, line))
    {
        if (line[0] == '#')
            continue;
        v1d<string> str = split(line, "\t");
        p1 = atoi(str[0].c_str());
        p2 = atoi(str[1].c_str());

        int max = p1;
        if (p2 > max)
            max = p2;

        while (dataset.index.size() < max + 1)
            dataset.index.push_back(-1);

        if (dataset.index[p1] == -1)
        {
            dataset.index[p1] = dataset.data.size();
            dataset.data.push_back(tmp);
        }
        dataset.data[dataset.index[p1]].push_back(p2);

        if (dataset.index[p2] == -1)
        {
            dataset.index[p2] = dataset.data.size();
            dataset.data.push_back(tmp);
        }
    }

    for (int i = 0; i < dataset.index.size(); i++)
        if (dataset.index[i] != -1)
            dataset.useful.push_back(i);

    return dataset;
}
v1d<string> search_algorithm::split(string str, string pattern)
{
    int pos;
    v1d<string> result;
    int size = str.size();

    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos == -1)
        {
            result.push_back(str.substr(i));
            i = size;
        }
        else if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}
