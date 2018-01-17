#include "NeuralNet.h"

NeuralNet::NeuralNet(std::vector<int> LayerNodes, int Inputs, int Outputs, std::vector< std::vector< std::vector<double> > > Weights)
{
    int LayerAmount = LayerNodes.size();
    Layers.resize(LayerAmount + 2);
    Neuron In(0, 0);
    for(int i = 0; i < Inputs; i++)
        Layers[0].push_back(In);


    for(int i = 1; i <= LayerAmount; i++)
    {
        for(int j = 0; j < LayerNodes[i-1]; j++)
        {
            Neuron Temp(0, Layers[i-1].size());
            Temp.SetWeights(Weights[i-1][j]);
            Layers[i].push_back(Temp);
        }
    }

    Neuron Out(0, Layers[Layers.size()-2].size());
    for(int i = 0; i < Outputs; i++)
        Layers.back().push_back(Out);

    Layers.back()[0].SetWeights(Weights.back()[0]);
    Layers.back()[1].SetWeights(Weights.back()[1]);
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