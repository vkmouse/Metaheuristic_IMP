#include "__header__.h"
se::se(int num_searcher, int num_region, int num_sample, int num_player)
{
    this->num_searcher = num_searcher;
    this->num_region = num_region;
    this->num_sample = num_sample;
    this->num_player = num_player;
}
void se::run()
{
    init();                 // 1. initialization
    resource_arrangement(); // 2. resource arrangement
    for (int iter = 0; iter < iteration && ObjectValueCount < MaxOV; iter++)
    {
        vision_search();    // 3. vision search
        marketing_survey(); // 4. marketing survey
    }
}

// 1. initialization
void se::init()
{
    // set aside arrays for searchers, samples, and sampleV (the
    // crossover results of searchers and samples)
    searcher_sol.assign(num_searcher, v1d<int>(NumSeed, 0));
    sample_sol.assign(num_region, v2d<int>(num_sample, v1d<int>(NumSeed, 0)));
    sample_sol_best.assign(num_region, v1d<int>(NumSeed, 0));
    sampleV_sol.assign(num_searcher, v3d<int>(num_region, v2d<int>(num_sample * 2, v1d<int>(NumSeed, 0))));

    searcher_sol_fitness.assign(num_searcher, 0.0);
    sample_sol_fitness.assign(num_region, v1d<double>(num_sample, 0.0));
    sample_sol_best_fitness.assign(num_region, 0.0);
    sampleV_sol_fitness.assign(num_searcher, v2d<double>(num_region, v1d<double>(num_sample * 2, 0.0)));

    for (int i = 0; i < num_searcher; i++)
    {
        searcher_sol[i] = CreateSeedset(mDataset.useful, NumSeed);
        searcher_sol_fitness[i] = ObjectValue(searcher_sol[i]);
    }

    // IM
    useful.resize(num_region);
    for (int i = 0; i < num_region; i++)
        for (int j = i; j < mDataset.useful.size(); j = j + num_region)
            useful[i].push_back(mDataset.useful[j]);
}
// 2. resource arrangement
void se::resource_arrangement()
{
    // 2.1. initialize searchers and regions
    // 2.1.1 assign searcher to its region and their investment

    // 2.1.2 initialize the sample solutions
    for (int i = 0; i < num_region; i++)
        for (int j = 0; j < num_sample; j++)
        {
            sample_sol[i][j] = CreateSeedset(mDataset.useful, NumSeed);
            sample_sol_fitness[i][j] = ObjectValue(sample_sol[i][j]);
        }

    // 2.2. initialize the investment and set how long regions have not been searched
    region_it.assign(num_region, 0.0);
    region_hl.assign(num_region, 1.0);
    for (int i = 0; i < num_searcher; i++)
    {
        int idx = i % num_region;
        region_it[idx]++;
        region_hl[idx] = 1.0;
    }

    // 2.3. initialize the expected values (ev)
    expected_value.assign(num_searcher, v1d<double>(num_region, 0.0));
    T_j.assign(num_region, 0.0);
    V_ij.assign(num_searcher, v1d<double>(num_region, 0.0));
    M_j.assign(num_region, 0.0);
}
// 3. vision search
void se::vision_search()
{
    // 3.1 construct V (searcher X sample)
    transit();
    // 3.2 compute the expected value of all regions of searchers
    compute_expected_value();
    // 3.3 select region to which a searcher will be assigned
    vision_selection(num_player);
}
// 3.1 construct V (searcher X sample)
void se::transit()
{
    // 3.1.1 exchange information between searchers and samples; save
    // the results in sampleV_sol, by using the concept of crossover
    // of GA
    double changeRate = 0.01 * (0.5 - ObjectValueCount / MaxOV);
    if (changeRate < 0)
        changeRate = 0;

    for (int i = 0; i < num_searcher; i++)
    {
        for (int j = 0; j < num_region; j++)
        {
            for (int k = 0; k < num_sample; k++)
            {
                // crossover
                const int m = k << 1;
                sampleV_sol[i][j][m] = searcher_sol[i];
                sampleV_sol[i][j][m + 1] = sample_sol[j][k];
                crossoverTwoSolution(sampleV_sol[i][j][m], sampleV_sol[i][j][m + 1], changeRate);
            }
        }
    }
    // 3.1.2 randomly mutate one bit of each solution in sampleV_sol,
    // by using the concept of mutation of GA
    for (int i = 0; i < num_searcher; i++)
    {
        for (int j = 0; j < num_region; j++)
        {
            for (int k = 0; k < 2 * num_sample; k++)
            {
                //useful[j]就是區域j可搜尋範圍
                sampleV_sol[i][j][k] = transition(useful[j], sampleV_sol[i][j][k]);
            }
        }
    }
}
void se::compute_expected_value()
{
    // 3.2.1 fitness value of searchers

    // 3.2.2 fitness value of samples, M_j
    double all_sample_fitness = 0.0; // f(m_j)
    for (int i = 0; i < num_region; i++)
    {
        double rbj = sample_sol_best_fitness[i];
        int idx = -1;

        for (int j = 0; j < num_sample; j++)
        {
            all_sample_fitness += sample_sol_fitness[i][j];
            // update fbj
            if (sample_sol_fitness[i][j] > rbj)
            {
                idx = j;
                rbj = sample_sol_fitness[i][j];
            }
        }

        if (idx >= 0)
        {
            sample_sol_best_fitness[i] = rbj;
            sample_sol_best[i] = sample_sol[i][idx];
        }
    }

    // M_j
    for (int i = 0; i < num_region; i++)
        M_j[i] = sample_sol_best_fitness[i] / all_sample_fitness;

    // 3.2.3 fitness value of sampleV, V_ij
    for (int i = 0; i < num_searcher; i++)
    {
        for (int j = 0; j < num_region; j++)
        {
            V_ij[i][j] = 0.0;
            for (int k = 0; k < num_sample; k++)
            {
                const int m = k << 1;
                sampleV_sol_fitness[i][j][m] = ObjectValue(sampleV_sol[i][j][m]);
                sampleV_sol_fitness[i][j][m + 1] = ObjectValue(sampleV_sol[i][j][m + 1]);
                V_ij[i][j] += sampleV_sol_fitness[i][j][m] + sampleV_sol_fitness[i][j][m + 1];
            }
            V_ij[i][j] /= 2 * num_sample; // *** Bugfix: Divide by 2*num_sample here ***
        }
    }

    // 3.2.4 T_j
    for (int i = 0; i < num_region; i++) // *** Bugfix: Changed num_searcher to num_region ***
        T_j[i] = region_it[i] / region_hl[i];

    // 3.2.5 compute the expected_value
    for (int i = 0; i < num_searcher; i++)
    {
        for (int j = 0; j < num_region; j++)
        {
            expected_value[i][j] = T_j[i] * V_ij[i][j] * M_j[j];
        }
    }

    // 3.2.6 update sampleV to sample
    for (int i = 0; i < num_searcher; i++)
    {
        for (int j = 0; j < num_region; j++)
        {
            for (int k = 0; k < num_sample; k++)
            {
                const int m = k << 1;
                if (sampleV_sol_fitness[i][j][m] > sample_sol_fitness[j][k])
                {
                    sample_sol[j][k] = sampleV_sol[i][j][m];
                    sample_sol_fitness[j][k] = sampleV_sol_fitness[i][j][m];
                }
                if (sampleV_sol_fitness[i][j][m + 1] > sample_sol_fitness[j][k])
                {
                    sample_sol[j][k] = sampleV_sol[i][j][m + 1];
                    sample_sol_fitness[j][k] = sampleV_sol_fitness[i][j][m + 1];
                }
            }
        }
    }
}
// 3.3 select region to which a searcher will be assigned
void se::vision_selection(int player)
{
    for (int i = 0; i < num_region; i++)
        region_hl[i]++;

    for (int i = 0; i < num_searcher; i++)
    {
        // find the idx of the best vij
        int play0_idx = rand() % num_region;
        double play0_ev = expected_value[i][play0_idx];

        for (int j = 0; j < num_player - 1; j++)
        {
            int play1_idx = rand() % num_region;
            if (expected_value[i][play1_idx] > play0_ev)
            {
                play0_idx = play1_idx;
                play0_ev = expected_value[i][play0_idx];
            }
        }

        // update searcher_sol
        if (sample_sol_best_fitness[play0_idx] > searcher_sol_fitness[i])
        {
            searcher_sol[i] = sample_sol_best[play0_idx];
            searcher_sol_fitness[i] = sample_sol_best_fitness[play0_idx];
        }

        // update region_it[i] and region_hl[i];
        region_it[play0_idx]++;
        region_hl[play0_idx] = 1;
    }
}

// 4. marketing survey
void se::marketing_survey()
{
    for (int i = 0; i < num_region; i++)
        // 4.1 update region_it
        if (region_hl[i] > 1)
            region_it[i] = 1.0;

    // 4.2 update the best solution
    int j = -1;
    for (int i = 0; i < num_searcher; i++)
    {
        determination(searcher_sol[i], searcher_sol_fitness[i]);
    }
}
