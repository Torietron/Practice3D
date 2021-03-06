/***** ModelData.h *****/
#ifndef DEF_MODELDATA_H
#define DEF_MODELDATA_H

#include "DxLib.h"
#include "PhysicsData.h"
#include <cstdint>

typedef struct { /* MMD character model */
    int ModelH, AttachIndex, LastIndex, BlendIndex1, BlendIndex2;
    uint_least8_t AnimSet, AnimIndex, State;
    int_fast32_t Pace;
    float PlayTime, TotalTime, PlayOffset, BlendDecay, LastPlayTime;
    VECTOR PosOffset, RotOffset;
    PhysicsBody_t Body;
    bool Event, Reverse, AutoBlend;
} MMD_t;

typedef struct { /* DX accessory/prop/character/environment */
    int ModelH, AttachIndex;
    uint_least8_t AnimSet, AnimIndex, State;
    int_fast32_t Pace;
    float PlayTime, TotalTime, PlayOffset;
    VECTOR Pos, Rot, RotOffset;
    bool Event, Reverse;
} DX_t;

typedef struct { /* MQO environment */
    int ModelH;
    uint_least8_t State;
    VECTOR Pos, Rot;
    bool Event;
} MQO_t;

typedef struct { /* 2D Warp Model */
    int ModelH, Anim;
    uint_least8_t State;
    VECTOR Pos, Rot;
    bool Event;
} Live2D_t;

typedef struct { /* 2D Sprite in a 3D position */
    int SpriteH[180], *SpritePtr, **SpritePtr_D;
    uint_least8_t FluxReverse, State, SpriteMax, SpriteIndex;
    uint_fast32_t AnimTime;
    float Flux, FluxRate, FluxMin, FluxMax; 
    float Size, SizeOffset, Angle;
    float cx, cy, a, b, c, d;
    VECTOR PosOffset, RotOffset;
    PhysicsBody_t Body;
    bool Event, Reverse, EnableModi;
} Sprite3D_t;

typedef struct { /* 2D Sprite */
    int SpriteH, *SpritePtr, **SpritePtr_D;
    uint_least8_t State, SpriteMax;
    int_fast16_t x, y;
    uint_fast16_t w, h;
    float Angle, AngleOffset;
    VECTOR Pos, PosOffset;
    bool Event;
} Sprite2D_t;

class ModelData { /* Manage models */
    public:
        ModelData(float rate = 0.39f);
        void Update(MMD_t &m);
        void Update(MQO_t &m);
        int Update(Sprite3D_t &m, const float &frameRate, const uint_fast8_t &skipRate = 0);
        void SetManualBlend(MMD_t &m, const uint_fast8_t &index1, const float &rate1, const uint_fast8_t &index2, const float &rate2);
        void RunManualBlend(MMD_t &m, const float &incrementRate1, const float &incrementRate2, const float &maxRate1 = 9999.9f, const float &maxRate2 = 9999.9f, const float &minRate1 = 0.0f, const float &minRate2 = 0.0f);
        void EndManualBlend(MMD_t &m);
        void Draw(MMD_t &m, const float &autoblend_max = 1.0f, const float &autoblend_min = 0.0f);
        void Draw(const MQO_t &m);
        void Draw(const Sprite3D_t &m, const float &y1 = 10.0f, const float &y2 = 10.0f, const float &y3 = -10.0f, const float &y4 = -10.0f);
        void SetPlayRate(const float &a);
        float GetPlayRate();
    private:
        float play_rate;
};

#endif