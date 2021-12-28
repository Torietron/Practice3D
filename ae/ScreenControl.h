/***** WindowControl.h *****/
#ifndef DEF_SCREENCONTROL_H
#define DEF_SCREENCONTROL_H

#include <cstdint>
class ScreenControl {
    public: 
        uint_fast8_t WinMode, New, LimitFPS, BitDepth, Cursor;
        int_fast16_t Width, Height; 
        uint_fast16_t MaxFPS;
        int_fast32_t CursorH;
        ScreenControl(int_fast16_t w, int_fast16_t h, uint_fast8_t b = 16, uint_fast16_t f = 60);
        int Init();
        int Update();
        void CountFPS();
        void DrawFPS(uint_fast8_t y = 0);
        void DrawCursor(int_fast32_t image, int_fast16_t padRight = 20, int_fast16_t padLeft = 2, int_fast16_t padBottom = 20, int_fast16_t padTop = 2);
        void SetFPSLimit(uint_fast16_t a);
        uint_fast16_t GetFPSLimit();
        void Wait();
        void End();
    private:
        int_fast16_t x, y;
        uint_fast16_t frame_count, limit;
        int_fast32_t wait_time, time_spent;
        uint_fast32_t start_time, end_time; 
        float average;
};

#endif 