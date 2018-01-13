#ifndef STRUCTS_H
#define STRUCTS_H
struct Sensor
{
    double X;
    double Y;
    double A;
    int Reading;
};

struct Output
{
    double Angle;
    double Speed;
};
#endif