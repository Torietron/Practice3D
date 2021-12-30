/***** PhysicsData.h *****/
#ifndef DEF_PHYSICSDATA_H
#define DEF_PHYSICSDATA_H

#include <cstdint>
#include "PlayerData.h"
#include "EnemyData.h"
#include "Projectiles.h"
class PhysicsData
{
    public:
        PhysicsData(float a = 0.99);
        bool BoxCollision(int_fast16_t aX, int_fast16_t aY, int_fast16_t aWidth, int_fast16_t aHeight, int_fast16_t bX, int_fast16_t bY, int_fast16_t bWidth,int_fast16_t bHeight);
        bool RadialCollision(int_fast16_t a1, int_fast16_t a2, int_fast16_t a3r, int_fast16_t b1, int_fast16_t b2, int_fast16_t b3r);
        bool SphereCollision3D();
        bool Fling(int_fast16_t &position, int_fast16_t destination, uint_fast8_t direction, uint_fast16_t speed = 1, float grav = 1.00, float iMulti = 1.00);
        void Propel(float &x, float &y, double angle, uint_fast16_t magnitude = 1);
        void Spin(double &rot, char L_or_R_Direction, uint_fast8_t totalRotPoints = 32);
        void Standardize(float &a, float min, float max);
        void Humanize(float &a, float variation);
        float& ApproxAngle(float &objAngle, float &objMainAxisCoord, float objInverseAxisAnchor, int_fast16_t focalPointCoord, int_fast16_t focalPointInverseCoord, float turnRate = 1.00, int_fast16_t totalRotPointMulti = 1, uint_fast8_t divisor = 4);   
        double& ApproxAngle(double &objAngle, float &objMainAxisRotCoord, float objInverseAxisAnchor, int_fast16_t focalPointCoord, int_fast16_t focalPointInverseCoord, float turnRate = 1.00, int_fast16_t totalRotPointMulti = 1, uint_fast8_t divisor = 4);
        void AnchoredAngle(float anchorX, float anchorY, double anchorAngle, float &targetX, float &targetY, double &targetAngle, uint_fast16_t distance);
        void Accelerate(float &vel, uint_fast8_t velBase, uint_fast8_t velMax, float accel, float grav = 1.00);
        void Manipulate(int_fast16_t &x, int_fast16_t &y, float &vel_x, float &vel_y, uint_fast32_t &LastTime, uint_fast32_t decayInterval = 240, float grav_x = 1.00, float grav_y = 1.00);
        float Get(uint_fast8_t enumFLAG);
        void DrawHitBox(int_fast16_t x, int_fast16_t y, int_fast16_t w, int_fast16_t h, int_fast32_t Color = GetColor(255,0,0), uint_fast8_t FillFlag = 0);
        //void DrawHitCircle();
    private:
        const float Decay;
        float velocity_x;
        float velocity_y;
        float inertia_x;
        float inertia_y;
        float gravity_x;
        float gravity_y;
};

typedef struct Delta_t {
    uint_fast8_t Event, Frames;
    uint_fast16_t Ticks;
    uint_fast32_t dTime, xTime;
    char label;
    bool Time(uint_fast32_t &LastTime, uint_fast32_t ElapsedGoal)
    {
        if(dTime - LastTime > ElapsedGoal) 
        {
            LastTime = GetNowCount();
            return true;
        }
        else return false;
    }
    void Update()
    {
        dTime = GetNowCount();
    }
}; 

typedef struct {
    uint_fast32_t Time, dTime;
    bool Flag;
} LastDelta_t;

typedef enum {
    FLING_DOWN,
    FLING_UP,
    FLING_RIGHT,
    FLING_LEFT
} Direct_t;

typedef enum{
    DECAY,
    LAST_VELOCITY_X,
    LAST_VELOCITY_Y,
    LAST_INERTIA_X,
    LAST_INERTIA_Y,
    LAST_GRAVITY_X,
    LAST_GRAVITY_Y
} pData_t;

#endif