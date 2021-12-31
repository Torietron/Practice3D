#include "DxLib.h"
#include <iostream>
#include <cstdint>
#include "Interface.h"

static char Keys[256];
static uint_fast16_t KeyPress[256];

Interface::Interface(int_fast8_t a)
:Selected(a)
{
    Brightness = 0;
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
}

void Interface::UpdateMenu(const uint_fast8_t MAX, const int KEY1, const int KEY2)
{
    GetHitKeyStateAll(Keys);
    if(Keys[KEY1] != 0) KeyPress[KEY1]++;
    else KeyPress[KEY1] = 0;
    if(Keys[KEY2] != 0) KeyPress[KEY2]++;
    else KeyPress[KEY2] = 0;

    if(KeyPress[KEY1] == 1 || KeyPress[KEY2] == 1) 
    {
        if(KeyPress[KEY2] == 1) Selected = (Selected + 1) % MAX;
        if(KeyPress[KEY1] == 1) Selected = (Selected - 1) % MAX;
        if(KeyPress[KEY1] == 1 && Selected < 0) Selected = MAX - 1;
    }
}

void Interface::DrawMenu(InterfaceOptions_t *option, const uint_fast8_t MAX, int_fast32_t SelectedColor, int_fast32_t baseColor)
{
    for(uint_fast8_t i = 0; i < MAX; i++)
    {
        if(i == Selected) 
        {
            option[i].Color = SelectedColor;
        }
        else
        {
            option[i].Color = baseColor;
        }
        DrawString(option[i].x,option[i].y,option[i].Text,option[i].Color);
        //DrawStringToHandle(100,100,"Text Here",GetColor(255,255,255),font00);
        //DrawStringToHandle(option[i].x,option[i].y,option[i].name,option[i].color,font00);
    }
}

void Interface::DrawBar(int_fast16_t x,int_fast16_t y,double numCurrent,double numMax,double scale,int_fast16_t w,int_fast16_t h,uint_fast32_t colorframe,uint_fast32_t colorfill)
{
    //??? MY LAUGHABLE CEREAL TOY QUALITY FORMULA, no keyboard keys were harmed during the making of this 
    //simplified ratio = ((endP - startP) *val%) + startP
    //scalable ratio = (((endP+(baseWidth*objscale)) - (startP-(baseWidth*objscale))) *(cast to double)val%) + startP-(baseWidth*objscale) //pixel shift as needed
    DrawBox(x-(w*scale) ,y-2+(0-h*scale) ,x+(w*scale),y+2+(0-h*scale),colorframe,FALSE);
    DrawBox(x+1-(w*scale) ,y-1+(0-h*scale) ,(((x-1+(w*scale)) - (x-(w*scale))) *((double)numCurrent/numMax)) + x-(w*scale), y+1+(0-h*scale),colorfill,TRUE); 
}

void Interface::Fade(const uint_fast8_t TYPE, const uint_fast8_t SPEED)
{
    switch(TYPE)
    {
        case 0:
            if(Brightness <= 254) Brightness = Brightness + SPEED;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, Brightness);  
            break;
        case 1:
            if(Brightness >= 1) Brightness = Brightness - SPEED; 
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, Brightness); 
            break;
    }
}

void Interface::EndBlend()
{
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Interface::DrawValue(int_fast16_t x, int_fast16_t y, int a, int_fast32_t color)
{
    DrawFormatString(x,y,color,"%d",a);
}

void Interface::DrawValue(int_fast16_t x, int_fast16_t y, char a, int_fast32_t color)
{
    DrawFormatString(x,y,color,"%c",a);
}

void Interface::DrawValue(int_fast16_t x, int_fast16_t y, float a, int_fast32_t color)
{
    DrawFormatString(x,y,color,"%f",a);
}

void Interface::DrawValue(int_fast16_t x, int_fast16_t y, double a, int_fast32_t color)
{
    DrawFormatString(x,y,color,"%lf",a);
}

void Interface::DrawValue(int_fast16_t x, int_fast16_t y, std::string a, int_fast32_t color)
{
    DrawFormatString(x,y,color,"%s",a.c_str()); //actual strings had an issue, cstyle strings work fine. also converts vector strings
}
