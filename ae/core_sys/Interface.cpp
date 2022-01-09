#include "DxLib.h"
#include <iostream>
#include <cstdint>
#include <cmath>
#include "Interface.h"
#include "ModelData.h"

static char Keys[256];
static uint_fast16_t KeyPress[256];

Interface::Interface(const int_fast16_t select)
:Selected(select)
{
    White = GetColor(255,255,255);
    Black = GetColor(0,0,0);
    Red = GetColor(255,0,0);
    Yellow = GetColor(255,255,0);
    Green = GetColor(0,255,0);
    Cyan = GetColor(0,255,255);
    Turquoise = GetColor(64,224,208);
    Amber = GetColor(255,126,0);
    FontSize = 16;
    FontThickness = 7;
    FontType = DX_FONTTYPE_ANTIALIASING_EDGE_4X4;
    FluxReverse = FALSE;
    Flux = 0.0f;
}

//For use with fluctuating positions and elements
void Interface::UpdateFlux(const float &fluxMax, const float &fluxMin, const float &fluxIncrement)
{
    if(Flux > fluxMax) FluxReverse = TRUE;
    if(Flux <= fluxMin) FluxReverse = FALSE;
    if(FluxReverse == FALSE) Flux += fluxIncrement;
    else Flux -= fluxIncrement;
}

//DX keys /ie: KEY_INPUT_UP, KEY_INPUT_DOWN
void Interface::UpdateMenu(const uint_fast8_t &MAX_OPTIONS_TO_CYCLE, const int &KEY1, const int &KEY2)
{
    GetHitKeyStateAll(Keys);
    if(Keys[KEY1] != 0) KeyPress[KEY1]++;
    else KeyPress[KEY1] = 0;
    if(Keys[KEY2] != 0) KeyPress[KEY2]++;
    else KeyPress[KEY2] = 0;

    if(KeyPress[KEY1] == 1 || KeyPress[KEY2] == 1) 
    {
        if(KeyPress[KEY2] == 1) Selected = (Selected + 1) % MAX_OPTIONS_TO_CYCLE;
        if(KeyPress[KEY1] == 1) Selected = (Selected - 1) % MAX_OPTIONS_TO_CYCLE;
        if(KeyPress[KEY1] == 1 && Selected < 0) Selected = MAX_OPTIONS_TO_CYCLE - 1;
    }
}

void Interface::DrawMenu(InterfaceOptions_t *options, const uint_fast8_t &MAX_OPTIONS_TO_DRAW, const int_fast32_t &SelectedColor, const int_fast32_t &baseColor)
{
    for(uint_fast8_t i = 0; i < MAX_OPTIONS_TO_DRAW; i++)
    {
        if(i == Selected) 
        {
            options[i].Color = SelectedColor;
        }
        else
        {
            options[i].Color = baseColor;
        }
        DrawString(options[i].x,options[i].y,options[i].Text,options[i].Color);
        //DrawStringToHandle(100,100,"Text Here",GetColor(255,255,255),font00);
        //DrawStringToHandle(option[i].x,option[i].y,option[i].name,option[i].color,font00);
    }
}

//Center axis coords first, Rota is center by default
void Interface::DrawBar(const int_fast16_t &x, const int_fast16_t &y, const double &numCurrent, const double &numMax, const double &scale, const int_fast16_t &w, const int_fast16_t &h, const uint_fast32_t &colorframe, const uint_fast32_t &colorfill)
{
    //??? MY LAUGHABLE CEREAL TOY QUALITY FORMULA, no keyboard keys were harmed during the making of this 
    //simplified ratio = ((endP - startP) *val%) + startP
    //scalable ratio = (((endP+(baseWidth*objscale)) - (startP-(baseWidth*objscale))) *(cast to double)val%) + startP-(baseWidth*objscale) //pixel shift as needed
    DrawBox(x-(w*(int)round(scale)) ,y-2+(0-h*(int)round(scale)) ,x+(w*(int)round(scale)),y+2+(0-h*(int)round(scale)),colorframe,FALSE);
    DrawBox(x+1-(w*(int)round(scale)) ,y-1+(0-h*(int)round(scale)) ,(((x-1+(w*(int)round(scale))) - (x-(w*(int)round(scale)))) *(int)round((numCurrent/numMax))) + x-(w*(int)round(scale)), y+1+(0-h*(int)round(scale)),colorfill,TRUE); 
}

//A flat value added to Flux is an easy xyz offset
void Interface::DrawMarker3D(Sprite3D_t &markerObj, const DxLib::VECTOR &targetPos, const float &yOffset, const float &xOffset, const float &zOffset, const float &centerX, const float &centerY)
{
    markerObj.Body.Pos = VGet((targetPos.x + xOffset), (targetPos.y + yOffset), (targetPos.z + zOffset));
    DrawBillboard3D(markerObj.Body.Pos, centerX, centerY, markerObj.Size, markerObj.Angle, *markerObj.SpritePtr, TRUE);
}

void Interface::DrawValue(const int_fast16_t &x, const int_fast16_t &y, const int &a, const int_fast32_t &color)
{
    DrawFormatString(x,y,color,"%d",a);
}

void Interface::DrawValue(const int_fast16_t &x, const int_fast16_t &y, const char &a, const int_fast32_t &color)
{
    DrawFormatString(x,y,color,"%c",a);
}

void Interface::DrawValue(const int_fast16_t &x, const int_fast16_t &y, const float &a, const int_fast32_t &color)
{
    DrawFormatString(x,y,color,"%f",a);
}

void Interface::DrawValue(const int_fast16_t &x, const int_fast16_t &y, const double &a, const int_fast32_t &color)
{
    DrawFormatString(x,y,color,"%lf",a);
}

void Interface::DrawValue(const int_fast16_t &x, const int_fast16_t &y, const std::string &a, const int_fast32_t &color)
{
    DrawFormatString(x,y,color,"%s",a.c_str()); //actual strings had an issue, cstyle strings work fine. also converts STL vector strings
}
