/***** PlayerData.h *****/
#ifndef DEF_PLAYERDATA_H
#define DEF_PLAYERDATA_H

#include "DxLib.h"
#include <cstdint>
#include "ModelData.h"
#include "EnemyData.h"

class PlayerData {
    public:
        uint_fast8_t Jump, Grounded;
        VECTOR Last;
        MMD_t MMD;
        PlayerData();
        void Load();
        void Update(Sphere_t *sObj, uint_fast8_t Destroyed, const int MAX);
    private:
        float anchor_y;
};

#endif