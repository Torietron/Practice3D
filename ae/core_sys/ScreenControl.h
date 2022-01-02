/***** ScreenControl.h *****/
#ifndef DEF_SCREENCONTROL_H
#define DEF_SCREENCONTROL_H

#include "DxLib.h"
#include <cstdint>

typedef struct {
    int_fast16_t RootX, RootY;
    VECTOR Pos, Offset;
    char Label;
    bool Event, Enabled;
} _Camera2D_t;

typedef struct _Camera3D_t {
    float AngleH, AngleV, Roll, Zoom;
    float Anchor, OffsetH, OffsetV, OffsetR;
    VECTOR Pos;
    char Label;
    bool Event, Enabled;
    void Apply(){
        SetCameraPositionAndAngle(Pos,AngleV,AngleH,Roll);
    }
} _Camera3D_t; 

class ScreenControl { /* Manage screen modifications */
    public:
        uint_fast8_t WinMode, New, Cursor, LimitFPS, ShowFPS;
        uint_fast8_t BitDepth, CursorIndex;
        uint_fast16_t Width, Height, MaxFPS;
        int_fast32_t CursorH[2], CursorH2[2];
        _Camera2D_t C2D;
        _Camera3D_t C3D;
        ScreenControl(const uint_fast16_t w, const uint_fast16_t h, const uint_fast8_t b = 16, const uint_fast16_t f = 60);
        int Init();
        int Update();
        void CountFPS();
        void DrawFPS();
        int DrawCursor(const int_fast16_t &padRight = 20, const int_fast16_t &padLeft = 2, const int_fast16_t &padBottom = 20, const int_fast16_t &padTop = 2);
        void SetFPSLocation(const int_fast16_t &x, const int_fast16_t &y);
        void SetFPSLimit(const uint_fast16_t &a);
        uint_fast16_t GetFPSLimit();
        void Wait();
        void End();
    private:
        int_fast16_t cursor_x, cursor_y, fps_x, fps_y;
        uint_fast16_t frame_count, limit;
        int_fast32_t wait_time, time_spent;
        uint_fast32_t start_time, end_time; 
        float average;
};

#endif 