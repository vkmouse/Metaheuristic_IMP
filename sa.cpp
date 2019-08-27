#include "__header__.h"
sa::sa(int neighbor, double maxTemperature, double minTemperature)
{
    this->neighbor = neighbor;
    this->maxTemperature = maxTemperature;
    this->minTemperature = minTemperature;
}
void sa::run()
{
    init();
    for (int iter = 0; iter < iteration && ObjectValueCount < MaxOV; iter++)
    {
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
void sa::init()
{
    mSeedset = CreateSeedset(mDataset.useful, NumSeed);
    mObjectValue = ObjectValue(mSeedset);
    determination(mSeedset, mObjectValue);
    nowTemperature = maxTemperature;
}
void sa::createNeighbor()
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