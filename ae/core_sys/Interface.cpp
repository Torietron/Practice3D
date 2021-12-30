#include "DxLib.h"
#include <iostream>
#include <cstdint>
#include "Interface.h"
#include "KeyPoll.h"

extern KeyPoll Key;
extern int_fast16_t Width, Height;

Interface::Interface(int_fast8_t a)
:selected(a)
{
    brightness = 0;
    White = GetColor(255,255,255);
    Red = GetColor(255,0,0);
    Yellow = GetColor(255,255,0);
    FontSize = 16;
    FontThickness = 7;
    FontType = DX_FONTTYPE_ANTIALIASING_EDGE_4X4;
    Options_t options;
    options.x = Width/2-75;
    options.y = Height/2+25;
}

void Interface::UpdateMenu(const uint_fast8_t MAX, const int KEY1, const int KEY2)
{
    if(Key.Poll[KEY1] == 1 || Key.Poll[KEY2] == 1) 
    {
        if(Key.Poll[KEY2] == 1) selected = (selected + 1) % MAX;
        if(Key.Poll[KEY1] == 1) selected = (selected - 1) % MAX;
        if(Key.Poll[KEY1] == 1 && selected < 0) selected = MAX - 1;
    }
}

void Interface::DrawMenu(Options_t *option, const uint_fast8_t MAX, int_fast32_t selectedColor, int_fast32_t baseColor)
{
    for(uint_fast8_t i = 0; i < MAX; i++)
    {
        if(i == selected) 
        {
            option[i].color = selectedColor;
        }
        else
        {
            option[i].color = baseColor;
        }
        DrawString(option[i].x,option[i].y,option[i].name,option[i].color);
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
            if(brightness <= 254) brightness = brightness + SPEED;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, brightness);  
            break;
        case 1:
            if(brightness >= 1) brightness = brightness - SPEED; 
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, brightness); 
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
