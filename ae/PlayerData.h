/***** PlayerData.h *****/
#ifndef DEF_PLAYERDATA_H
#define DEF_PLAYERDATA_H

#include "DxLib.h"
#include <cstdint>
#include "ModelData.h"
#include "EnemyData.h"

class PlayerData {
    public:
        uint_least8_t Jump, Grounded;
        VECTOR Last;
        MMD_t MMD;
        Sprite3D_t Marker;
        PlayerData();
        void Load();
        void Update(Sphere_t *sObj, int_fast16_t Destroyed, const int_fast16_t MAX);
        void Draw(Sphere_t *sObj);
    private:
        float anchor_y;
};

#endif