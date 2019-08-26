#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

template <typename T>
using v1d = vector<T>;
template <typename T>
using v2d = vector<vector<T>>;
template <typename T>
using v3d = vector<vector<vector<T>>>;

struct Dataset
{
    v2d<int> data;
    v1d<int> index;
    v1d<int> useful;
};

Dataset ReadDataset(string FileName);
void PrintDataset(Dataset dataset);
int getNode(Dataset dataset);
int getEdges(Dataset dataset);
v1d<string> split(string str, string pattern);
void CreateSample(Dataset dataset, double Rate);

int main(int argc, char **argv)
{
    if (argc < 3)
        exit;
    string filename = argv[1];
    double propagation_probability = atof(argv[2]);

    Dataset dataset = ReadDataset(filename);
    CreateSample(dataset, propagation_probability);
    return 0;
}

Dataset ReadDataset(string FileName)
{
    ifstream myfile;
    myfile.open(FileName.c_str());
    Dataset dataset;
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
void PrintDataset(Dataset dataset)
{
    for (int i = 0; i < dataset.data.size(); i++)
    {
        for (int j = 0; j < dataset.index.size(); j++)
            if (dataset.index[j] == i)
                cout << j << " -> ";
        for (int j = 0; j < dataset.data[i].size(); j++)
        {
            cout << dataset.data[i][j] << ", ";
        }
        cout << endl;
    }
}
int getNode(Dataset dataset)
{
    return dataset.data.size();
}
int getEdges(Dataset dataset)
{
    int Edges = 0;
    for (int i = 0; i < dataset.data.size(); i++)
        Edges += dataset.data[i].size();
    return Edges;
}
v1d<string> split(string str, string pattern)
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
void CreateSample(Dataset dataset, double Rate)
{
    for (int i = 0; i < dataset.data.size(); i++)
        for (int j = 0; j < dataset.data[i].size();)
            if (static_cast<double>(rand()) / RAND_MAX > Rate)
                dataset.data[i].erase(dataset.data[i].begin() + j);
            else
                j++;

    for (int i = 0; i < dataset.data.size(); i++)
        for (int j = 0; j < dataset.data[i].size(); j++)
            cout << i << "\t" << dataset.data[i][j] << endl;
}