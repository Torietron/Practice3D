#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "PhysicsData.h"

static const double PI = M_PI;
static const double nPI = M_PI * -1;
static double cROT = 0.00;
static int_fast16_t ix, iy, xD, yD;
static int_fast32_t random1, random2;
static uint_fast32_t vTime;

extern int_fast16_t Width, Height;
extern Delta_t Delta;

PhysicsData::PhysicsData(float a)
:Decay(a)
{
    //to check or use with Get()
    velocity_x = 0.00, velocity_y = 0.00;
    inertia_x = 0.00, inertia_y = 0.00;
    gravity_x = 0.00, gravity_y = 0.00;
}

//center the axis coord first, Rota is center by default
bool PhysicsData::BoxCollision(int_fast16_t aX, int_fast16_t aY, int_fast16_t aWidth, int_fast16_t aHeight, int_fast16_t bX, int_fast16_t bY, int_fast16_t bWidth,int_fast16_t bHeight)
{
    if(((aX - aWidth > bX - bWidth && aX - aWidth < bX + bWidth)  || (bX - bWidth > aX - aWidth && bX - bWidth < aX + aWidth))
    && ((aY - aHeight > bY - bHeight && aY - aHeight < bY + bHeight) || (bY - bHeight > aY - aHeight && bY - bHeight < aY + aHeight))) return true;
    else return false;
}

//old: uncentered coord
/* 
bool PhysicsData::BoxCollisionTL(int_fast16_t aX, int_fast16_t aY, int_fast16_t aWidth, int_fast16_t aHeight, int_fast16_t bX, int_fast16_t bY, int_fast16_t bWidth,int_fast16_t bHeight)
{
    if(((aX > bX && aX < bX + bWidth)  || (bX > aX && bX < aX + aWidth))
    && ((aY > bY && aY < bY + bHeight) || (bY > aY && bY < aY + bHeight))) return true;
    else return false;                                           ^
}
*/

bool PhysicsData::RadialCollision(int_fast16_t a1, int_fast16_t a2, int_fast16_t a3r, int_fast16_t b1, int_fast16_t b2, int_fast16_t b3r)
{
    //apply theorem
    return false;
}

bool PhysicsData::SphereCollision3D()
{
    //code
}

bool PhysicsData::Fling(int_fast16_t &position, int_fast16_t destination, uint_fast8_t direction, uint_fast16_t speed, float grav, float iMulti)
{
    gravity_x = grav;
    gravity_y = grav;
    inertia_x = (speed*gravity_x)*iMulti;
    inertia_y = (speed*gravity_y)*iMulti;

    switch(direction)
    {
        case FLING_DOWN:
            if(position <= destination-1)
            {
                if(destination - position > 200) {velocity_y = ((speed*8)*gravity_y)*iMulti; position += velocity_y;}
                if(destination - position > 40) {velocity_y = (speed*4)*gravity_y; position += velocity_y;}
                if(destination - position > 15) {velocity_y = (speed*2)*gravity_y; position += velocity_y;}
                if(destination - position > 1) {velocity_y = ((speed*1)*gravity_y)*iMulti; position += velocity_y;}
            }
            if(position == destination-1) position++;
            break;
        case FLING_UP:
            if(position >= destination+1)
            {
                if(position - destination > 200) {velocity_y = ((speed*8)*gravity_y)*iMulti; position -= velocity_y;}
                if(position - destination > 40) {velocity_y = (speed*4)*gravity_y; position -= velocity_y;}
                if(position - destination > 15) {velocity_y = (speed*2)*gravity_y; position -= velocity_y;}
                if(position - destination > 1) {velocity_y = ((speed*1)*gravity_y)*iMulti; position -= velocity_y;}
            }
            if(position == destination+1) position--;
            break;
        case FLING_LEFT:
            if(position >= destination+1)
            {
                if(position - destination > 200) {velocity_x = ((speed*8)*gravity_x)*iMulti; position -= velocity_x;}
                if(position - destination > 40) {velocity_x = (speed*4)*gravity_x; position -= velocity_x;}
                if(position - destination > 15) {velocity_x = (speed*2)*gravity_x; position -= velocity_x;}
                if(position - destination > 1) {velocity_x = ((speed*1)*gravity_x)*iMulti; position -= velocity_x;}
            }
            if(position == destination+1) position--;
            break;
        case FLING_RIGHT:
            if(position <= destination-1)
            {
                if(destination - position > 200) {velocity_x = ((speed*8)*gravity_x)*iMulti; position += velocity_x;}
                if(destination - position > 40) {velocity_x = (speed*4)*gravity_x; position += velocity_x;}
                if(destination - position > 15) {velocity_x = (speed*2)*gravity_x; position += velocity_x;}
                if(destination - position > 1) {velocity_x = ((speed*1)*gravity_x)*iMulti; position += velocity_x;}
            }
            if(position == destination-1) position++;
            break;
    }
    if(position != destination) return true;
    else return false;
}

void PhysicsData::Propel(float &x, float &y, double angle, uint_fast16_t magnitude)
{
    velocity_x = sin(angle)*magnitude;
    velocity_y = cos(angle)*magnitude;
    x += velocity_x;
    y -= velocity_y;
}

void PhysicsData::Spin(double &rot, char L_or_R_Direction, uint_fast8_t totalRotPoints)
{
    if(L_or_R_Direction == 'R') rot += PI/totalRotPoints;
    if(L_or_R_Direction == 'L') rot -= PI/totalRotPoints;
}

void PhysicsData::Standardize(float &a, float min, float max)
{
    if(a < min) a = min;
    if(a > max) a = max;
}

void PhysicsData::Humanize(float &a, float variation)
{
    float hTemp = 0.00;
    srand(static_cast<int_fast32_t>(time(0)));
    random1 = rand();
    random2 = rand();
    SRand(random1 * random2);
    random1 = GetRand(variation/2);
    random2 = rand() % (int)variation / 2;

    hTemp = random1 + random2;
    a = hTemp;
}

float& PhysicsData::ApproxAngle(float &objAngle, float &objMainAxisRotCoord, float objInverseAxisAnchor, int_fast16_t focalPointCoord, int_fast16_t focalPointInverseCoord, float turnRate, int_fast16_t totalRotPointMulti, uint_fast8_t divisor)
{
    if(focalPointInverseCoord <= objInverseAxisAnchor)
    {
        cROT = nPI;
        objAngle = PI*2;
    }
    else cROT = PI;

    while(objMainAxisRotCoord != focalPointCoord)
    {
        if(objMainAxisRotCoord > focalPointCoord)
        {
            objAngle += (cROT*2/(Width*totalRotPointMulti))/divisor;
            objMainAxisRotCoord -= turnRate;
        }
        if(objMainAxisRotCoord < focalPointCoord)
        {
            objAngle -= (cROT*2/(Width*totalRotPointMulti))/divisor;
            objMainAxisRotCoord += turnRate;
        }
    }
    return objAngle;
}

double& PhysicsData::ApproxAngle(double &objAngle, float &objMainAxisRotCoord, float objInverseAxisAnchor, int_fast16_t focalPointCoord, int_fast16_t focalPointInverseCoord, float turnRate, int_fast16_t totalRotPointMulti, uint_fast8_t divisor)
{
    if(focalPointInverseCoord <= objInverseAxisAnchor)
    {
        cROT = nPI;
        objAngle = PI*2;
    }
    else cROT = PI;

    while(objMainAxisRotCoord != focalPointCoord)
    {
        if(objMainAxisRotCoord > focalPointCoord)
        {
            objAngle += (cROT*2/(Width*totalRotPointMulti))/divisor;
            objMainAxisRotCoord -= turnRate;
        }
        if(objMainAxisRotCoord < focalPointCoord)
        {
            objAngle -= (cROT*2/(Width*totalRotPointMulti))/divisor;
            objMainAxisRotCoord += turnRate;
        }
    }
    return objAngle;
}

void PhysicsData::AnchoredAngle(float anchorX, float anchorY, double anchorAngle, float &targetX, float &targetY, double &targetAngle, uint_fast16_t distance)
{
    targetX = anchorX, targetY = anchorY, targetAngle = anchorAngle;
    Propel(targetX,targetY,targetAngle,distance);
}

void PhysicsData::Accelerate(float &vel, uint_fast8_t velBase, uint_fast8_t velMax, float accel, float grav)
{
    inertia_x = accel;
    inertia_y = accel;
    gravity_x = grav;
    gravity_y = grav;

    vel = velBase;
    vel = vel + (accel * grav);
    if(vel > velMax) vel = velMax;

    velocity_x = vel;
    velocity_y = vel;

}

void PhysicsData::Manipulate(int_fast16_t &x, int_fast16_t &y, float &vel_x, float &vel_y, uint_fast32_t &LastTime, uint_fast32_t decayInterval, float grav_x, float grav_y)
{
    velocity_x = vel_x;
    velocity_y = vel_y;
    gravity_x = grav_x;
    gravity_y = grav_y;
    inertia_x = (velocity_x*Decay)*gravity_x;
    inertia_y = (velocity_y*Decay)*gravity_y;

    if(Delta.Time(LastTime, decayInterval))
    {
        if(vel_x > 0) vel_x = floor(inertia_x);
        if(vel_x < 0) vel_x = ceil(inertia_x);
        if(vel_y > 0) vel_y = floor(inertia_y);
        if(vel_y < 0) vel_y = ceil(inertia_y);
        LastTime = GetNowCount();
    }

    x += vel_x;
    y += vel_y;
    
}

float PhysicsData::Get(uint_fast8_t enumFLAG)
{
    //welcome to the leaning tower of switchizza
    switch(enumFLAG)
    {
        case DECAY:
            return Decay;
        case LAST_VELOCITY_X:
            return velocity_x;
        case LAST_VELOCITY_Y:
            return velocity_y;
        case LAST_INERTIA_X:
            return inertia_x;
        case LAST_INERTIA_Y:
            return inertia_y;
        case LAST_GRAVITY_X: 
            return gravity_x;
        case LAST_GRAVITY_Y:
            return gravity_y;
    }
}

//center axis coord first, Rota is center by default
void PhysicsData::DrawHitBox(int_fast16_t x, int_fast16_t y, int_fast16_t w, int_fast16_t h, int_fast32_t Color, uint_fast8_t FillFlag)
{
    DrawBox(x - w, y - h, x + w, y + h, Color, FillFlag);
}