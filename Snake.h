#include <vector>
#include "NeuralNet.h"

struct Location
{
    double X;
    double Y;
};

class Snake
{
public:
    Snake(DNA Dna);
    void AddBodyPart(){ Body.push_back(Body.back()); };
    int R;
    double A;
    std::vector<Location> Body;
    std::vector<Sensor> Sensors;
    NeuralNet NN;
    DNA D;
private:
};