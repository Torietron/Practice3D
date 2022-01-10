/***** PlayerData.h *****/
#ifndef DEF_PLAYERDATA_H
#define DEF_PLAYERDATA_H

#include "DxLib.h"
#include <cstdint>
#include "ModelData.h"
#include "EnemyData.h"
#include "PhysicsData.h"

typedef struct {
    uint_fast8_t PolyLevel;
    int_fast32_t Color;
    float Radius, AngleH, AngleV;
    PhysicsBody_t Body;            
} Spell_t;

class PlayerData {
    public:
        int_fast8_t Selected;
        uint_least8_t Jump, isCasting;
        uint_fast8_t CastingTime;
        VECTOR Last;
        MMD_t MMD;
        Sprite3D_t Marker, MainCircle, MiniCircle, Sig;
        PhysicsLastTime_t GCD;
        PlayerData();
        void Load();
        void Update(const Sphere_t *sObj, int_fast16_t Destroyed, const int_fast16_t MAX);
        void CreateSpell(const Sphere_t &sObj);
        void UpdateSpells();
        void Draw(const Sphere_t *sObj);
    private:
        VECTOR *Pos, *Rot;
};

#endif