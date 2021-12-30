/***** WindowControl.h *****/
#ifndef DEF_SCREENCONTROL_H
#define DEF_SCREENCONTROL_H

#include "DxLib.h"
#include <cstdint>

typedef struct {
    int_fast16_t RootX, RootY;
    VECTOR Pos, Offset;
    char Label;
    bool Event, Enabled;
} Camera2D_t;

typedef struct Camera3D_t {
    float AngleH, AngleV, Roll, Zoom;
    float Anchor, OffsetH, OffsetV, OffsetR;
    VECTOR Pos;
    char Label;
    bool Event, Enabled;
    void Apply(){
        SetCameraPositionAndAngle(Pos,AngleV,AngleH,Roll);
    }
} Camera3D_t; 

class ScreenControl {
    public:
        uint_fast8_t WinMode, New, Cursor, LimitFPS, ShowFPS;
        uint_fast8_t BitDepth, CursorIndex;
        int_fast16_t Width, Height;
        uint_fast16_t MaxFPS;
        int_fast32_t CursorH[2], CursorH2[2];
        Camera2D_t C2D;
        Camera3D_t C3D;
        ScreenControl(int_fast16_t w, int_fast16_t h, uint_fast8_t b = 16, uint_fast16_t f = 60);
        int Init();
        int Update();
        void CountFPS();
        void DrawFPS();
        int DrawCursor(int_fast16_t padRight = 20, int_fast16_t padLeft = 2, int_fast16_t padBottom = 20, int_fast16_t padTop = 2);
        void SetFPSLocation(int_fast16_t x, int_fast16_t y);
        void SetFPSLimit(uint_fast16_t a);
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