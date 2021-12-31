/***** Interface.h *****/
#ifndef DEF_INTERFACE_H
#define DEF_INTERFACE_H

#include "DxLib.h"
#include <cstdint>
#include <iostream>

//assign char at declaration
typedef struct {
    int_fast16_t x, y;
    char Text[50];
    int_fast32_t Color;
} InterfaceOptions_t;

class Interface {
    public:
        uint_least8_t FluxReverse;
        int_fast8_t Selected;
        uint_fast8_t Brightness;
        int_fast32_t White;     //-1 (raw values)
        int_fast32_t Black;     //-16777216
        int_fast32_t Red;       //-65536
        int_fast32_t Yellow;    //-256
        int_fast32_t Green;     //-16711936
        int_fast32_t Cyan;      //-16711681
        int_fast32_t Turquoise; //-12525360
        int_fast32_t Amber;     //-33280
        uint_fast8_t FontSize, FontThickness, FontType;
        float Flux;
        InterfaceOptions_t Options;
        Interface(int_fast8_t a = 0);
        void UpdateMenu(const uint_fast8_t MAX, const int KEY1 = KEY_INPUT_UP, const int KEY2 = KEY_INPUT_DOWN);
        void DrawMenu(InterfaceOptions_t *option, const uint_fast8_t MAX, int_fast32_t selectedColor = GetColor(255,255,0), int_fast32_t baseColor = GetColor(255,255,255));
        void DrawBar(int_fast16_t x,int_fast16_t y,double numCurrent = 100.00,double numMax = 100.00,double scale = 1.00,int_fast16_t w = 60,int_fast16_t h = 80,uint_fast32_t colorframe = GetColor(255,255,255),uint_fast32_t colorfill = GetColor(255,0,0));
        //void DrawImgBar();
        void DrawMarker3D(const DxLib::VECTOR &targetPos, DxLib::VECTOR &markerPos, const float &markerSize, const int &markerH, const float &heightOffset = 16.0f , const float &markerRot = 0.0f, const float &cx = 1.0f, const float &cy = 1.0f);
        void Fade(const uint_fast8_t TYPE, const uint_fast8_t SPEED);
        void EndBlend();
        void DrawValue(int_fast16_t x, int_fast16_t y, int a, int_fast32_t color = GetColor(255,255,255));
        void DrawValue(int_fast16_t x, int_fast16_t y, char a, int_fast32_t color = GetColor(255,255,255));
        void DrawValue(int_fast16_t x, int_fast16_t y, float a, int_fast32_t color = GetColor(255,255,255));
        void DrawValue(int_fast16_t x, int_fast16_t y, double a, int_fast32_t color = GetColor(255,255,255));
        void DrawValue(int_fast16_t x, int_fast16_t y, std::string a, int_fast32_t color = GetColor(255,255,255)); 
        //void DrawImgMap();
        //void DrawButton();
        //void DrawSlider();
        //void DrawImgSlider();
};  

typedef enum {
    FADE_IN,
    FADE_OUT
} InterfaceFadeType_t;

typedef enum {
    SPEED1 = 1,
    SPEED2 = 5,
    SPEED3 = 15
} InterfaceFadeSpeed_t; //clean divisions of 255


#endif