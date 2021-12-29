/***** ModelData.h *****/
#ifndef DEF_MODELDATA_H
#define DEF_MODELDATA_H

#include <cstdint>

class ModelData {
    public:
        void Update();
        float AngleH();
        float AngleV():
    private:
        void Advance();
};

typedef struct {
    int ModelH, AttachIndex;
    uint_fast8_t AnimSet, AnimIndex;
    float PlayTime;
} MMD_t;

#endif