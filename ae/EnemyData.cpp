#include "DxLib.h"
#include <cstdint>
#include "EnemyData.h"

void EnemyData::Draw(Sphere_t *sObj, const uint_fast16_t MAX, uint_fast8_t *DFlag)
{
    for(uint_fast8_t i = 0; i < 2; i++)
    {
        if(DFlag[i] == 1)
        {
            DrawSphere3D(sObj[i].v, sObj[i].r, 200, GetColor(200,0,120), GetColor(100,0,60), TRUE);
        }
    }
}