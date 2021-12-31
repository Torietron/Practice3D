#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "PhysicsData.h"
#include "ScreenControl.h"
#include "ModelData.h"

static const double PI = M_PI;
static const double nPI = M_PI * -1;
static double cROT = 0.00;
static int_fast32_t random1, random2;

extern ScreenControl Screen;

PhysicsData::PhysicsData(float a)
:Decay(a)
{
    //to check or use with Get()
    velocity_x = 0.00f, velocity_y = 0.00f;
    inertia_x = 0.00f, inertia_y = 0.00f;
    gravity_x = 0.00f, gravity_y = 0.00f;
}

void PhysicsData::PhysicsFormula::Standardize(float &a, float min, float max)
{
    if(a < min) a = min;
    if(a > max) a = max;
}

void PhysicsData::PhysicsFormula::Humanize(float &a, float variation)
{
    float hTemp = 0.00f;
    srand(static_cast<int_fast32_t>(time(0)));
    random1 = rand();
    random2 = rand();
    SRand(random1 * random2);
    random1 = GetRand((int)variation/2);
    random2 = rand() % (int)variation / 2;

    hTemp = random1 + random2;
    a = hTemp;
}

float& PhysicsData::PhysicsFormula::ApproxAngle(float &objAngle, float &objMainAxisRotCoord, float objInverseAxisAnchor, int_fast16_t focalPointCoord, int_fast16_t focalPointInverseCoord, float turnRate, int_fast16_t totalRotPointMulti, uint_fast8_t divisor)
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
            objAngle += (cROT*2/(Screen.Width*totalRotPointMulti))/divisor;
            objMainAxisRotCoord -= turnRate;
        }
        if(objMainAxisRotCoord < focalPointCoord)
        {
            objAngle -= (cROT*2/(Screen.Width*totalRotPointMulti))/divisor;
            objMainAxisRotCoord += turnRate;
        }
    }
    return objAngle;
}

double& PhysicsData::PhysicsFormula::ApproxAngle(double &objAngle, float &objMainAxisRotCoord, float objInverseAxisAnchor, int_fast16_t focalPointCoord, int_fast16_t focalPointInverseCoord, float turnRate, int_fast16_t totalRotPointMulti, uint_fast8_t divisor)
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
            objAngle += (cROT*2/(Screen.Width*totalRotPointMulti))/divisor;
            objMainAxisRotCoord -= turnRate;
        }
        if(objMainAxisRotCoord < focalPointCoord)
        {
            objAngle -= (cROT*2/(Screen.Width*totalRotPointMulti))/divisor;
            objMainAxisRotCoord += turnRate;
        }
    }
    return objAngle;
}

void PhysicsData::PhysicsFormula::AnchoredAngle(float anchorX, float anchorY, double anchorAngle, float &targetX, float &targetY, double &targetAngle, uint_fast16_t distance)
{
    targetX = anchorX, targetY = anchorY, targetAngle = anchorAngle;
    parent->Propel(targetX,targetY,targetAngle,distance);
}

DxLib::VECTOR PhysicsData::PhysicsFormula::SetCross(const VECTOR &a, const VECTOR &b)
{
	VECTOR c;
	c.x = (a.y * b.z - a.z * b.y);
	c.y = (a.z * b.x - a.x * b.z);
	c.z = (a.x * b.y - a.y * b.x);
	return c;
}

float PhysicsData::PhysicsFormula::Dot3(const VECTOR &a, const VECTOR &b)
{
	float dot;
	dot = (a.x * b.x + a.y * b.y + a.z * b.z);
	return dot;
}

float PhysicsData::PhysicsFormula::Dot2(const float &x, const float &z)
{
	float dot;
	dot = (x*x+z*z);
	return dot;
}

//center the axis coord first, Rota is center by default
bool PhysicsData::BoxCollision(const int_fast16_t &aX, const int_fast16_t &aY, const int_fast16_t &aWidth, const int_fast16_t &aHeight, const int_fast16_t &bX, const int_fast16_t &bY, const int_fast16_t &bWidth, const int_fast16_t &bHeight)
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

bool PhysicsData::RadialCollision(const int_fast16_t &a1, const int_fast16_t &a2, const int_fast16_t &a3r, const int_fast16_t &b1, const int_fast16_t &b2, const int_fast16_t &b3r)
{
    //apply theorem
    return false;
}

//Refresh model collision info beforehand, once per frame
bool PhysicsData::SphereCollision3D(const int &modelH, const VECTOR &spherePos, const float &radius, MV1_COLL_RESULT_POLY_DIM &hpd)
{
    hpd = MV1CollCheck_Sphere(modelH, -1, spherePos, radius);
    if(hpd.HitNum >= 1)
    {
        MV1CollResultPolyDimTerminate(hpd);
        return true;
    }
    else return false;
}

bool PhysicsData::Fling(int_fast16_t &position, int_fast16_t destination, const uint_fast8_t ENUM_FLING_DIRECTION, uint_fast16_t speed, float grav, float iMulti)
{
    gravity_x = grav;
    gravity_y = grav;
    inertia_x = (speed*gravity_x)*iMulti;
    inertia_y = (speed*gravity_y)*iMulti;

    switch(ENUM_FLING_DIRECTION)
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

float PhysicsData::Get(const uint_fast8_t ENUM_GET)
{
    //welcome to the leaning tower of switchizza
    switch(ENUM_GET)
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
void PhysicsData::DrawHitBox(const int_fast16_t &x, const int_fast16_t &y, const int_fast16_t &w, const int_fast16_t &h, int_fast32_t Color, uint_fast8_t FillFlag)
{
    DrawBox(x - w, y - h, x + w, y + h, Color, FillFlag);
}