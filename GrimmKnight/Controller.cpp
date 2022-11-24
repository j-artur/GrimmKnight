#include "Controller.h"
#include "Engine.h"
#include <cmath>

Controller::Controller()
{
    dInput = nullptr;
    joyDev = nullptr;

    ZeroMemory(&joyState, sizeof(joyState));
    ZeroMemory(&joyCtrl, sizeof(joyCtrl));
    ZeroMemory(&xboxState, sizeof(XINPUT_STATE));
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dInput, NULL);

    dInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumerateControllers, &controllers, DIEDFL_ATTACHEDONLY);

    selected = controllers.begin();
}

Controller::~Controller()
{
    if (joyDev)
        joyDev->Unacquire();

    if (joyDev)
        joyDev->Release();

    if (dInput)
        dInput->Release();
}

bool Controller::Initialize()
{
    if (!dInput)
        return false;

    if (controllers.empty())
        return false;

    if (joyDev)
    {
        joyDev->Unacquire();
        joyDev->Release();
    }

    if (FAILED(dInput->CreateDevice(selected->guid, &joyDev, NULL)))
        return false;

    if (FAILED(joyDev->SetCooperativeLevel(Engine::window->Id(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
        return false;

    if (FAILED(joyDev->SetDataFormat(&c_dfDIJoystick)))
        return false;

    parts.clear();
    if (FAILED(joyDev->EnumObjects(EnumControllerParts, &parts, DIDFT_ALL)))
        return false;

    DIPROPRANGE axisRange;
    DIPROPDWORD deadZone;

    for (const auto &p : parts)
    {
        if (p.type & DIDFT_AXIS)
        {
            axisRange.diph.dwSize = sizeof(DIPROPRANGE);
            axisRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
            axisRange.diph.dwHow = DIPH_BYID;
            axisRange.diph.dwObj = p.type;
            axisRange.lMin = -1000;
            axisRange.lMax = +1000;

            if (FAILED(joyDev->SetProperty(DIPROP_RANGE, &axisRange.diph)))
                return false;

            deadZone.diph.dwSize = sizeof(DIPROPDWORD);
            deadZone.diph.dwHeaderSize = sizeof(DIPROPHEADER);
            deadZone.diph.dwHow = DIPH_BYID;
            deadZone.diph.dwObj = p.type;
            deadZone.dwData = 2000;

            if (FAILED(joyDev->SetProperty(DIPROP_DEADZONE, &deadZone.diph)))
                return false;
        }
    }

    if (FAILED(joyDev->Acquire()))
        return false;

    return true;
}

bool Controller::XboxInitialize(int ctrl)
{
    XINPUT_CAPABILITIES caps;
    ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));

    XInputGetCapabilities(ctrl, XINPUT_FLAG_GAMEPAD, &caps);

    if (caps.SubType == XINPUT_DEVSUBTYPE_UNKNOWN)
        return false;

    return true;
}

bool Controller::UpdateState()
{
    if (FAILED(joyDev->Poll()))
        return false;

    HRESULT result = joyDev->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&joyState);

    if (FAILED(result))
    {
        if (result == DIERR_NOTACQUIRED || result == DIERR_INPUTLOST)
        {
            joyDev->Unacquire();

            joyDev->Acquire();

            joyDev->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&joyState);
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool Controller::XboxUpdateState(int ctrl)
{
    if (XInputGetState(ctrl, &xboxState))
        return false;

    return true;
}

bool Controller::ButtonPress(int button)
{
    if (joyCtrl[button])
    {
        if (ButtonDown(button))
        {
            joyCtrl[button] = false;
            return true;
        }
    }
    else if (ButtonUp(button))
    {
        joyCtrl[button] = true;
    }

    return false;
}

long Controller::Axis(int axis)
{
    switch (axis)
    {
    case AxisX:
        return joyState.lX;
    case AxisY:
        return joyState.lY;
    case AxisZ:
        return joyState.lZ;
    case AxisRX:
        return joyState.lRx;
    case AxisRY:
        return joyState.lRy;
    case AxisRZ:
        return joyState.lRz;

    default:
        return 0;
    }
}

long Controller::Slider(int slider)
{
    return joyState.rglSlider[slider];
}

long Controller::Dpad(int pov)
{
    return joyState.rgdwPOV[pov];
}

bool Controller::DeviceNext()
{
    ++selected;

    if (selected == controllers.end())
        selected = controllers.begin();

    if (Initialize())
        return true;
    else
        return false;
}

bool Controller::DevicePrev()
{
    if (selected == controllers.begin())
        selected = controllers.end();

    --selected;

    if (Initialize())
        return true;
    else
        return false;
}

BOOL CALLBACK EnumerateControllers(LPCDIDEVICEINSTANCE lpDDi, LPVOID data)
{
    JoyInfo joy;

    joy.guid = lpDDi->guidInstance;

    joy.name = lpDDi->tszInstanceName;

    ((list<JoyInfo> *)data)->push_back(joy);

    return DIENUM_CONTINUE;
}

BOOL CALLBACK EnumControllerParts(LPCDIDEVICEOBJECTINSTANCE lpDIdoi, LPVOID data)
{
    JoyPart part;

    part.guid = lpDIdoi->guidType;
    part.type = lpDIdoi->dwType;
    part.name = lpDIdoi->tszName;

    if (lpDIdoi->guidType != GUID_Unknown)
        ((list<JoyPart> *)data)->push_back(part);

    return DIENUM_CONTINUE;
}

int Controller::XboxButton(int button)
{
    switch (button)
    {
    case DpadUp:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
        break;
    case DpadDown:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
        break;
    case DpadLeft:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
        break;
    case DpadRight:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
        break;
    case ButtonStart:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_START);
        break;
    case ButtonBack:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
        break;
    case LeftThumb:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
        break;
    case RightThumb:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
        break;
    case LeftBumper:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
        break;
    case RightBumper:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
        break;
    case ButtonA:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_A);
        break;
    case ButtonB:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_B);
        break;
    case ButtonX:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_X);
        break;
    case ButtonY:
        return (xboxState.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
        break;
    }

    return false;
}

int Controller::XboxTrigger(int trigger)
{
    switch (trigger)
    {
    case LeftTrigger:
        return xboxState.Gamepad.bLeftTrigger;
        break;
    case RightTrigger:
        return xboxState.Gamepad.bRightTrigger;
        break;
    }

    return 0;
}

int Controller::XboxAnalog(int stick)
{
    switch (stick)
    {
    case ThumbLX:
        if (abs(xboxState.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            return xboxState.Gamepad.sThumbLX;
        break;
    case ThumbLY:
        if (abs(xboxState.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            return xboxState.Gamepad.sThumbLY;
        break;
    case ThumbRX:
        if (abs(xboxState.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
            return xboxState.Gamepad.sThumbRX;
        break;
    case ThumbRY:
        if (abs(xboxState.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
            return xboxState.Gamepad.sThumbRY;
        break;
    }

    return 0;
}

void Controller::XboxVibrate(int ctrl, int left, int right)
{
    vibration.wLeftMotorSpeed = left;
    vibration.wRightMotorSpeed = right;
    XInputSetState(ctrl, &vibration);
}
