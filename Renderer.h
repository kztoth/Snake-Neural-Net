// make a renderer class and it can include the height and width

#include <windows.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include "GameState.h"

struct offscreen_buffer
{
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};

class Renderer
{
public:
    Renderer(int W, int H);
    void CopyBuffer(HDC DeviceContext, int WindowWidth, int WindowHeight, int X, int Y);
    void ResizePaintDIB(int W, int H);
    void RenderScreen(GameState *State, NeuralNet *Net);
private:
    int Width;
    int Height;
    offscreen_buffer *Buffer;
    void RenderSnake(GameState *State);
    void RenderFood(GameState *State);
    void RenderNet(NeuralNet *Net);
    void RenderBackground(int X, int Y, int Width, int Height, ULONG Color);
    void RenderGradient(int XOffset, int YOffset);
    void PlotCircleFill(int X0, int Y0, int Radius, ULONG Color);
    void PlotCircle(int X0, int Y0, int Radius, ULONG Color);
    void PlotLine(double X0, double Y0, double X1, double Y1, ULONG Color);
    ULONG GetColor(UCHAR Red, UCHAR Green, UCHAR Blue);
};