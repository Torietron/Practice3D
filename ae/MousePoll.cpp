#include "DxLib.h"
#include <cstdint>
#include "MousePoll.h"

static uint_fast32_t dTime, mTime = 0;

MousePoll::MousePoll(float decay)
:SMOOTH_DECAY(decay)
{
    x = 0, y = 0;
    delta_x = 0, delta_y = 0;
    posdelta_x = 0, posdelta_y = 0;
    last_x = 0, last_y = 0;
    for(uint_fast16_t i = 0; i < 4; i++)
    {
        Poll[i] = 0;
    }
}

//L == 1, R == 2, dual click is 3, 0 is unpressed, greater than zero is held
int MousePoll::Update(uint_fast16_t rate)
{
    //Movement
    {
        dTime = GetNowCount();
        last_x = x; 
        last_y = y;
        GetMousePoint(&x,&y);

        if(dTime - mTime > rate)
        {
            if(delta_x > -0.30f && delta_x < 0.30f)
            {
                delta_x = (float)(last_x - x);
                delta_y = (float)(last_y - y);
                mTime = GetNowCount();    
            } 
        }

        delta_x = delta_x * SMOOTH_DECAY;
        delta_y = delta_y * SMOOTH_DECAY;
    }
    
    //Clicks
    {
        int_fast8_t tempclick = GetMouseInput();
        for(uint_fast8_t i = 0; i < 4; i++)
        {
            if(tempclick == i)
            {
                Poll[i]++;
            }
            else Poll[i] = 0;
        }
    }
    return 0;
}

bool MousePoll::Moved()
{
    if(last_x == x && last_y == y) return false;
    else
    {
        delta_x = (float)(last_x - x);
        delta_y = (float)(last_y - y);
        if(delta_x < 0) posdelta_x = delta_x * -1;
        else posdelta_x = delta_x;
        if(delta_y < 0) posdelta_y = delta_y * -1;
        else posdelta_y = delta_y;
        return true;
    }
}

float MousePoll::GetDeltaX(uint_fast8_t type)
{
    switch(type)
    {
        case 0:
            return delta_x;
        case 1:
            return posdelta_x;
        default:
            break;
    }
    return delta_x;
}

float MousePoll::GetDeltaY(uint_fast8_t type)
{
    switch(type)
    {
        case 0:
            return delta_y;
        case 1:
            return posdelta_y;
        default:
            break;
    }
    return delta_y;
}

void MousePoll::Reset(int_fast16_t a, int_fast16_t b)
{
    SetMousePoint(a,b);
    GetMousePoint(&x,&y);
    last_x = x;
    last_y = y;
    delta_x = 0.0f;
    delta_y = 0.0f;
    posdelta_x = 0.0f;
    posdelta_y = 0.0f;
}