/***** PhysicsData.h *****/
#ifndef DEF_PHYSICSDATA_H
#define DEF_PHYSICSDATA_H

#include "DxLib.h"
#include <cstdint>

//Track individual times and events
typedef struct {
    uint_fast32_t Time, dTime;
    bool Event;
} PhysicsLastDelta_t;

typedef struct _PhysicsDelta_t {
    uint_least8_t Frames;
    uint_fast16_t Ticks;
    uint_fast32_t dTime, xTime;
    char Label;
    bool Event;
    bool Time(uint_fast32_t &LastTime, uint_fast32_t ElapsedGoal) {
        if(dTime - LastTime > ElapsedGoal) {
            LastTime = GetNowCount();
            return true;
        }
        else return false;
    }
    void Update() {
        dTime = GetNowCount();
    }
} _PhysicsDelta_t; 

class PhysicsData { //Manipulate time and space
    protected:
        class _PhysicsFormula {
            public: //Calculations here
                bool BoxColl2D(const int_fast16_t &aX, const int_fast16_t &aY, const int_fast16_t &aWidth, const int_fast16_t &aHeight, const int_fast16_t &bX, const int_fast16_t &bY, const int_fast16_t &bWidth, const int_fast16_t &bHeight);
                bool BoxColl2D(const float &aX, const float &aY, const float &aWidth, const float &aHeight, const float &bX, const float &bY, const float &bWidth, const float &bHeight);
                bool RadialColl2D(const float &aX, const float &aY, const float &aR, const float &bX, const float &bY, const float &bR);
                bool SphereColl3D(const int &modelH, const VECTOR &spherePos, const float &radius, MV1_COLL_RESULT_POLY_DIM &hpd);     
                float Standardize(float &a, const float &min, const float &max);
                float Humanize(float &a, const float &variation);
                float& ApproxAngle(const int_fast16_t screenWidth, float &objAngle, float &objMainAxisCoord, float objInverseAxisAnchor, int_fast16_t focalPointCoord, int_fast16_t focalPointInverseCoord, float turnRate = 1.00, int_fast16_t totalRotPointMulti = 1, uint_fast8_t divisor = 4);   
                double& ApproxAngle(const int_fast16_t screenWidth, double &objAngle, float &objMainAxisRotCoord, float objInverseAxisAnchor, int_fast16_t focalPointCoord, int_fast16_t focalPointInverseCoord, float turnRate = 1.00, int_fast16_t totalRotPointMulti = 1, uint_fast8_t divisor = 4);
                void AnchoredAngle(float anchorX, float anchorY, double anchorAngle, float &targetX, float &targetY, double &targetAngle, uint_fast16_t distance);
                float RelAngle3(const DxLib::VECTOR &a, const DxLib::VECTOR &b);
                float RelAngle2(const DxLib::VECTOR &a, const DxLib::VECTOR &b);
                DxLib::VECTOR Cross3(const VECTOR &a, const VECTOR &b);
                float Dot3(const VECTOR &a, const VECTOR &b);
                float Dot2(const float &x, const float &z);
            private:
                PhysicsData *parent;
        };
    public: //Manipulations here
        _PhysicsDelta_t Delta;
        _PhysicsFormula Formula;
        PhysicsData(float a = 0.99f);
        void Spin(float &rot, char L_or_R_Direction, uint_fast8_t totalRotPoints = 32);
        void Spin(double &rot, char L_or_R_Direction, uint_fast8_t totalRotPoints = 32);
        bool Fling(int_fast16_t &position, int_fast16_t destination, const uint_fast8_t ENUM_FLING_DIRECTION, uint_fast16_t speed = 1, float grav = 1.00, float iMulti = 1.00);
        void Propel(float &x, float &y, double angle, uint_fast16_t magnitude = 1);
        void Accelerate(float &vel, uint_fast8_t velBase, uint_fast8_t velMax, float accel, float grav = 1.00);
        void Manipulate(int_fast16_t &x, int_fast16_t &y, float &vel_x, float &vel_y, uint_fast32_t &LastDeltaTime, uint_fast32_t decayInterval = 240, float grav_x = 1.00, float grav_y = 1.00);
        void Manipulate(float &x, float &y, float &vel_x, float &vel_y, uint_fast32_t &LastDeltaTime, uint_fast32_t decayInterval = 240, float grav_x = 1.00, float grav_y = 1.00);
        float Get(const uint_fast8_t ENUM_GET);
        void DrawHitBox(const int_fast16_t &x, const int_fast16_t &y, const int_fast16_t &w, const int_fast16_t &h, const int_fast32_t Color = GetColor(255,0,0), const uint_fast8_t FillFlag = 0);
        void DrawHitCircle(const int_fast16_t &x, const int_fast16_t &y, const int_fast16_t &collRadius);
    private:
        const float Decay;
        float velocity_x;
        float velocity_y;
        float inertia_x;
        float inertia_y;
        float gravity_x;
        float gravity_y;
};

typedef enum {
    FLING_DOWN,
    FLING_UP,
    FLING_RIGHT,
    FLING_LEFT
} _PhysicsFlingDirection_t;

typedef enum {
    DECAY,
    LAST_VELOCITY_X,
    LAST_VELOCITY_Y,
    LAST_INERTIA_X,
    LAST_INERTIA_Y,
    LAST_GRAVITY_X,
    LAST_GRAVITY_Y
} _PhysicsGet_t;

#endif