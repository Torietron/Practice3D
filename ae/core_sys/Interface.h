/***** Interface.h *****/
#ifndef DEF_INTERFACE_H
#define DEF_INTERFACE_H

#include "DxLib.h"
#include <cstdint>
#include <iostream>
#include "ModelData.h"

typedef struct { /* Use to create alternative option sets */
    int_fast16_t x, y;
    char Text[50];
    int_fast32_t Color;
} InterfaceOptions_t; 

class Interface { /* Manage Ui elements */
    public:
        uint_least8_t FluxReverse;
        uint_fast8_t Brightness, FontSize, FontThickness, FontType;; 
        int_fast16_t Selected;
        int_fast32_t White;     //-1 (raw values)
        int_fast32_t Black;     //-16777216
        int_fast32_t Red;       //-65536
        int_fast32_t Yellow;    //-256
        int_fast32_t Green;     //-16711936
        int_fast32_t Cyan;      //-16711681
        int_fast32_t Turquoise; //-12525360
        int_fast32_t Amber;     //-33280
        float Flux;
        InterfaceOptions_t Options[10];
        Interface(const int_fast16_t select = 0);
        void UpdateFlux(const float &fluxMax = 2.00f, const float &fluxMin = 0.00f, const float &fluxIncrement = 0.03f);
        void UpdateMenu(const uint_fast8_t &MAX_OPTIONS_TO_CYCLE, const int &KEY1 = KEY_INPUT_UP, const int &KEY2 = KEY_INPUT_DOWN);
        void DrawMenu(InterfaceOptions_t *options, const uint_fast8_t &MAX_OPTIONS_TO_DRAW, const int_fast32_t &selectedColor = -256, const int_fast32_t &baseColor = -1);
        void DrawBar(const int_fast16_t &x, const int_fast16_t &y, const double &numCurrent = 100.00f, const double &numMax = 100.00f, const double &scale = 1.00f, const int_fast16_t &w = 60, const int_fast16_t &h = 80, const uint_fast32_t &colorframe = -1, const uint_fast32_t &colorfill = -65536);
        void DrawMarker3D(Sprite3D_t &markerObj, const DxLib::VECTOR &targetPos, const float &yOffset = 16.0f, const float &xOffset = 0.0f, const float &zOffset = 0.0f, const float &centerX = 1.0f, const float &centerY = 1.0f);
        void Fade(const uint_fast8_t &ENUM_FADETYPE, const uint_fast8_t &ENUM_FADESPEED);
        void EndBlend();
        void DrawValue(const int_fast16_t &x, const int_fast16_t &y, const int &a, const int_fast32_t &color = -1);
        void DrawValue(const int_fast16_t &x, const int_fast16_t &y, const char &a, const int_fast32_t &color = -1);
        void DrawValue(const int_fast16_t &x, const int_fast16_t &y, const float &a, const int_fast32_t &color = -1);
        void DrawValue(const int_fast16_t &x, const int_fast16_t &y, const double &a, const int_fast32_t &color = -1);
        void DrawValue(const int_fast16_t &x, const int_fast16_t &y, const std::string &a, const int_fast32_t &color = -1); 
};  

typedef enum { //FADE_IN, FADE_OUT
    FADE_IN,
    FADE_OUT
} _InterfaceFadeType_t;

typedef enum { //SPEED1, SPEED2, SPEED3 -- (1,5,15) -- Clean divisions of 255
    SPEED1 = 1,
    SPEED2 = 5,
    SPEED3 = 15
} _InterfaceFadeSpeed_t;

#endif