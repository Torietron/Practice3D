#include "DxLib.h"
#include <cstdint>
#include "ScreenControl.h"

ScreenControl::ScreenControl(int_fast16_t w, int_fast16_t h)
:Width(w),Height(h)
{
    WinMode = FALSE, New = TRUE, Lock = FALSE, ShowCursor = FALSE;
    TargetFPS = 60, BitDepth = 16;
    frame_count = 0, wait_time = 0, start_time = 0;
    average = 0.0f; 
}

int ScreenControl::Init()
{
    //ChangeWindowMode(WinMode);
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);
	SetWindowSizeExtendRate(1.0);
	SetGraphMode(Width, Height, BitDepth, TargetFPS*2);
    return 0;
}

int ScreenControl::Update()
{
    if(CheckHitKey(KEY_INPUT_LALT) == 1 && WinMode == FALSE)
    {
        if(ShowCursor == TRUE) ShowCursor = FALSE;
        else ShowCursor = TRUE;
        SetMouseDispFlag(ShowCursor);
    }
    if(CheckHitKey(KEY_INPUT_RALT) == 1 && CheckHitKey(KEY_INPUT_RETURN) == 1) 
    {
        if(WinMode == FALSE) WinMode = TRUE;
        else WinMode = FALSE;
        New = TRUE;
    }

    if(New == TRUE)
    {
        if(WinMode == TRUE)
        {
            ChangeWindowMode(WinMode);
            //SetGraphMode(Width, Height, BitDepth, TargetFPS*2);
            //SetMouseDispFlag(FALSE);
            New = FALSE;
        }
        else if(WinMode == FALSE)
        {
            ChangeWindowMode(WinMode);
            //SetGraphMode(Width, Height, BitDepth, TargetFPS*2);
            //SetMouseDispFlag(FALSE);
            New = FALSE;
        }
        return 1;
    }
    return 0;
}

void ScreenControl::CountFps()
{
    if(frame_count == 0) start_time = GetNowCount();

    if(frame_count == TargetFPS)
    {
        end_time = GetNowCount();
        average = 1000.0f/((end_time-start_time)/(float)TargetFPS);
        frame_count = 0;
        start_time = end_time;
    }
    frame_count++;
}

void ScreenControl::DrawFps(uint_fast8_t y)
{
    DrawFormatString(0,y,-1,"%.1f",average);
}

void ScreenControl::Wait()
{
    time_spent = GetNowCount() - start_time;
    //ie: 60fps(16.6ms) - time spent
    wait_time = ((frame_count*1000)/TargetFPS) - time_spent;
    if(wait_time > 0) Sleep((DWORD)wait_time);
}

void ScreenControl::End()
{
    DxLib_End();
}

