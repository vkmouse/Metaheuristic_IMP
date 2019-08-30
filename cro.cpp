#include "__header__.h"
cro::cro(int n1, int n2, double Pi, double Fb, int K, double Fa, double Fd, double Pd)
{
    this->n1 = n1;
    this->n2 = n2;
    this->Pi = Pi;
    this->Fb = Fb;
    this->K = K;
    this->Fa = Fa;
    this->Fd = Fd;
    this->Pd = Pd;
}
void cro::run()
{
    init();
    for (int iter = 0; iter < iteration && ObjectValueCount < MaxOV; iter++)
    {
        childReef = transitionCRO();
        LarvaeSetting(); //幼蟲設置
        ReefSorting(ReefSet, rank);
        AsexualReproduction(ReefSet, rank, Map, Fa); //無性生殖
        Depredation(ReefSet, rank, Map, Fd, Pd);     //掠奪
    }
}
void cro::init()
{
    //Map
    Map.assign(n1, v1d<int>(n2, -1));
    //ReefSet
    ReefSet.clear();
    double f;
    for (int i = 0; i < Map.size(); i++)
        for (int j = 0; j < Map[i].size(); j++)
        {
            f = static_cast<double>(rand()) / RAND_MAX;
            if (f < Pi)
            {
                struct Reef reef;
                reef.n1 = i;
                reef.n2 = j;
                //transition
                reef.sol = CreateSeedset(mDataset.useful, NumSeed);
                //evaluation
                reef.ObjectValue = ObjectValue(reef.sol);
                //determination
                determination(reef.sol, reef.ObjectValue);
                ReefSet.push_back(reef);

                Map[i][j] = ReefSet.size() - 1;
            }
        }
}
v2d<int> cro::transitionCRO()
{
    v2d<int> BSReef, BReef;
    int numBS = ReefSet.size() * Fb;

    for (int i = 0; i < numBS; i++)
    {
        int f = rand() % ReefSet.size();
        BSReef.push_back(ReefSet[f].sol);
        ReefSet.erase(ReefSet.begin() + f);
    }
    for (int i = 0; i < ReefSet.size(); i++)
        BReef.push_back(ReefSet[i].sol);
    // transition
    if (BSReef.size() > 2)
        for (int i = 0; i < BSReef.size() - 1; i++) // 散播式交配
        {
            crossoverTwoSolution(BSReef[i], BSReef[i + 1], 0.001);
            BSReef.erase(BSReef.begin() + i + 1);
        }
    for (int i = 0; i < BReef.size(); i++) // 自體受精
        BReef[i] = transition(mDataset.useful, BReef[i]);

    BSReef.insert(BSReef.end(), BReef.begin(), BReef.end());

    return BSReef;
}
void cro::LarvaeSetting()
{
    //evaluation
    v1d<int> childObj(childReef.size(), 0);
    for (int i = 0; i < childReef.size(); i++)
    {
        childObj[i] = ObjectValue(childReef[i]);
        determination(childReef[i], childObj[i]);
    }
    for (int i = 0; i < childReef.size(); i++)
        for (int j = 0; j < K; j++)
        {
            int n1 = rand() % Map.size();
            int n2 = rand() % Map[n1].size();

            int tmp = Map[n1][n2];
            if (tmp == -1) //為空礁
            {
                struct Reef reef;
                reef.n1 = n1;
                reef.n2 = n2;

                reef.sol = childReef[i];
                reef.ObjectValue = childObj[i];
                ReefSet.push_back(reef);

                Map[n1][n2] = ReefSet.size() - 1;
            }
            else
            {
                if (childObj[i] > ReefSet[tmp].ObjectValue)
                {
                    ReefSet[tmp].sol = childReef[i];
                    ReefSet[tmp].ObjectValue = childObj[i];
                    j = K;
                }
                j++;
            }
        }
}
void cro::ReefSorting(v1d<struct Reef> ReefSet, v1d<int> &rank)
{
    if (rank.size() != ReefSet.size())
    {
        v1d<int> count(ReefSet.size());
        v1d<int> isNotUsed;

        rank.resize(ReefSet.size());
        for (int i = 0; i < ReefSet.size(); i++)
            if (rank[i] >= 0 && rank[i] < ReefSet.size()) //數字在範圍內, count++
                count[rank[i]]++;

        for (int i = 0; i < ReefSet.size(); i++)
            if (count[i] == 0) //count==0, 放入未被使用
                isNotUsed.push_back(i);

        for (int i = 0; i < ReefSet.size(); i++)
            if (rank[i] < 0 || rank[i] >= ReefSet.size()) //數字不在範圍內
            {
                rank[i] = isNotUsed[isNotUsed.size() - 1];
                isNotUsed.pop_back();
            }
            else if (count[rank[i]] > 1) //被使用過超過1次,未被使用的元素覆蓋此元素
            {
                count[rank[i]]--;
                rank[i] = isNotUsed[isNotUsed.size() - 1];
                isNotUsed.pop_back();
            }
    }
    //Sorting
    for (int i = 0; i < rank.size(); i++)
        for (int j = 1; j < rank.size() - i; j++)
            if (ReefSet[rank[j - 1]].ObjectValue < ReefSet[rank[j]].ObjectValue)
            {
                int tmp = rank[j - 1];
                rank[j - 1] = rank[j];
                rank[j] = tmp;
            }
}
void cro::AsexualReproduction(v1d<struct Reef> &ReefSet, v1d<int> rank, v2d<int> &Map, double Fa) //無性生殖
{
    int numA = rank.size() * Fa;

    for (int i = 0; i < numA; i++)
    {
        int tmp = rank[i];
        for (int n1 = 0; n1 < Map.size(); n1++)
            for (int n2 = 0; n2 < Map[n1].size(); n2++)
                if (Map[n1][n2] == -1)
                {
                    struct Reef reef;
                    reef.n1 = n1;
                    reef.n2 = n2;

                    reef.sol = ReefSet[tmp].sol;
                    reef.ObjectValue = ReefSet[tmp].ObjectValue;
                    ReefSet.push_back(reef);

                    Map[n1][n2] = ReefSet.size() - 1;

                    n1 = Map.size() - 1;
                    n2 = Map[n1].size() - 1;
                }
    }
}
void cro::Depredation(v1d<struct Reef> &ReefSet, v1d<int> rank, v2d<int> &Map, double Fd, double Pd) //掠奪
{
    int numD = ReefSet.size() * Fd;
    v1d<int> reefD;
    double f;

    for (int i = 0; i < numD; i++) //紀錄要刪除的元素
    {
        f = static_cast<double>(rand()) / RAND_MAX;
        if (f < Pd)
        {
            int tmp = rank[rank.size() - 1 - i];
            reefD.push_back(tmp);
        }
    }
    for (int i = 0; i < reefD.size(); i++) //要刪除的元素由大到小排列
        for (int j = 1; j < reefD.size() - i; j++)
            if (reefD[j - 1] < reefD[j])
            {
                int tmp = reefD[j - 1];
                reefD[j - 1] = reefD[j];
                reefD[j] = tmp;
            }
    for (int i = 0; i < reefD.size(); i++) //由大開始刪除
        ReefSet.erase(ReefSet.begin() + reefD[i]);

    for (int i = 0; i < Map.size(); i++) //更新Map
        for (int j = 0; j < Map[i].size(); j++)
            Map[i][j] = -1;
    for (int i = 0; i < ReefSet.size(); i++)
        Map[ReefSet[i].n1][ReefSet[i].n2] = i;
}