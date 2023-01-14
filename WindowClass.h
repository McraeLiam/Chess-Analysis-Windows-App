#pragma once

#include "InternalDefs.h"
#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

#include <iostream>
#include <ctime>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
    Window();
    Window(const Window&) = delete;
    Window& operator =(const Window&) = delete;
    ~Window();

    HRESULT CreateGraphicsResources();
    void DiscardGraphicsResources();
    void Render();
    void CalculateLayout();

    bool ProcessMessages();

private:
    HINSTANCE m_hInstance;
    HWND m_hMainWnd;
};