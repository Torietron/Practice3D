/***** ModelData.h *****/
#ifndef DEF_MODELDATA_H
#define DEF_MODELDATA_H

#include "DxLib.h"
#include <cstdint>

typedef struct {
    int ModelH, AttachIndex;
    uint_fast8_t AnimSet, AnimIndex;
    int_fast32_t Pace;
    float PlayTime, TotalTime, PlayOffset;
    VECTOR Pos, Rot, RotOffset;
    bool Event, Reverse;
} MMD_t;

typedef struct {
    int ModelH, AttachIndex;
    uint_fast8_t AnimSet, AnimIndex;
    int_fast32_t Pace;
    float PlayTime, TotalTime, PlayOffset;
    VECTOR Pos, Rot, RotOffset;
    bool Event, Reverse;
} X_t;

typedef struct {
    int ModelH;
    VECTOR Pos, Rot;
    bool Event;
} MQO_t;

typedef struct {
    int ModelH, Anim;
    VECTOR Pos, Rot;
    bool Event;
} LIVE2D_t;

typedef struct {
    int SpriteH;
    uint_fast8_t FluxReverse;
    float Flux, FluxRate;
    VECTOR Pos, Rot;
    int_fast16_t x, y;
    uint_fast16_t w, h;
    bool Event;
} SPRITE3D_t;

typedef struct {
    int SpriteH;
    VECTOR Pos, Rot;
    int_fast16_t x, y;
    uint_fast16_t w, h;
    bool Event;
} SPRITE2D_t;

class ModelData {
    public:
        ModelData(float rate = 0.39f);
        void Update(MMD_t &m);
        void Update(X_t &m);
        void Update(MQO_t &m);
        void SetPlayRate(float a);
        float GetPlayRate();
    private:
        float play_rate;
};

#endif