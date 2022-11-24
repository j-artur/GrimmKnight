#include "Timer.h"
#include "Types.h"

Timer::Timer()
{
    QueryPerformanceFrequency(&freq);

    ZeroMemory(&start, sizeof(start));
    ZeroMemory(&end, sizeof(end));

    stoped = false;
}

void Timer::Start()
{
    if (stoped)
    {
        llong elapsed = end.QuadPart - start.QuadPart;

        QueryPerformanceCounter(&start);
        start.QuadPart -= elapsed;

        stoped = false;
    }
    else
    {
        QueryPerformanceCounter(&start);
    }
}

void Timer::Stop()
{
    if (!stoped)
    {
        QueryPerformanceCounter(&end);
        stoped = true;
    }
}

float Timer::Reset()
{
    llong elapsed;

    if (stoped)
    {
        elapsed = end.QuadPart - start.QuadPart;

        QueryPerformanceCounter(&start);

        stoped = false;
    }
    else
    {
        QueryPerformanceCounter(&end);

        elapsed = end.QuadPart - start.QuadPart;

        start = end;
    }

    return float(elapsed / double(freq.QuadPart));
}

float Timer::Elapsed()
{
    llong elapsed;

    if (stoped)
    {
        elapsed = end.QuadPart - start.QuadPart;
    }
    else
    {
        QueryPerformanceCounter(&end);

        elapsed = end.QuadPart - start.QuadPart;
    }

    return float(elapsed / double(freq.QuadPart));
}

llong Timer::Stamp()
{
    QueryPerformanceCounter(&end);
    return end.QuadPart;
}

float Timer::Elapsed(llong stamp)
{
    llong elapsed;

    if (stoped)
    {
        elapsed = end.QuadPart - stamp;
    }
    else
    {
        QueryPerformanceCounter(&end);

        elapsed = end.QuadPart - stamp;
    }

    return float(elapsed / double(freq.QuadPart));
}
