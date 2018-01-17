#include "GameState.h"
#include <stdio.h>

#define PI 3.14159265
#define SENSORLENGTH 40
#define BASESPEED 1
#define GROWTHRATE 100
#define TOTALSNAKES 8
#define POPULATION 8

double GameState::Dist(double X0, double Y0, double X1, double Y1)
{
    double r = sqrt(pow(X1 - X0, 2) + pow(Y1 - Y0, 2));
    return(r);
}

void GameState::SpawnSnake(int X, int Y, int Angle, int Radius, int PopCount){
    Snake Sn(Population[PopCount]);
    Sn.Body[0].X = X;
    Sn.Body[0].Y = Y;
    Sn.A = Angle;
    Sn.R = Radius;
    S.push_back(Sn);
}

GameState::GameState(int W, int H)
{
    srand(time(NULL));
    for(int i = 0; i < POPULATION; i++)
    {
        DNA Temp;
        Population.push_back(Temp);
    }
    for(int i = 0; i < TOTALSNAKES; i++)
        SpawnSnake(400, 400 + 10 * i, 0, 5, i);
    Width = W;
    Height = H;
    NewFood();
    F.R = 10;
}

void GameState::NewFood()
{
    F.X = (rand() % (Width - S[0].R * 2)) + S[0].R;
    F.Y = (rand() % (Height - S[0].R * 2)) + S[0].R;
}

void GameState::EatFood(unsigned int i)
{
    NewFood();
    for(int j = 0; j < GROWTHRATE; j++)
        S[i].AddBodyPart();
}

// REFACTOR: change this into something other than just moving the snake back
void GameState::Reset(unsigned int i)
{
    S[i].Body.resize(1);
    S[i].Body[0].X = 400;
    S[i].Body[0].Y = 400;
    S[i].A = rand() % 360*PI/180;
    // for(unsigned int i = 0; i < S[i].Sensors.size(); i++)
    // {
    //     S[i].Sensors[i].X = S[i].Body[0].X;
    //     S[i].Sensors[i].Y = S[i].Body[0].Y;
    // }
}

void GameState::CheckCollision(unsigned int i)
{
// TODO: Collision with bodies
    if(S[i].Body[0].Y < S[i].R) Reset(i);
    if(S[i].Body[0].Y > Height - S[i].R) Reset(i);
    if(S[i].Body[0].X < S[i].R) Reset(i);
    if(S[i].Body[0].X > Width - S[i].R) Reset(i);

    double HeadDistance = Dist(S[i].Body[0].X, S[i].Body[0].Y, F.X, F.Y);
    if(HeadDistance < (F.R + S[i].R))
        EatFood(i);
    for(unsigned int j = 0; j < S[i].Sensors.size(); j++)
    {
        S[i].Sensors[j].Reading = 0;
        double FoodDistance = fabs(((S[i].Sensors[j].Y - S[i].Body[0].Y) * F.X) - ((S[i].Sensors[j].X - S[i].Body[0].X) * F.Y) + 
                                      (S[i].Sensors[j].X * S[i].Body[0].Y) - (S[i].Sensors[j].Y * S[i].Body[0].X)) / 
                                sqrt(pow(S[i].Sensors[j].Y - S[i].Body[0].Y, 2) + pow(S[i].Sensors[j].X - S[i].Body[0].X, 2));
        double TipDistance = Dist(S[i].Sensors[j].X, S[i].Sensors[j].Y, F.X, F.Y);
        if(S[i].Sensors[j].X < 0 | S[i].Sensors[j].X > Width | S[i].Sensors[j].Y < 0 | S[i].Sensors[j].Y > Height)
            S[i].Sensors[j].Reading = -1;
        if(FoodDistance < F.R && (TipDistance < HeadDistance || (HeadDistance < SENSORLENGTH && TipDistance < SENSORLENGTH)))
            S[i].Sensors[j].Reading = 1;
    }

}

void GameState::Move()
{
    for(unsigned int i = 0; i < S.size(); i++)
    {
        Output Input = S[i].NN.GetOutput();
        S[i].A += Input.Angle * PI / 180;

        S[i].Body.back().X = cos(S[i].A) * (BASESPEED + Input.Speed) + S[i].Body[0].X;
        S[i].Body.back().Y = sin(S[i].A) * (BASESPEED + Input.Speed) + S[i].Body[0].Y;
        S[i].Body.insert(S[i].Body.begin(), S[i].Body.back());
        S[i].Body.pop_back();

        for(unsigned int j = 0; j < S[i].Sensors.size(); j++)
        {
            S[i].Sensors[j].X = cos(S[i].A + S[i].Sensors[j].A) * (SENSORLENGTH) + S[i].Body[0].X;
            S[i].Sensors[j].Y = sin(S[i].A + S[i].Sensors[j].A) * (SENSORLENGTH) + S[i].Body[0].Y;
        }
        CheckCollision(i);
    }
}

void GameState::Calculate()
{
    for(unsigned int i = 0; i < S.size(); i++)
        S[i].NN.GetResults(S[i].Sensors);
}