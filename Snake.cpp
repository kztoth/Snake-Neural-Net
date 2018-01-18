#include "Snake.h"

#define PI 3.14159265

Snake::Snake(DNA Dna)
{
    D = Dna;

    for(unsigned int i = 0; i < D.SensorAngle.size(); i++)
    {
        Sensor Temp;
        Temp.A = D.SensorAngle[i] * PI / 180;
        Temp.Reading = 0;
        Sensors.push_back(Temp);
    }

    Location Head;
    Body.push_back(Head);

    NeuralNet Net(D.Layers, Sensors.size(), 2, D.Weights);
    NN = Net;
}