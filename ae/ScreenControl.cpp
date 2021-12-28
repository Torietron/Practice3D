#include "DxLib.h"
#include <cstdint>
#include "ScreenControl.h"

static uint_fast16_t Key_Y = 0;

ScreenControl::ScreenControl(int_fast16_t w, int_fast16_t h, uint_fast8_t b, uint_fast16_t f)
:Width(w),Height(h),BitDepth(b),TargetFPS(f)
{
    WinMode = TRUE, New = TRUE, Lock = FALSE, Cursor = FALSE;
    frame_count = 0, wait_time = 0, start_time = 0;
    average = 0.0f; 
}

int ScreenControl::Init()
{
    ChangeWindowMode(WinMode);
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);
	SetWindowSizeExtendRate(1.0);
    SetMouseDispFlag(Cursor);
	SetGraphMode(Width, Height, BitDepth, TargetFPS);
    SetDrawScreen(DX_SCREEN_BACK);
    return 0;
}

//Returns 1 on WinMode change
int ScreenControl::Update()
{
    if(CheckHitKey(KEY_INPUT_Y) == 1) Key_Y++;
    else Key_Y = 0;

    if(Key_Y == 1 && WinMode == TRUE)
    {
        if(Cursor == TRUE) Cursor = FALSE;
        else Cursor = TRUE;
        SetMouseDispFlag(Cursor);
    }

    if(CheckHitKey(KEY_INPUT_RALT) == 1 && CheckHitKey(KEY_INPUT_RETURN) == 1) 
    {
        if(WinMode == FALSE) WinMode = TRUE;
        else WinMode = FALSE;
        New = TRUE;
    }

    if(New == TRUE)
    {
        ChangeWindowMode(WinMode);
        SetWindowSizeChangeEnableFlag(TRUE, TRUE);
        SetWindowSizeExtendRate(1.0);
        SetGraphMode(Width, Height, BitDepth, TargetFPS);
        SetDrawScreen(DX_SCREEN_BACK);
        if(WinMode == FALSE) Cursor = FALSE;
        SetMouseDispFlag(Cursor);
        New = FALSE;
        
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

