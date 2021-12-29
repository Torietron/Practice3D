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
    VECTOR Pos, Rot;
    bool Flag, Reverse;
} MMD_t;

class ModelData {
    public:
        ModelData(float rate = 0.39f);
        void Update(MMD_t &m);
        void SetPlayRate(float a);
        float GetPlayRate();
    private:
        float play_rate;
        void Animate(MMD_t &m);
};

#endif