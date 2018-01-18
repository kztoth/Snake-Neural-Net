#include "Population.h"

#define MAXSENSORS 15
#define MAXLAYERS 5
#define MUTATIONMOD 100

Population::Population(int PopNum)
{
    PopCount = PopNum;
    Generation = 0;
    Top = 0;
    Next = 0;
    MutationRate = 1;
    for(int i = 0; i < PopNum; i++)
    {
        DNA Temp;
        Pop.push_back(Temp);
    }
}

DNA Population::Mutate(DNA Input)
{
    if((rand() % MUTATIONMOD) < MutationRate && Input.SensorAngle.size() < MAXSENSORS) 
    {
        Input.SensorAngle.push_back(rand() % 270 - 135);
        for(unsigned int i = 0; i < Input.Weights[0].size(); i++)
            Input.Weights[0][i].push_back(((double)((rand() % 1999) - 1000) / 1000));
    }
    if((rand() % MUTATIONMOD) < MutationRate && Input.SensorAngle.size() > 0) 
    {
        int a = (rand() % Input.SensorAngle.size());
        Input.SensorAngle.erase(Input.SensorAngle.begin() + a);
        for(unsigned int i = 0; i < Input.Weights[0].size(); i++)
            Input.Weights[0][i].erase(Input.Weights[0][i].begin() + a);
    }

    for(unsigned int i = 0; i < Input.SensorAngle.size(); i++)
        if((rand() % MUTATIONMOD) < MutationRate) 
            Input.SensorAngle.push_back(rand() % 270 - 135);

    for(unsigned int i = 0; i < Input.Weights.size(); i++)
    {
        for(unsigned int j = 0; j < Input.Weights[i].size(); j++)
        {
            int Connections;
            if(i == 0)
                Connections = Input.SensorAngle.size();
            else
                Connections = Input.Layers[i-1];

            for(int k = 0; k < Connections; k++)
                if((rand() % MUTATIONMOD) < MutationRate)
                    Input.Weights[i][j][k] = ((double)((rand() % 1999) - 1000) / 1000);
        }
    }

    // Need to rebuild the whole Weights array after changing the layers
    // TODO: Mutate the Layers

    return Input;
}

DNA Population::Crossover(DNA First, DNA Seccond)
{
    return First;
}

void Population::Evolve()
{
    std::vector<DNA> Bucket;
    double TempFit;

    GetCurrentTopFitness();

    for(int i = 0; i < PopCount; i++)
    {
        TempFit = Pop[i].Fitness / Max;
        for(int j = 0; j < (TempFit * TempFit * 10); j++)
            Bucket.push_back(Pop[i]);
    }

    if(Bucket.size() < 1)
        Bucket = Pop;

    unsigned int BucketSize = Bucket.size();
    int a;
    int b;
    for(int i = 0; i < PopCount; i++)
    {
        a = rand() % BucketSize;
        b = rand() % BucketSize;
        Pop[i] = Crossover(Bucket[a], Bucket[b]);
        Pop[i] = Mutate(Pop[i]);
    }
}

double Population::GetTopFitness()
{
    for(unsigned int i = 0; i < Pop.size(); i++)
    {
        Top = (Pop[i].Fitness > Top) ? Pop[i].Fitness : Top;
    }

    return Top;
}

double Population::GetCurrentTopFitness()
{
    Max = 0;
    for(unsigned int i = 0; i < Pop.size(); i++)
    {
        Max = (Pop[i].Fitness > Max) ? Pop[i].Fitness : Max;
    }

    return Max;
}

DNA Population::NextPop()
{
    if(Next >= Pop.size())
    {
        Evolve();
        Next = 0;
        Generation++;
    }
    DNA Return = Pop[Next++];
    return Return;
}