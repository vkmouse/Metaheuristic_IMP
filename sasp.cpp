#include "__header__.h"
sasp::sasp(int numPartition, int neighbor, double maxTemperature, double minTemperature)
{
    this->numPartition = numPartition; // sasp
    this->neighbor = neighbor;
    this->maxTemperature = maxTemperature;
    this->minTemperature = minTemperature;
}
void sasp::run()
{
    init();
    for (int iter = 0; iter < iteration && ObjectValueCount < MaxOV; iter++)
    {
        int partition = ObjectValueCount / (double)numOVPerPartition;
        if (numPartition > partition)
            createNeighbor(partition); // sasp
        else
            createNeighbor();
        determination(mSeedset, mObjectValue);
        double r = 0.0 + rand() / static_cast<double>(RAND_MAX);
        double p = exp(double(mObjectValue - MaxObjectValue) / nowTemperature);
        if (r >= p)
        {
            mObjectValue = MaxObjectValue;
            mSeedset = BestSeedset;
        }
        if (nowTemperature > minTemperature)
            nowTemperature *= 0.999;
    }
}
void sasp::init()
{
    mSeedset = CreateSeedset(mDataset.useful, NumSeed);
    mObjectValue = ObjectValue(mSeedset);
    determination(mSeedset, mObjectValue);
    nowTemperature = maxTemperature;
    // sasp
    numOVPerPartition = MaxOV / 2.0 / numPartition;
    useful.resize(numPartition);
    for (int j = 0; j < numPartition; j++)
        for (int k = j; k < mDataset.useful.size(); k = k + numPartition)
            useful[j].push_back(mDataset.useful[k]);
}
void sasp::createNeighbor()
{
    v1d<int> cSeedset, BestcSeedset;
    int cObjectValue, BestcObjectValue;

    BestcSeedset = transition(mDataset.useful, mSeedset);
    BestcObjectValue = ObjectValue(BestcSeedset);

    for (int i = 0; i < neighbor - 1; i++)
    {
        cSeedset = transition(mDataset.useful, mSeedset);
        cObjectValue = ObjectValue(cSeedset);
        if (cObjectValue > BestcObjectValue)
        {
            BestcSeedset = cSeedset;
            BestcObjectValue = cObjectValue;
        }
    }

    mSeedset = BestcSeedset;
    mObjectValue = BestcObjectValue;
}
void sasp::createNeighbor(int partition)
{
    v1d<int> cSeedset, BestcSeedset;
    int cObjectValue, BestcObjectValue;

    BestcSeedset = transition(mDataset.useful, mSeedset);
    BestcObjectValue = ObjectValue(BestcSeedset);

    for (int i = 0; i < neighbor - 1; i++)
    {
        cSeedset = transition(useful[partition], mSeedset); // sasp
        cObjectValue = ObjectValue(cSeedset);
        if (cObjectValue > BestcObjectValue)
        {
            BestcSeedset = cSeedset;
            BestcObjectValue = cObjectValue;
        }
    }

    mSeedset = BestcSeedset;
    mObjectValue = BestcObjectValue;
}