#include "DxLib.h"
#include <cstdint>
#include "MousePoll.h"

static uint_fast32_t dTime, mTime = 0;

MousePoll::MousePoll()
{
    x = 0, y = 0;
    for(uint_fast16_t i = 0; i < 4; i++)
    {
        Poll[i] = 0;
    }
}

//L == 1, R == 2, dual click is 3, 0 is unpressed, greater than zero is held
int MousePoll::Update(uint_fast16_t rate)
{
    dTime = GetNowCount();
    GetMousePoint(&x,&y);
    if(dTime - mTime > 100) 
    {
        last_x = x;
        last_y = y;
        mTime = GetNowCount();
    }
    
    int_fast8_t tempclick = GetMouseInput();
    for(uint_fast8_t i = 0; i < 4; i++)
    {
        if(tempclick == i)
        {
            Poll[i]++;
        }
        else Poll[i] = 0;
    }
    return 0;
}

bool MousePoll::Moved()
{
    if(last_x == x && last_y == y) return false;
    else
    {
        delta_x = (last_x - x);
        delta_y = (last_y - y);
        if(delta_x < 0) distance_x = delta_x * -1;
        else distance_x = delta_x;
        if(delta_y < 0) distance_y = delta_y * -1;
        else distance_y = delta_y;
        return true;
    }
}

int_fast16_t MousePoll::GetDeltaX()
{
    return delta_x;
}

int_fast16_t MousePoll::GetDeltaY()
{
    return delta_y;
}

void MousePoll::Reset(int_fast16_t a, int_fast16_t b)
{
    SetMousePoint(a,b);
    x = a;
    y = b;
    last_x = x;
    last_y = y;
    delta_x = 0;
    delta_y = 0;
    distance_x = 0;
    distance_y = 0;
    mTime = 0;
}