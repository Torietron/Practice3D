/***** EnemyData.h *****/
#ifndef DEF_ENEMYDATA_H
#define DEF_ENEMYDATA_H

#include "DxLib.h"
#include <cstdint>
#include "PhysicsData.h"

typedef struct {
	bool Active;
	PhysicsBody_t Body;
} Sphere_t;

class EnemyData {
    public:
        void Draw(Sphere_t *sObj, const uint_fast16_t MAX, uint_fast16_t *DFlag);
};

#endif