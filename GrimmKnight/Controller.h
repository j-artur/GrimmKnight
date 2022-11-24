#ifndef _PROGJOGOS_CONTROLLER_H_
#define _PROGJOGOS_CONTROLLER_H_

#define INITGUID
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <list>
#include <string>
#include <xinput.h>
using std::list;
using std::string;

enum XboxCtrl
{
    DpadUp,
    DpadDown,
    DpadLeft,
    DpadRight,
    ButtonStart,
    ButtonBack,
    LeftThumb,
    RightThumb,
    LeftBumper,
    RightBumper,
    ButtonA,
    ButtonB,
    ButtonX,
    ButtonY,
    LeftTrigger,
    RightTrigger,
    ThumbLX,
    ThumbLY,
    ThumbRX,
    ThumbRY
};

enum XboxPlayer
{
    PLAYER1,
    PLAYER2,
    PLAYER3,
    PLAYER4
};

enum JoyAxis
{
    AxisX,
    AxisY,
    AxisZ,
    AxisRX,
    AxisRY,
    AxisRZ
};

struct JoyInfo
{
    GUID guid;
    string name;
};

struct JoyPart
{
    GUID guid;
    DWORD type;
    string name;
};

class Controller
{
  private:
    LPDIRECTINPUT8 dInput;
    LPDIRECTINPUTDEVICE8 joyDev;
    DIJOYSTATE joyState;
    XINPUT_STATE xboxState;
    XINPUT_VIBRATION vibration;
    bool joyCtrl[32];

    list<JoyPart> parts;
    list<JoyInfo> controllers;
    list<JoyInfo>::const_iterator selected;

  public:
    Controller();
    ~Controller();

    bool Initialize();
    bool XboxInitialize(int ctrl = 0);

    bool UpdateState();
    bool XboxUpdateState(int ctrl = 0);

    bool ButtonDown(int button);
    bool ButtonUp(int button);
    bool ButtonPress(int button);
    long Axis(int axis);
    long Slider(int slider);
    long Dpad(int pov = 0);

    bool DeviceNext();
    bool DevicePrev();
    const string *DeviceName();
    list<JoyPart> &DeviceParts();

    int XboxButton(int button);
    int XboxTrigger(int trigger);
    int XboxAnalog(int stick);
    void XboxVibrate(int ctrl = 0, int left = 65535, int right = 65535);
};

inline bool Controller::ButtonDown(int button)
{
    return joyState.rgbButtons[button] && 0x80;
}

inline bool Controller::ButtonUp(int button)
{
    return !(joyState.rgbButtons[button] && 0x80);
}

inline const string *Controller::DeviceName()
{
    return &(*selected).name;
}

inline list<JoyPart> &Controller::DeviceParts()
{
    return parts;
}

BOOL CALLBACK EnumerateControllers(LPCDIDEVICEINSTANCE lpDDi, LPVOID data);

BOOL CALLBACK EnumControllerParts(LPCDIDEVICEOBJECTINSTANCE lpDIdoi, LPVOID data);

#endif
