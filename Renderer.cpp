#include "Renderer.h"
static offscreen_buffer BackBuffer;

Renderer::Renderer(int W, int H)
{
    Width = W;
    Height = H;
    Buffer = &BackBuffer;
    ResizePaintDIB(Width, Height);
}

ULONG Renderer::GetColor(UCHAR Red, UCHAR Green, UCHAR Blue)
{
    return(((Red << 16) | (Green << 8) | Blue));
}

void Renderer::PlotLine(double X0, double Y0, double X1, double Y1, ULONG Color)
{
    bool Steep = (fabs(Y1 - Y0) > fabs(X1 - X0));
    if(Steep)
    {
        std::swap(X0, Y0);
        std::swap(X1, Y1);
    }
    if(X0 > X1)
    {
        std::swap(X0, X1);
        std::swap(Y0, Y1);
    }

    float DX = X1 - X0;
    float DY = fabs(Y1 - Y0);

    float Err = DX / 2.0f;
    int YStep = (Y0 < Y1) ? 1 : -1;
    int Y = (int)Y0;

    int MaxX = (int)X1;

    UCHAR *Row = (UCHAR *)Buffer->Memory;
    ULONG *Pixel = (ULONG *)Row;

    for(int X = (int)X0; X < MaxX; X++)
    {
        if(Steep)
        {
            if(X < Height && X >= 0 && Y < Width && Y >= 0)
                *(Pixel + Y + X * Buffer->Width) = Color;            
        }
        else
        {
            if(X < Width && X >= 0 && Y < Height && Y >= 0)
                *(Pixel + X + Y * Buffer->Width) = Color;
        }

        Err -= DY;
        if(Err < 0)
        {
            Y += YStep;
            Err += DX;
        }
    }
}

void Renderer::PlotCircle(int X0, int Y0, int Radius, ULONG Color)
{
    int X = Radius - 1;
    int Y = 0;
    int DX = 1;
    int DY = 1;
    int Err = DX - (Radius << 1);
    int Width = Buffer->Width;

    UCHAR *Row = (UCHAR *)Buffer->Memory;

    ULONG *Pixel = (ULONG *)Row;
    while(X >= Y)
    {
        *(Pixel + ((X0 - Y) + ((Y0 * Width) - (X * Width)))) = Color;
        *(Pixel + ((X0 + Y) + ((Y0 * Width) - (X * Width)))) = Color;

        *(Pixel + ((X0 - X) + ((Y0 * Width) - (Y * Width)))) = Color;
        *(Pixel + ((X0 + X) + ((Y0 * Width) - (Y * Width)))) = Color;

        *(Pixel + ((X0 - X) + ((Y0 * Width) + (Y * Width)))) = Color;
        *(Pixel + ((X0 + X) + ((Y0 * Width) + (Y * Width)))) = Color;

        *(Pixel + ((X0 - Y) + ((Y0 * Width) + (X * Width)))) = Color;
        *(Pixel + ((X0 + Y) + ((Y0 * Width) + (X * Width)))) = Color;

        if(Err <= 0)
        {
            Y++;
            Err += DY;
            DY += 2;
        }
        if(Err > 0)
        {
            X--;
            DX += 2;
            Err += (-Radius << 1) + DX;
        }
    }
}

void Renderer::PlotCircleFill(int X0, int Y0, int Radius, ULONG Color)
{
    int X = Radius - 1;
    int Y = 0;
    int DX = 1;
    int DY = 1;
    int Err = DX - (Radius << 1);
    while(X >= Y)
    {
        PlotLine((X0 - Y), (Y0 - X), (X0 + Y), (Y0 - X), Color);
        PlotLine((X0 - X), (Y0 - Y), (X0 + X), (Y0 - Y), Color);
        PlotLine((X0 - X), (Y0 + Y), (X0 + X), (Y0 + Y), Color);
        PlotLine((X0 - Y), (Y0 + X), (X0 + Y), (Y0 + X), Color);
        if(Err <= 0)
        {
            Y++;
            Err += DY;
            DY += 2;
        }
        if(Err > 0)
        {
            X--;
            DX += 2;
            Err += (-Radius << 1) + DX;
        }
    }
}

void Renderer::RenderGradient(int XOffset, int YOffset)
{
    UCHAR *Row = (UCHAR *)Buffer->Memory;
    for(int Y = 0; Y < Buffer->Height; Y++)
    {
        ULONG *Pixel = (ULONG *)Row;
        for(int X = 0; X < Buffer->Width; X++)
        {
            UCHAR Blue = (UCHAR)(X + XOffset);
            UCHAR Green = (UCHAR)(Y + YOffset);
            UCHAR Red = (UCHAR)(X + XOffset) + (UCHAR)(Y + YOffset);
            *Pixel++ = GetColor(Red, Green, Blue);
        }
        Row += Buffer->Pitch;
    }
}

void Renderer::RenderBackground(int X0, int Y0, int Width, int Height, ULONG Color)
{
    UCHAR *Row = (UCHAR *)Buffer->Memory;
    Row += Buffer->Pitch * Y0;
    for(int Y = Y0; Y < Height; Y++)
    {
        ULONG *Pixel = (ULONG *)Row + X0;
        for(int X = X0; X < Width; X++)
        {
            *Pixel++ = Color;
        }
        Row += Buffer->Pitch;
    }
}

void Renderer::RenderFood(GameState *State)
{
    PlotCircleFill(State->F.X, State->F.Y, State->F.R, 0x000000FF);
}

void Renderer::RenderSnake(GameState *State)
{
    for(unsigned int i = 0; i < State->S.size(); i++)
    {
        int Radius = State->S[i].R;
        PlotCircleFill(State->S[i].Body[0].X, State->S[i].Body[0].Y, Radius, 0x00000000);
        for(unsigned int j = 1; j < State->S[i].Body.size(); j++)
        {
            PlotCircle(State->S[i].Body[j].X, State->S[i].Body[j].Y, Radius, 0x00000000);
            PlotCircle(State->S[i].Body[j].X, State->S[i].Body[j].Y, Radius-1, 0x00000000);
        }
        for(unsigned int j = 0; j < State->S[i].Sensors.size(); j++)
        {
            ULONG Color = GetColor(100, 100, 100);
            if(State->S[i].Sensors[j].Reading == -1)
                Color = GetColor(255, 0, 0);
            if(State->S[i].Sensors[j].Reading == 1)
                Color = GetColor(0, 255, 0);

            PlotLine(State->S[i].Body[0].X, State->S[i].Body[0].Y, 
                     State->S[i].Sensors[j].X, 
                     State->S[i].Sensors[j].Y,
                     Color);
        }
    }
}

void Renderer::RenderNet(NeuralNet *Net)
{
    //TODO: Render the neural net.
}

void Renderer::RenderScreen(GameState *State, NeuralNet *Net)
{
    RenderBackground(0, 0, State->Width, State->Height, GetColor(200, 200, 200));
    RenderSnake(State);
    RenderFood(State);
    RenderBackground(State->Width, 0, Width, Height, GetColor(255, 255, 255));
    RenderNet(Net);

}

void Renderer::ResizePaintDIB(int W, int H)
{
    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }
    Width = W;
    Height = H;

    Buffer->BytesPerPixel = 4;
    Buffer->Width = Width;
    Buffer->Height = Height;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = (Buffer->Width * Buffer->Height) * Buffer->BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    Buffer->Pitch = Width * Buffer->BytesPerPixel;
}

void Renderer::CopyBuffer(HDC DeviceContext,
                int WindowWidth, int WindowHeight, 
                int X, int Y)
{
    StretchDIBits(DeviceContext, 
                  0, 0, WindowWidth, WindowHeight,
                  0, 0, Width, Height,
                  Buffer->Memory,
                  &Buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);
}