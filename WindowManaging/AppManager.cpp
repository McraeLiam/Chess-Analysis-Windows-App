#include "WindowClass.h"

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

// Internal Object References
std::vector<Section> sectionsRef = {};
std::vector<GameArea> gameRef = {};

// Graphics Handling
ID2D1Factory*            pFactory;
ID2D1HwndRenderTarget*  pRenderTarget;
ID2D1SolidColorBrush*   pBrush;

// D2D1_FACTORY_TYPE_SINGLE_THREADED and D2D1_FACTORY_TYPE_MULTI_THREADED are both options
HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

void Window::CalculateLayout()
{
    if (pRenderTarget != NULL)
    {
        D2D1_SIZE_F size = pRenderTarget->GetSize();
        const float x = size.width / 2;
        const float y = size.height / 2;
        
    }
}

HRESULT Window::CreateGraphicsResources() 
{
    HRESULT hr = S_OK;
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hMainWnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hMainWnd, size),
            &pRenderTarget);

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 1.0f);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
            if (SUCCEEDED(hr)) 
            {
                CalculateLayout();
            }
        }
    }
    return hr;
}

void Window::Render() 
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hMainWnd, &ps);
        D2D1::ColorF backgroundColor(0, 0, 0, 0);

        pRenderTarget->BeginDraw();

        pRenderTarget->Clear(D2D1::ColorF(backgroundColor));

        for (int i = 0; i < sectionsRef.size(); ++i)
        {
            if (sectionsRef[i].showBorder)
            {
                pRenderTarget->DrawRectangle(sectionsRef[i].renderShape, pBrush);
            }
        }

        for (int i = 0; i < gameRef.size(); ++i)
        {
            if (gameRef[i].showBorder)
            {
                pRenderTarget->DrawRectangle(sectionsRef[i].renderShape, pBrush);
            }
        }

        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hMainWnd, &ps);
    }
}

void Window::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

int red = 255;

// Main Window Handling
Window* pWindow = new Window();

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_PAINT:
        pWindow->Render();
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window()
    : m_hInstance(GetModuleHandle(nullptr))
{
    const wchar_t* CLASS_NAME = L"Hugos Window Class";

    WNDCLASS wndClass = {};
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hInstance = m_hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProc;

    RegisterClass(&wndClass);

    DWORD style = WS_OVERLAPPEDWINDOW;//WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    int width = 1000;
    int height = 500;

    RECT rect;
    rect.left = 100;
    rect.top = 100;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    m_hMainWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Main",
        style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );

    ShowWindow(m_hMainWnd, SW_SHOW);
}

Window::~Window()
{
    const wchar_t* CLASS_NAME = L"Hugos Window Class";

    UnregisterClass(CLASS_NAME, m_hInstance);
}

bool Window::ProcessMessages()
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

int main()
{
    char weekDay[3] = "  ";
    std::cout << "Opening Window." << std::endl;

    bool running = true;
    while (running)
    {
        time_t ttime = time(0);
        tm* local_time = localtime(&ttime);
        switch (local_time->tm_wday)
        {
            case 0: weekDay[0] = 'S'; weekDay[1] = 'U'; break;
            case 1: weekDay[0] = 'M'; weekDay[1] = 'O'; break;
            case 2: weekDay[0] = 'T'; weekDay[1] = 'U'; break;
            case 3: weekDay[0] = 'W'; weekDay[1] = 'E'; break;
            case 4: weekDay[0] = 'T'; weekDay[1] = 'H'; break;
            case 5: weekDay[0] = 'F'; weekDay[1] = 'R'; break;
            case 6: weekDay[0] = 'S'; weekDay[1] = 'A'; break;
        }

        std::cout << std::endl << local_time->tm_sec
            << "::" << local_time->tm_min
            << "::" << local_time->tm_hour;
            /* << "\t" << local_time->tm_mday
            << "::" << weekDay
            << "::" << local_time->tm_mon
            << "::" << 1900+local_time->tm_year;*/

        if (!pWindow->ProcessMessages())
        {
            std::cout << "Closing Window.";
            running = false;
        }
        Sleep(100);
    }
    
    delete pWindow;

    return true;
}
