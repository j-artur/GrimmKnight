#include "Window.h"
#include "Engine.h"

bool Window::windowKeys[256] = {0};
bool Window::windowCtrl[256] = {0};
int Window::windowMouseX = 0;
int Window::windowMouseY = 0;
int Window::windowMouseWheel = 0;

Window::Window()
{
    hInstance = GetModuleHandle(NULL);
    windowHandle = 0;
    windowWidth = GetSystemMetrics(SM_CXSCREEN);
    windowHeight = GetSystemMetrics(SM_CYSCREEN);
    windowIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowCursor = LoadCursor(NULL, IDC_ARROW);
    windowColor = RGB(0, 0, 0);
    windowTitle = string("Windows Game");
    windowStyle = WS_POPUP | WS_VISIBLE;
    windowMode = FULLSCREEN;
    windowPosX = 0;
    windowPosY = 0;
    windowCenterX = windowWidth / 2.0f;
    windowCenterY = windowHeight / 2.0f;
}

void Window::Mode(int mode)
{
    windowMode = mode;

    if (mode == WINDOWED)
    {
        windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
    }
    else
    {
        windowStyle = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
    }
}

void Window::Size(int width, int height)
{
    windowWidth = width;
    windowHeight = height;

    windowCenterX = windowWidth / 2.0f;
    windowCenterY = windowHeight / 2.0f;

    windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - windowWidth / 2;
    windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - windowHeight / 2;
}

bool Window::KeyPress(int vkcode)
{
    if (windowCtrl[vkcode])
    {
        if (KeyDown(vkcode))
        {
            windowCtrl[vkcode] = false;
            return true;
        }
    }
    else if (KeyUp(vkcode))
    {
        windowCtrl[vkcode] = true;
    }

    return false;
}

bool Window::Create()
{
    WNDCLASSEX wndClass;

    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = Window::WinProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = windowIcon;
    wndClass.hCursor = windowCursor;
    wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(windowColor);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = "GameWindow";
    wndClass.hIconSm = windowIcon;

    if (!RegisterClassEx(&wndClass))
        return false;

    windowHandle = CreateWindowEx(NULL, "GameWindow", windowTitle.c_str(), windowStyle, windowPosX, windowPosY,
                                  windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

    if (windowMode == WINDOWED)
    {
        RECT winRect = {0, 0, windowWidth, windowHeight};

        AdjustWindowRectEx(&winRect, GetWindowStyle(windowHandle), GetMenu(windowHandle) != NULL,
                           GetWindowExStyle(windowHandle));

        windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - (winRect.right - winRect.left) / 2;
        windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - (winRect.bottom - winRect.top) / 2;

        MoveWindow(windowHandle, windowPosX, windowPosY, winRect.right - winRect.left, winRect.bottom - winRect.top,
                   TRUE);
    }

    return (windowHandle ? true : false);
}

LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_MOUSEMOVE:
        windowMouseX = GET_X_LPARAM(lParam);
        windowMouseY = GET_Y_LPARAM(lParam);
        return 0;
    case WM_MOUSEWHEEL:
        windowMouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
        return 0;
    case WM_KEYDOWN:
        windowKeys[wParam] = true;
        return 0;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        windowKeys[VK_LBUTTON] = true;
        return 0;
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
        windowKeys[VK_MBUTTON] = true;
        return 0;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
        windowKeys[VK_RBUTTON] = true;
        return 0;
    case WM_KEYUP:
        windowKeys[wParam] = false;
        return 0;
    case WM_LBUTTONUP:
        windowKeys[VK_LBUTTON] = false;
        return 0;
    case WM_MBUTTONUP:
        windowKeys[VK_MBUTTON] = false;
        return 0;
    case WM_RBUTTONUP:
        windowKeys[VK_RBUTTON] = false;
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
