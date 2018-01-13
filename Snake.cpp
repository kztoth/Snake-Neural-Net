#include "Snake.h"

#define PI 3.14159265

Snake::Snake()
{
    Sensor FarLeft;
    Sensor Left;
    Sensor Center;
    Sensor Right;
    Sensor FarRight;
    FarLeft.A = 90 * PI / 180;
    FarLeft.Reading = 0;
    Left.A = 30 * PI / 180;
    Left.Reading = 0;
    Center.A = 0;
    Center.Reading = 0;
    Right.A = -30 * PI / 180;
    Right.Reading = 0;
    FarRight.A = -90 * PI / 180;
    FarRight.Reading = 0;

    Sensors.push_back(FarLeft);
    Sensors.push_back(Left);
    Sensors.push_back(Center);
    Sensors.push_back(Right);
    Sensors.push_back(FarRight);

    Location Head;
    Body.push_back(Head);

    NeuralNet Net(0, Sensors.size(), 2);
    NN = Net;
}