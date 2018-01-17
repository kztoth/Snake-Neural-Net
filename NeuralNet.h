#include "Neuron.h"
#include "Structs.h"
#include <vector>

class NeuralNet{
public:
    NeuralNet(void){};
    NeuralNet(std::vector<int> LayerNodes, int Inputs, int Outputs, std::vector< std::vector< std::vector<double> > > Weights);
    void GetResults(std::vector<Sensor> Sensors);
    std::vector< std::vector<Neuron> > Layers;
    Output GetOutput(){ return Out; };
private:
    Output Out;
};