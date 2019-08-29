#include "__header__.h"
#include <algorithm>

ga::ga(int NumCms, double Pc, double Pm)
{
    this->NumCms = NumCms;
    this->Pc = Pc;
    this->Pm = Pm;
}
void ga::run()
{
    init();
    for (int iter = 0; iter < iteration && ObjectValueCount < MaxOV; iter++)
    {
        selection();
        crossover(0.005);
        mutation();
        fitness();
    }
}
void ga::init()
{
    CmsSet.resize(NumCms);
    CmsObjectValue.resize(NumCms);
    for (int i = 0; i < NumCms; i++)
    {
        CmsSet[i] = CreateSeedset(mDataset.useful, NumSeed);
        CmsObjectValue[i] = ObjectValue(CmsSet[i]);
        determination(CmsSet[i], CmsObjectValue[i]);
    }
}
void ga::selection()
{
    double total = 0;
    v1d<double> RouletteWheel(NumCms, 0.0);
    v2d<int> CmsSettmp;
    CmsSettmp.resize(NumCms);

    for (int i = 0; i < NumCms; i++)
        total = total + CmsObjectValue[i];
    RouletteWheel[0] = CmsObjectValue[0] / total;
    for (int i = 1; i < NumCms; i++)
        RouletteWheel[i] = CmsObjectValue[i] / total + RouletteWheel[i - 1];

    for (int i = 0; i < NumCms; i++)
    {
        double f = static_cast<double>(rand()) / RAND_MAX;
        if (f <= RouletteWheel[0])
            CmsSettmp[i] = CmsSet[0];
        else if (f >= 1.0)
            CmsSettmp[i] = CmsSet.back();
        else
        {
            for (int j = 1; j < RouletteWheel.size(); j++)
                if (RouletteWheel[j - 1] <= f && f < RouletteWheel[j])
                {
                    CmsSettmp[i] = CmsSet[j];
                    j = RouletteWheel.size();
                }
        }
    }
    CmsSet = CmsSettmp;
    CmsSet[0] = BestSeedset;
}
void ga::mutation()
{
    for (int i = 0; i < NumCms; i++)
    {
        double f = static_cast<double>(rand()) / RAND_MAX;
        if (Pm > f)
            CmsSet[i] = transition(mDataset.useful, CmsSet[i]);
    }
}
void ga::fitness()
{
    for (int i = 0; i < NumCms; i++)
    {
        CmsObjectValue[i] = ObjectValue(CmsSet[i]);
        determination(CmsSet[i], CmsObjectValue[i]);
    }
}
void ga::crossover(double changeRate)
{
    for (int i = 0; i < NumCms; i = i + 2)
    {
        double f = static_cast<double>(rand()) / RAND_MAX;
        if (Pc > f)
        {
            // generate crossover mask
            v1d<int> index(NumCms);
            for (int j = 0; j < NumCms; j++)
            {
                index[j] = j;
            }
            std::random_shuffle(index.begin(), index.end());
            for (int j = 0; j < NumCms * changeRate; j++)
            {
                int temp = CmsSet[i][index[j]];
                CmsSet[i][index[j]] = CmsSet[i + 1][index[j]];
                CmsSet[i + 1][index[j]] = temp;
            }
        }
    }
}