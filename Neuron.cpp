#include <cmath>
#include "Neuron.h"

Neuron::Neuron(double Bias, int Inputs)
{
    OutputValue = 0;
    B = Bias;
    for(int i = 0; i < Inputs; i++)
        InputWeights.push_back(0);
}

void Neuron::CalculateOutput(std::vector<Neuron> Layer)
{
    double Sum = 0;
    for(unsigned int i = 0; i < Layer.size(); i++)
        Sum += Layer[i].GetOutputValue() * InputWeights[i];
    Sum += B;

    OutputValue = tanh(Sum);
}