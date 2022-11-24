#ifndef _PROGJOGOS_WINDOW_H_
#define _PROGJOGOS_WINDOW_H_

#include "Types.h"
#include <string>
#include <windows.h>
#include <windowsx.h>
using std::string;

enum WindowModes
{
    FULLSCREEN,
    WINDOWED,
    BORDERLESS
};

class Window
{
  private:
    HINSTANCE hInstance;
    HWND windowHandle;
    int windowWidth;
    int windowHeight;
    HICON windowIcon;
    HCURSOR windowCursor;
    COLORREF windowColor;
    string windowTitle;
    DWORD windowStyle;
    int windowMode;
    int windowPosX;
    int windowPosY;
    float windowCenterX;
    float windowCenterY;
    static bool windowKeys[256];
    static bool windowCtrl[256];
    static int windowMouseX;
    static int windowMouseY;
    static int windowMouseWheel;

  public:
    Window();

    HINSTANCE AppId();
    HWND Id();
    int Width();
    int Height();

    void Icon(const uint icon);
    void Cursor(const uint cursor);
    void Title(const string title);
    void Size(int width, int height);
    void Mode(int mode);

    int Mode() const;
    float CenterX() const;
    float CenterY() const;
    string Title() const;

    void HideCursor(bool hide);
    void Close();

    bool KeyDown(int vkcode);
    bool KeyUp(int vkcode);
    bool KeyPress(int vkcode);
    float MouseX();
    float MouseY();
    int MouseWheel();

    COLORREF Color();
    void Color(int r, int g, int b);
    bool Create();

    static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

inline HINSTANCE Window::AppId()
{
    return hInstance;
}

inline HWND Window::Id()
{
    return windowHandle;
}

inline int Window::Width()
{
    return windowWidth;
}

inline int Window::Height()
{
    return windowHeight;
}

inline void Window::Icon(const uint icon)
{
    windowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(icon));
}

inline void Window::Cursor(const uint cursor)
{
    windowCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(cursor));
}

inline void Window::Title(const string title)
{
    windowTitle = title;
}

inline int Window::Mode() const
{
    return windowMode;
}

inline float Window::CenterX() const
{
    return windowCenterX;
}

inline float Window::CenterY() const
{
    return windowCenterY;
}

inline string Window::Title() const
{
    return windowTitle;
}

inline void Window::HideCursor(bool hide)
{
    ShowCursor(!hide);
}

inline void Window::Close()
{
    PostMessage(windowHandle, WM_DESTROY, 0, 0);
}

inline bool Window::KeyDown(int vkcode)
{
    return windowKeys[vkcode];
}

inline bool Window::KeyUp(int vkcode)
{
    return !(windowKeys[vkcode]);
}

inline float Window::MouseX()
{
    return float(windowMouseX);
}

inline float Window::MouseY()
{
    return float(windowMouseY);
}

inline int Window::MouseWheel()
{
    return windowMouseWheel;
}

inline COLORREF Window::Color()
{
    return windowColor;
}

inline void Window::Color(int r, int g, int b)
{
    windowColor = RGB(r, g, b);
}

#endif
