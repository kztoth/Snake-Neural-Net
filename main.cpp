#include <windows.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include "Renderer.h"

#define NEURALRENDERWIDTH 0

struct window_dimension
{
    int Width;
    int Height;
};

// TODO: probably remove these variables
double Speed = 0;
double Angle = 0;

static GameState *State = NULL;
static Renderer *Render = NULL;
static NeuralNet *Net = NULL;

window_dimension GetWindowDim(HWND Window)
{
    window_dimension Result;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right;
    Result.Height = ClientRect.bottom;

    return(Result);
}

LRESULT CALLBACK MainWindowCallback(HWND Window, 
                                    UINT Message,
                                    WPARAM WParam,
                                    LPARAM LParam)
{
    LRESULT Result = 0;
    switch(Message)
    {
        case WM_SYSKEYUP:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_KEYDOWN:
        {
            bool WasDown = ((LParam & (1 << 30)) != 0);
            bool IsDown = ((LParam & (1 << 31)) == 0);
            if(WasDown != IsDown)
            {
                switch(WParam)
                {
                    case VK_LEFT:
                        if(IsDown)
                            Angle = -1;
                        if(WasDown)
                            Angle = 0;
                        break;
                    case VK_RIGHT:
                        if(IsDown)
                            Angle = 1;
                        if(WasDown)
                            Angle = 0;
                        break;
                    case VK_UP:
                        if(IsDown)
                            Speed = .3;
                        if(WasDown)
                            Speed = 0;
                        break;
                    case VK_DOWN:
                        if(IsDown)
                            Speed = -.3;
                        if(WasDown)
                            Speed = 0;
                        break;
                }
            }
        }break;
        case WM_SIZE:
        {
            window_dimension Dim = GetWindowDim(Window);
            Render->ResizePaintDIB(Dim.Width, Dim.Height);
        }break;
        case WM_CLOSE:
            DestroyWindow(Window);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;

            window_dimension Dim = GetWindowDim(Window);

            Render->CopyBuffer(DeviceContext,
                       Dim.Width, Dim.Height, 
                       X, Y);
            ReleaseDC(Window, DeviceContext);
        }break;
        default:
            Result = DefWindowProc(Window, Message, WParam, LParam);  
    }
    return Result;
}

int CALLBACK WinMain(HINSTANCE Instance, 
                     HINSTANCE PrevInstance, 
                     LPSTR CommandLine, 
                     int ShowCommand)
{
    WNDCLASS WindowClass;

    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "SnakeClass";
    WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if(RegisterClass(&WindowClass))
    {
        HWND Window = 
            CreateWindowEx(
                0, 
                WindowClass.lpszClassName, 
                "Snake AI",
                WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_VISIBLE,
                CW_USEDEFAULT, CW_USEDEFAULT,
                1000, 800, 0, 0, Instance, 0);
        if(Window)
        {
            bool Running = TRUE;
            MSG Message;
            window_dimension Dim = GetWindowDim(Window);
            GameState GS(Dim.Width - NEURALRENDERWIDTH, Dim.Height);
            State = &GS;
            Renderer Ren(Dim.Width, Dim.Height);
            Render = &Ren;
            NeuralNet NN(0, State->S[0].Sensors.size(), 2);
            Net = &NN;
            while(Running)
            {
                while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
                {
                    if(Message.message == WM_QUIT)
                        Running = FALSE;
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }

                // Calculate the NN
                // Calculate new game state
                // render game state in buffer
                // push to screen
                State->Calculate();
                State->Move();
                Render->RenderScreen(State, &State->S[0].NN);

                HDC DeviceContext = GetDC(Window);
                window_dimension Dim = GetWindowDim(Window);
                Render->CopyBuffer(DeviceContext,
                           Dim.Width, Dim.Height, 
                           0, 0);
                ReleaseDC(Window, DeviceContext);
            }
        }
    }

    return 0;
    std::cout << "";    // Black magic don't remove.
                        // Turns out that the terminal would just close if nothing keeps 
                        // it open. The thing to keep it open is the print statement.
}