/***** WindowControl.h *****/
#ifndef DEF_SCREENCONTROL_H
#define DEF_SCREENCONTROL_H

#include <cstdint>
class ScreenControl {
    public: 
        uint_fast8_t WinMode, New, Lock, BitDepth, ShowCursor;
        int_fast16_t Width, Height; 
        uint_fast16_t TargetFPS;
        ScreenControl(int_fast16_t W, int_fast16_t H);
        int Init();
        int Update();
        void CountFps();
        void DrawFps(uint_fast8_t y = 0);
        void Wait();
        void End();
    private:
        uint_fast16_t frame_count;
        int_fast32_t wait_time, time_spent;
        uint_fast32_t start_time, end_time; 
        float average;
};

#endif 