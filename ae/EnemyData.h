/***** EnemyData.h *****/
#ifndef DEF_ENEMYDATA_H
#define DEF_ENEMYDATA_H

#include "DxLib.h"
#include <cstdint>

typedef struct {
	VECTOR v;
	float r;
	bool active;
} Sphere_t;

class EnemyData {
    public:
        void Draw(Sphere_t *sObj, const uint_fast16_t MAX, uint_fast8_t *DFlag);
};

#endif