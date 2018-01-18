#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include "Population.h"

struct Food
{
    int X;
    int Y;
    int R;
};

class GameState
{
public:
    GameState(int W, int H);
    void Move();
    void Calculate();
    std::vector<Snake> S;
    Food F;
    int Width;
    int Height;
    void NewFood();// TODO: move back to private. Being used to move food at will.
private:
    Population P;
    void StartNew();
    int RunNum;
    int RunTime;
    double LastFitness;
    void Kill(unsigned int i, bool Out);
    void CheckCollision(unsigned int i);
    void EatFood(unsigned int i);
    void SpawnSnake(int X, int Y, int Angle, int Radius);
    double Dist(double X0, double Y0, double X1, double Y1);
};