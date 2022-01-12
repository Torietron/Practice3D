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
    PhysicsBody_t Body, Probe;            
} Spell_t;

class PlayerData {
    public:
        int_fast8_t Selected;
        uint_least8_t Jump, isCasting, Blinked, Morphed;
        uint_fast8_t CastingTime;
        float SpeedBonus, BlinkDecay;
        VECTOR Last, BlinkRot, BlinkOffset;
        MMD_t MMD;
        Sprite3D_t Marker, MainCircle, MiniCircle, Sig, EnergyWisp;
        PhysicsLastTime_t GCD, Cancelled;
        PlayerData();
        void Load();
        void Update(const Sphere_t *sObj, int_fast16_t Destroyed, const int_fast16_t MAX);
        int SetState(const uint_fast8_t &state);
        int CreateSpell(const uint_fast8_t &spelltype);
        void UpdateSpells();
        void Draw(const Sphere_t *sObj);
    private:
        VECTOR const *Target;
        VECTOR *Pos, *Rot;
};

#endif