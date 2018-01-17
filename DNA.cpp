#include "DNA.h"
#include <stdio.h>

void DNA::GenName(int Length)
{
    static const char Alpha[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for(int i = 0; i < Length; i++)
    {
        Name[i] = Alpha[rand() % sizeof(Alpha) - 1];
    }
    Name[Length] = '\0';
}

DNA::DNA()
{
    GenName(rand() % 10 + 5);
    int SensorCount = rand() % 16;
    for(int i = 0; i < SensorCount; i++)
    {
        SensorAngle.push_back(rand() % 270 - 135);
    }

    int LayerCount = rand() % 5 + 1;
    Weights.resize(LayerCount);
    for(int i = 0; i < LayerCount; i++)
    {
        int Nodes = 1 << (rand() % 3 + 3);
        Layers.push_back(Nodes);
        for(int j = 0; j < Nodes; j++)
        {
            Weights[i].resize(Nodes);

            int Connections;
            if(i == 0)
                Connections = SensorCount;
            else
                Connections = Layers[i-1];

            for(int k = 0; k < Connections; k++)
            {
                Weights[i][j].push_back(((double)((rand() % 1999) - 1000) / 1000));
            }
        }
    }

    Weights.resize(Weights.size() + 1);
    Weights.back().resize(2);
    for(int i = 0; i < Layers.back(); i++)
    {
        Weights.back()[0].push_back(((double)((rand() % 1999) - 1000) / 1000));
        Weights.back()[1].push_back(((double)((rand() % 1999) - 1000) / 1000));
    }
}

void DNA::Print()
{
    printf("DNA Name: %s\n", Name);
    printf("Sensor Count: %d\n", SensorAngle.size());
    printf("Sensor Angles:\n");
    for(unsigned int i = 0; i < SensorAngle.size(); i++)
    {
        if(i > 0 && i % 10 == 0) printf("\n");
        printf("%d\t", SensorAngle[i]);
    }
    printf("\n");
    printf("Layers: %d\n", Layers.size());
    printf("Nodes:\n");
    for(unsigned int i = 0; i < Layers.size(); i++)
        printf("%d\t", Layers[i]);
    printf("\n");
    // printf("Weights:\n");
    // for(unsigned int i = 0; i < Weights.size(); i++)
    // {
    //     for(unsigned int j = 0; j < Weights[i].size(); j++)
    //     {
    //         printf("%d\t", Weights[i][j].size());
    //     }
    //     printf("\n");
    // }
    printf("\n");
}