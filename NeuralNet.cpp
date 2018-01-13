#include "NeuralNet.h"

NeuralNet::NeuralNet(int LayerAmount, int Inputs, int Outputs)
{
    Layers.resize(LayerAmount + 2);
    Neuron In(0, 0);
    for(int i = 0; i < Inputs; i++)
        Layers[0].push_back(In);
    Neuron Out(0, Layers[Layers.size()-2].size());
    for(int i = 0; i < Outputs; i++)
        Layers.back().push_back(Out);

    // testing values
    std::vector<double> A;
    A.push_back(1);
    A.push_back(1);
    A.push_back(.1);
    A.push_back(-1);
    A.push_back(-1);
    std::vector<double> S;
    S.push_back(1);
    S.push_back(1);
    S.push_back(1);
    S.push_back(1);
    S.push_back(1);
    Layers.back()[0].SetWeights(A);
    Layers.back()[1].SetWeights(S);
}

void NeuralNet::GetResults(std::vector<Sensor> Sensors)
{
    for(unsigned int i = 0; i < Sensors.size(); i++)
        Layers[0][i].SetOutput(Sensors[i].Reading);

    for(unsigned int i = 1; i < Layers.size(); i++)
        for(unsigned int j = 0; j < Layers[i].size(); j++)
            Layers[i][j].CalculateOutput(Layers[i-1]);

    Out.Angle = Layers.back()[0].GetOutputValue();
    Out.Speed = Layers.back()[1].GetOutputValue();
}