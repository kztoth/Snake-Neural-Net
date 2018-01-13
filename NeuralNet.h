#include "Neuron.h"
#include "Structs.h"
#include <vector>

class NeuralNet{
public:
    NeuralNet(void){};
    NeuralNet(int LayerAmount, int Inputs, int Outputs);
    void GetResults(std::vector<Sensor> Sensors);
    std::vector< std::vector<Neuron> > Layers;
    Output GetOutput(){ return Out; };
private:
    Output Out;
};