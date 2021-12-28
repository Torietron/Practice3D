#include "DxLib.h"
#include <cstdint>
#include "ScreenControl.h"

static uint_fast16_t Key_Y = 0;

ScreenControl::ScreenControl(int_fast16_t w, int_fast16_t h, uint_fast8_t b, uint_fast16_t f)
:Width(w),Height(h),BitDepth(b),MaxFPS(f)
{
    WinMode = TRUE, New = TRUE, LimitFPS = FALSE, Cursor = FALSE;
    CursorH = 0;
    frame_count = 0, wait_time = 0, start_time = 0;
    limit = MaxFPS;
    average = 0.0f; 
}

int ScreenControl::Init()
{
    ChangeWindowMode(WinMode);
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);
	SetWindowSizeExtendRate(1.0);
    SetMouseDispFlag(Cursor);
	SetGraphMode(Width, Height, BitDepth, MaxFPS);
    SetDrawScreen(DX_SCREEN_BACK);
    return 0;
}

//Returns 1 on WinMode change
int ScreenControl::Update()
{
    if(CheckHitKey(KEY_INPUT_Y) == 1) Key_Y++;
    else Key_Y = 0;

    //Cursor Escape
    if(Key_Y == 1 && CursorH == 0)
    {
        if(Cursor == TRUE) Cursor = FALSE;
        else Cursor = TRUE;
        SetMouseDispFlag(Cursor);
    }
    if(Key_Y == 1 && CursorH != 0) //if cursor image is loaded
    {
        if(Cursor == TRUE) Cursor = FALSE;
        else Cursor = TRUE;
    }

    //Set Mode
    if(CheckHitKey(KEY_INPUT_RALT) == 1 && CheckHitKey(KEY_INPUT_RETURN) == 1) 
    {
        if(WinMode == FALSE) WinMode = TRUE;
        else WinMode = FALSE;
        New = TRUE;
    }

    //Handle New Screen
    if(New == TRUE)
    {
        ChangeWindowMode(WinMode);
        SetWindowSizeChangeEnableFlag(TRUE, TRUE);
        SetWindowSizeExtendRate(1.0);
        SetGraphMode(Width, Height, BitDepth, MaxFPS);
        SetDrawScreen(DX_SCREEN_BACK);
        if(WinMode == FALSE) Cursor = FALSE;
        if(CursorH != 0) SetMouseDispFlag(FALSE);
        else SetMouseDispFlag(Cursor);
        New = FALSE;
        
        return 1;
    }
    return 0;
}

void ScreenControl::CountFPS()
{
    if(frame_count == 0) start_time = GetNowCount();

    if(frame_count == MaxFPS)
    {
        end_time = GetNowCount();
        average = 1000.0f/((end_time-start_time)/(float)MaxFPS);
        frame_count = 0;
        start_time = end_time;
    }
    frame_count++;
}

void ScreenControl::DrawFPS(uint_fast8_t y)
{
    DrawFormatString(0,y,-1,"%.1f",average);
}

//Image Cursor
int ScreenControl::DrawCursor(int_fast32_t image, int_fast16_t padRight, int_fast16_t padLeft, int_fast16_t padBottom, int_fast16_t padTop)
{
    if(Cursor == TRUE && CursorH != 0) 
    {
        GetMousePoint(&cursor_x,&cursor_y);
        if(WinMode == FALSE)
        {
            if(cursor_x > Width - padRight)    cursor_x = Width - padRight;  
            if(cursor_x < 0 + padLeft)         cursor_x = padLeft;
            if(cursor_y > Height - padBottom)  cursor_y = Height - padBottom;
            if(cursor_y < 0 + padTop)          cursor_y = padTop;
        }
        DrawGraph(cursor_x,cursor_y,image,TRUE);

        return 1;
    }
    return 0;
}

void ScreenControl::SetFPSLimit(uint_fast16_t a)
{
    limit = a;
    if(limit > MaxFPS) limit = MaxFPS;
}

uint_fast16_t ScreenControl::GetFPSLimit()
{
    return limit;
}

void ScreenControl::Wait()
{
    time_spent = GetNowCount() - start_time;
    //ie: 60fps(16.6ms) - time spent
    wait_time = ((frame_count*1000)/limit) - time_spent;
    if(wait_time > 0) Sleep((DWORD)wait_time);
}

void ScreenControl::End()
{
    DxLib_End();
}

