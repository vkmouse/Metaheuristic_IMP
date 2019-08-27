#include "__header__.h"
void hc::run()
{
    init();
    for (int iter = 0; iter < iteration && ObjectValueCount < MaxOV; iter++)
    {
        mSeedset = transition(mDataset.useful, BestSeedset);
        mObjectValue = ObjectValue(mSeedset, MaxObjectValue);
        determination(mSeedset, mObjectValue);
    }
}
void hc::init()
{
    mSeedset = CreateSeedset(mDataset.useful, NumSeed);
    mObjectValue = ObjectValue(mSeedset, MaxObjectValue);
    determination(mSeedset, mObjectValue);
}