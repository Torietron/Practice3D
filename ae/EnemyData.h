/***** EnemyData.h *****/
#ifndef DEF_ENEMYDATA_H
#define DEF_ENEMYDATA_H

#include "DxLib.h"

typedef struct {
	VECTOR v;
	float r;
	bool active;
} Sphere_t;

class EnemyData {
    public:
        int temp;
};

#endif