#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include "Snake.h"

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
private:  // could pass pointers to snake instead of passing i but this is probably fine
    void Reset(unsigned int i);
    void CheckCollision(unsigned int i);
    void EatFood(unsigned int i);
    void NewFood();
    void SpawnSnake(int X, int Y, int Angle, int Radius);
    double Dist(double X0, double Y0, double X1, double Y1);
};