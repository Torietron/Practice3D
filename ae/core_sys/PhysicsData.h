/***** PhysicsData.h *****/
#ifndef DEF_PHYSICSDATA_H
#define DEF_PHYSICSDATA_H

#include "DxLib.h"
#include <cstdint>

typedef struct { /* Track individual times and events */
    uint_fast32_t Time, Count;
    bool Event;
} PhysicsLastTime_t;

typedef struct { /* Set Enable3D to TRUE for 3D calculations */
    bool Enable3D;
    int_least8_t Grounded;
    uint_fast32_t Interval;
    float Accel, AccelRate, VelBase, VelMax, Angle;
    float TermVel, FrictionRatio, MassRatio;
    VECTOR Pos, Vel, Rot, gForce;
    PhysicsLastTime_t Time;
} PhysicsBody_t;

typedef struct _PhysicsTimeDelta_t {
    uint_least8_t Frames;
    uint_fast16_t Ticks;
    uint_fast32_t dTime, xTime;
    char Label;
    bool Event;
    bool Time(PhysicsLastTime_t &Last, const uint_fast32_t &ElapsedGoal) {
        if(dTime - Last.Time > ElapsedGoal) {
            Last.Time = GetNowCount();
            Last.Count++; 
            return true;
        }
        else return false;
    }
    void Update() {
        dTime = GetNowCount();
    }
} _PhysicsTimeDelta_t; 

class PhysicsData { /* Manipulate time and space */
    protected:
        class _PhysicsFormula {
            public: //Calculations here
                bool BoxColl2D(const int_fast16_t &aX, const int_fast16_t &aY, const int_fast16_t &aWidth, const int_fast16_t &aHeight, const int_fast16_t &bX, const int_fast16_t &bY, const int_fast16_t &bWidth, const int_fast16_t &bHeight);
                bool BoxColl2D(const float &aX, const float &aY, const float &aWidth, const float &aHeight, const float &bX, const float &bY, const float &bWidth, const float &bHeight);
                bool RadialColl2D(const float &aX, const float &aY, const float &aR, const float &bX, const float &bY, const float &bR);
                bool SphereColl3D(const int &modelH, const DxLib::VECTOR &spherePos, const float &radius, MV1_COLL_RESULT_POLY_DIM &hpd);     
                float Standardize(float &a, const float &min, const float &max);
                float Humanize(float &a, const float &variation);
                float& ApproxAngle(float &objAngle, float &objMainAxisCoord, const float &objInverseAxisAnchor, const int_fast16_t &focalPointCoord, const int_fast16_t &focalPointInverseCoord, const int_fast16_t &screenWidth, const float &turnRate = 1.00f, const int_fast16_t &totalRotPointMulti = 1, const uint_fast8_t &divisor = 4);   
                double& ApproxAngle(double &objAngle, float &objMainAxisCoord, const float &objInverseAxisAnchor, const int_fast16_t &focalPointCoord, const int_fast16_t &focalPointInverseCoord, const int_fast16_t &screenWidth, const float &turnRate = 1.00f, const int_fast16_t &totalRotPointMulti = 1, const uint_fast8_t &divisor = 4);
                void AnchoredAngle(float &x, float &y, float &angle, const float &anchorX, const float &anchorY, const float &anchorAngle, const float &distance);
                double RelAngle3Precise(const DxLib::VECTOR_D &a, const DxLib::VECTOR_D &b, const uint_fast8_t &formulaOrder = 0);
                float RelAngle3(const DxLib::VECTOR &a, const DxLib::VECTOR &b, const uint_fast8_t &formulaOrder = 0);
                float RelAngle3Fast(const DxLib::VECTOR &a, const DxLib::VECTOR &b, const uint_fast8_t &formulaOrder = 0);
                double RelAngle2Precise(const DxLib::VECTOR_D &a, const DxLib::VECTOR_D &b, const uint_fast8_t &formulaOrder = 0);
                float RelAngle2(const DxLib::VECTOR &a, const DxLib::VECTOR &b, const uint_fast8_t &formulaOrder = 0);
                float RelAngle2Fast(const DxLib::VECTOR &a, const DxLib::VECTOR &b, const uint_fast8_t &formulaOrder = 0);
                DxLib::VECTOR_D Cross3Precise(const DxLib::VECTOR_D &a, const DxLib::VECTOR_D &b);
                DxLib::VECTOR Cross3(const DxLib::VECTOR &a, const DxLib::VECTOR &b);
                DxLib::VECTOR Cross3Fast(const DxLib::VECTOR &a, const DxLib::VECTOR &b);
                double Dot3Precise(const DxLib::VECTOR_D &a, const DxLib::VECTOR_D &b);
                float Dot3(const DxLib::VECTOR &a, const DxLib::VECTOR &b);
                float Dot3Fast(const DxLib::VECTOR &a, const DxLib::VECTOR &b);
                float Distance3(const DxLib::VECTOR &a, const DxLib::VECTOR &b);
                float Distance2(const float &x1, const float &x2, const float &y1, const float &y2);
                float Distance1(const float &a, const float &b);
                DxLib::VECTOR_D ConvertVector(const DxLib::VECTOR &input);
                DxLib::VECTOR ConvertVector(const DxLib::VECTOR_D &input);
            private:
                PhysicsData *parent;
        };
    public: //Manipulations here
        _PhysicsTimeDelta_t Delta;
        _PhysicsFormula Formula;
        PhysicsData(float pullRate = 0.3f);
        void Spin(float &angle, const char &L_or_R_Direction = 'L', const uint_fast8_t &totalRotPoints = 32);
        void Spin(double &angle, const char &L_or_R_Direction = 'L', const uint_fast8_t &totalRotPoints = 32);
        bool Fling(int_fast16_t &position, int_fast16_t destination, const uint_fast8_t ENUM_FLING_DIRECTION, uint_fast16_t speed = 1, float grav = 1.0f, float iMulti = 1.0f);
        void Propel(float &x, float &y, const float &angle2D, const float &magnitude = 1);
        void PropelFast(float &x, float &y, const float &angle2D, const float &magnitude = 1);
        void Propel(PhysicsBody_t &Body, const float &magnitude = 1, const float &offsetH = 0.0f, const float &offsetV = 0.0f);
        void PropelFast(PhysicsBody_t &Body, const float &magnitude = 1, const float &offsetH = 0.0f, const float &offsetV = 0.0f);
        float Accelerate(float &vel, const float &velBase, const float &velMax, const float &accel, const float &grav = 1.0f);
        float Accelerate(PhysicsBody_t &Body, const uint_fast8_t &ENUM_AXIS);
        void Manipulate(int_fast16_t &x, int_fast16_t &y, float &velX, float &velY, PhysicsLastTime_t &Last, const uint_fast32_t &interval = 240, const float &gravX = 1.0f, const float &gravY = 1.0f, const float &decay = 0.99f);
        void Manipulate(float &x, float &y, float &velX, float &velY, PhysicsLastTime_t &Last, const uint_fast32_t &interval = 240, const float &gravX = 1.0f, const float &gravY = 1.0f, const float &decay = 0.99f);
        void Manipulate(PhysicsBody_t &Body, const uint_fast8_t &snapX = 1.0f, const uint_fast8_t &snapY = 1.0f, const uint_fast8_t &snapZ = 1.0f);
        float GetLastValue(const uint_fast8_t &ENUM_LAST);
        void SetWorldGravityMulti(const float &x = 0.0f, const float &y = 1.0f, const float &z = 0.0f);
        void SetWorldGravityPos(const float &x = -0.0f, const float &y = 0.0f, const float &z = 0.0f);
        void SetWorldGravityRange(const float &x = 1.0f, const float &y = 1700.0f, const float &z = 1.0f);
        float GetWorldGravityMulti(const uint_fast8_t &ENUM_AXIS);
        float GetWorldGravityPos(const uint_fast8_t &ENUM_AXIS);
        float GetWorldGravityRange(const uint_fast8_t &ENUM_AXIS);
        void DrawHitBox(const int_fast16_t &x, const int_fast16_t &y, const int_fast16_t &w, const int_fast16_t &h, const int_fast32_t &color = -65536, const uint_fast8_t &fillFlag = 0);
        void DrawHitCircle(const int_fast16_t &x, const int_fast16_t &y, const int_fast16_t &collRadius, const int_fast32_t &color = -65536, const uint_fast8_t &fillFlag = 0);
    private:
        const float GRAV_PULL_RATE;
        VECTOR velocity, gravity;
        VECTOR world_gravity_multi, world_gravity_pos, world_gravity_range;
        int ForceDecay(PhysicsBody_t &Body, const uint_fast8_t &ENUM_AXIS);
};

typedef enum { //FLING_DOWN, FLING_UP, FLING_RIGHT, FLING_LEFT
    FLING_DOWN,
    FLING_UP,
    FLING_RIGHT,
    FLING_LEFT
} _PhysicsFlingDirection_t;

typedef enum { //AXIS_X, AXIS_Y, AXIS_Z
    AXIS_X,
    AXIS_Y,
    AXIS_Z
} _PhysicsAxis_t;

typedef enum { //LAST_VELOCITY_X, LAST_VELOCITY_Y, LAST_VELOCITY_Z, LAST_GRAVITY_X, LAST_GRAVITY_Y, LAST_GRAVITY_Z
    LAST_VELOCITY_X,
    LAST_VELOCITY_Y,
    LAST_VELOCITY_Z,
    LAST_GRAVITY_X,
    LAST_GRAVITY_Y,
    LAST_GRAVITY_Z
} _PhysicsGet_t;

#endif