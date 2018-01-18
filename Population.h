#include "Snake.h"
#include <vector>

class Population
{
public:
    Population(void){};
    Population(int PopNum);
    int Generation;
    void Evolve();
    void SetFitness(int Index, double Fitness){ Pop[Index].Fitness = Fitness; };
    double GetTopFitness();
    double GetCurrentTopFitness();
    int GetIndex(){ return Next - 1; };
    DNA NextPop();
    DNA GetDNA(int i){ return Pop[i]; };
private:
    double Top;
    double Max;
    double MutationRate;
    int PopCount;
    unsigned int Next;
    DNA Crossover(DNA First, DNA Seccond); // TODO
    DNA Mutate(DNA Input);    // TODO
    std::vector<DNA> Pop;
};