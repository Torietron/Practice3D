#include "DxLib.h"
#include <cstdint>
#include "ScreenControl.h"

static uint_fast16_t KeyEscape = 0;

ScreenControl::ScreenControl(const uint_fast16_t w, const uint_fast16_t h, const uint_fast8_t b, const uint_fast16_t f)
:Width(w),Height(h),BitDepth(b),MaxFPS(f)
{
    ShowFPS = FALSE, LimitFPS = FALSE;
    WinMode = TRUE, New = TRUE, Cursor = FALSE;
    Brightness = 0;
    CursorIndex = 0;
    CursorH[0] = 0, CursorH[1] = 0; 
    CursorH2[0] = 0, CursorH2[1] = 0;
    frame_count = 0, wait_time = 0, start_time = 0;
    fps_x = 0, fps_y = 0;
    limit = MaxFPS;
    average = 0.0f; 
    C3D.OffsetV = 0.46f;
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
    if(CheckHitKey(KEY_INPUT_Y) == 1) KeyEscape++;
    else KeyEscape = 0;

    //Cursor Escape
    if(KeyEscape == 1 && CursorH[0] == 0)
    {
        if(Cursor == TRUE) Cursor = FALSE;
        else Cursor = TRUE;
        SetMouseDispFlag(Cursor);
    }
    if(KeyEscape == 1 && CursorH[0] != 0) //if cursor image is loaded
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
        if(CursorH[0] != 0) SetMouseDispFlag(FALSE);
        else SetMouseDispFlag(Cursor);

        if(C3D.Enabled)
        {
            SetCameraNearFar(0.1f, 1000.0f);
            SetUseZBuffer3D(TRUE);
            SetWriteZBuffer3D(TRUE);
            SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 10.0f, -20.0f),VGet(0.0f, 10.0f, 0.0f));
        }

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

void ScreenControl::DrawFPS()
{
    DrawFormatString(fps_x,fps_y,-1,"%.1f",average);
}

/*Image Cursor
  Draws if Cursor is TRUE and an image is loaded in CursorH
*/
int ScreenControl::DrawCursor(const int_fast16_t &padRight, const int_fast16_t &padLeft, const int_fast16_t &padBottom, const int_fast16_t &padTop)
{
    if(Cursor == TRUE && CursorH[0] != 0) 
    {
        GetMousePoint(&cursor_x,&cursor_y);
        if(WinMode == FALSE)
        {
            if(cursor_x > (int_fast16_t)Width - padRight)    cursor_x = Width - padRight;  
            if(cursor_x < 0 + padLeft)         cursor_x = padLeft;
            if(cursor_y > (int_fast16_t)Height - padBottom)  cursor_y = Height - padBottom;
            if(cursor_y < 0 + padTop)          cursor_y = padTop;
        }
        DrawGraph(cursor_x,cursor_y,CursorH[CursorIndex],TRUE);

        return 1;
    }
    return 0;
}

/*  Fade Types: FADE_IN, FADE_OUT  
    Fade Speeds: SPEED1, SPEED2, SPEED3 
    -- BlendEffect, turn-off/finish effect with EndBlend()
*/
void ScreenControl::Fade(const uint_fast8_t &ENUM_FADETYPE, const uint_fast8_t &ENUM_FADESPEED)
{
    switch(ENUM_FADETYPE)
    {
        case 0:
            if(Brightness <= 254) Brightness = Brightness + ENUM_FADESPEED;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, Brightness);  
            break;
        case 1:
            if(Brightness >= 1) Brightness = Brightness - ENUM_FADESPEED; 
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, Brightness); 
            break;
    }
}

//Call after using any BlendEffects to turn-off/finish blend for subsequent draw calls
void ScreenControl::EndBlend()
{
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ScreenControl::SetFPSLocation(const int_fast16_t &x, const int_fast16_t &y)
{
    fps_x = x;
    fps_y = y;
}

void ScreenControl::SetFPSLimit(const uint_fast16_t &a)
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

