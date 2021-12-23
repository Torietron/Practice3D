#include "DxLib.h"
#include <cstdint>
#include "MousePoll.h"

MousePoll::MousePoll()
{
    x = 0;
    y = 0;
    dTime = 0;
    for(uint_fast16_t i = 0; i < 4; i++)
    {
        Poll[i] = 0;
    }
}

//L == 1, R == 2, dual click is 3, 0 is unpressed, greater than zero is held
int MousePoll::Update()
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
    return 0;
}