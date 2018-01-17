#include <stdlib.h>
#include <time.h>
#include <vector>

class DNA
{
public:
    DNA();
    std::vector<int> SensorAngle;
    std::vector<int> Layers;
    std::vector< std::vector< std::vector<double> > > Weights;  // REFACTOR: This just seems pretty dumb...
                                                                // Layer[Node[Weights]]
    double Fitness;
    void Print();
private:
    char Name[16];
    void GenName(int Length);
};