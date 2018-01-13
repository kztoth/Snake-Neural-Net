#include <vector>

class Neuron
{
public:
    Neuron(double Bias, int Inputs);
    void CalculateOutput(std::vector<Neuron> Layer); // can pass in a pointer to the previous layer
    void SetOutput(double Output){ OutputValue = Output; };
    void SetInputWeight(int Neuron, double Weight){ InputWeights[Neuron] = Weight; };
    void SetBias(double Bias){ B = Bias; };
    void SetWeights(std::vector<double> Weights){ InputWeights = Weights; };
    double GetOutputValue(){ return OutputValue; };
private:
    double OutputValue;
    std::vector<double> InputWeights;
    double B;
};