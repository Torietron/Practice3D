#include "DxLib.h"
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "PhysicsData.h"

static bool DisplayErrorGetEnum = TRUE;
static bool DisplayErrorAxisEnum = TRUE;
static bool DisplayErrorGravRange = TRUE;
static bool DisplayErrorRelAngle3 = TRUE, DisplayErrorRelAngle3Precise = TRUE, DisplayErrorRelAngle3Fast = TRUE;
static bool DisplayErrorRelAngle2 = TRUE, DisplayErrorRelAngle2Precise = TRUE, DisplayErrorRelAngle2Fast = TRUE;

static float *gForce, *grav, *vel;
static uint_fast8_t random;
static int_fast16_t temp, dice1, dice2;
static float tempf, dice1f, dice2f, hTemp1, hTemp2;
static double tempd;
static VECTOR S;
static VECTOR_D S_d, vec_d1, vec_d2;

PhysicsData::PhysicsData(float pullRate)
:GRAV_PULL_RATE(pullRate)
{
    //to check or use with GetLastValue()
    velocity.x = 0.00f, velocity.y = 0.00f;
    gravity.x = 0.00f, gravity.y = 0.00f;

    //world grav init
    SetWorldGravityMulti();
    SetWorldGravityPos();
    SetWorldGravityRange();

    //TimeDelta init
    Delta.AutoUpdate = TRUE;
}

//center axis coords first, Rota is center by default
bool PhysicsData::_PhysicsFormula::BoxColl2D(const int_fast16_t &aX, const int_fast16_t &aY, const int_fast16_t &aWidth, const int_fast16_t &aHeight, const int_fast16_t &bX, const int_fast16_t &bY, const int_fast16_t &bWidth, const int_fast16_t &bHeight)
{
    if(((aX - aWidth > bX - bWidth && aX - aWidth < bX + bWidth)  || (bX - bWidth > aX - aWidth && bX - bWidth < aX + aWidth))
    && ((aY - aHeight > bY - bHeight && aY - aHeight < bY + bHeight) || (bY - bHeight > aY - aHeight && bY - bHeight < aY + aHeight))) return true;
    else return false;
}

//center axis coords first, Rota is center by default
bool PhysicsData::_PhysicsFormula::BoxColl2D(const float &aX, const float &aY, const float &aWidth, const float &aHeight, const float &bX, const float &bY, const float &bWidth, const float &bHeight)
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

//center axis coords first, Rota is center by default
bool PhysicsData::_PhysicsFormula::RadialColl2D(const float &aX, const float &aY, const float &aR, const float &bX, const float &bY, const float &bR)
{
    S.x = aX - bX;
    S.y = aY - bY;
    tempf = sqrtf(S.x * S.x + S.y * S.y);
    if(tempf <= aR + aR) return true;

    S.x = bX - aX;
    S.y = bY - aY;
    tempf = sqrtf(S.x * S.x + S.y * S.y);
    if(tempf <= bR + bR) return true;

    return false;
}

//Refresh model collision info beforehand, once per frame
bool PhysicsData::_PhysicsFormula::SphereColl3D(const int &modelH, const DxLib::VECTOR &spherePos, const float &radius, MV1_COLL_RESULT_POLY_DIM &hpd)
{
    hpd = MV1CollCheck_Sphere(modelH, -1, spherePos, radius);
    if(hpd.HitNum >= 1)
    {
        MV1CollResultPolyDimTerminate(hpd);
        return true;
    }
    else return false;
}

//Optional Return
float PhysicsData::_PhysicsFormula::Standardize(float &a, const float &min, const float &max)
{
    if(a < min) a = min;
    if(a > max) a = max;
    tempf = a;
    return tempf;
}

//Optional Return
float PhysicsData::_PhysicsFormula::Humanize(float &value, const float &variation)
{
    random = 0;
    
    srand(static_cast<int_fast32_t>(time(0)));
    dice1 = rand() * 3;
    dice2 = rand() * 7;
    SRand(dice1 * dice2);
    dice1f = (variation * ((float)GetRand(100) * 0.01f)) / 4;
    dice2f = (variation * ((float)(rand() % 100) * 0.01f)) / 4;

    hTemp1 = dice1f + dice2f;
    random += dice1;

    srand(static_cast<int_fast32_t>(time(0)));
    dice1 = rand() * 8;
    dice2 = rand() * 4;
    SRand(dice1 * dice2);
    dice1f = (variation * ((float)GetRand(100) * 0.01f)) / 4;
    dice2f = (variation * ((float)(rand() % 100) * 0.01f)) / 4;

    hTemp2 = dice1f + dice2f;
    random += dice1;

    if(random % 2 == 0) tempf = value + ((hTemp1 + hTemp2) * 2);
    else tempf = value + (((hTemp1 + hTemp2) * 2) * -1);

    value = tempf;
    return tempf;
}

float& PhysicsData::_PhysicsFormula::ApproxAngle(float &objAngle, float &objMainAxisCoord, const float &objInverseAxisAnchor, const int_fast16_t &focalPointCoord, const int_fast16_t &focalPointInverseCoord, const int_fast16_t &screenWidth, const float &turnRate, const int_fast16_t &totalRotPointMulti, const uint_fast8_t &divisor)
{
    tempd = objAngle;

    if(focalPointInverseCoord <= objInverseAxisAnchor)
    {
        temp = -1;
        tempd = DX_PI_F*2;
    }
    else temp = 1;

    while(objMainAxisCoord != focalPointCoord)
    {
        if(objMainAxisCoord > focalPointCoord)
        {
            tempd += ((DX_PI_F*temp)*2/(screenWidth*totalRotPointMulti))/divisor;
            objMainAxisCoord -= turnRate;
        }
        if(objMainAxisCoord < focalPointCoord)
        {
            tempd -= ((DX_PI_F*temp)*2/(screenWidth*totalRotPointMulti))/divisor;
            objMainAxisCoord += turnRate;
        }
    }

    tempd = round(tempd * 10000000)/10000000;
    objAngle = (float)tempd;

    return objAngle;
}

double& PhysicsData::_PhysicsFormula::ApproxAngle(double &objAngle, float &objMainAxisCoord, const float &objInverseAxisAnchor, const int_fast16_t &focalPointCoord, const int_fast16_t &focalPointInverseCoord, const int_fast16_t &screenWidth, const float &turnRate, const int_fast16_t &totalRotPointMulti, const uint_fast8_t &divisor)
{
    if(focalPointInverseCoord <= objInverseAxisAnchor)
    {
        temp = -1;
        objAngle = DX_PI_F*2;
    }
    else temp = 1;

    while(objMainAxisCoord != focalPointCoord)
    {
        if(objMainAxisCoord > focalPointCoord)
        {
            objAngle += ((DX_PI_F*temp)*2/(screenWidth*totalRotPointMulti))/divisor;
            objMainAxisCoord -= turnRate;
        }
        if(objMainAxisCoord < focalPointCoord)
        {
            objAngle -= ((DX_PI_F*temp)*2/(screenWidth*totalRotPointMulti))/divisor;
            objMainAxisCoord += turnRate;
        }
    }
    return objAngle;
}

void PhysicsData::_PhysicsFormula::AnchoredAngle(float &x, float &y, float &angle, const float &anchorX, const float &anchorY, const float &anchorAngle, const float &distance)
{
    x = anchorX, y = anchorY, angle = anchorAngle;
    parent->Propel(x,y,angle,distance);
}

/*  * 3rd Arg Default is 0
    0 = arctan2(y1-y2, squarert(x1-x2*x1-x2 + z1-z2*z1-z2))
    1 = arctan2(z1-z2, squarert(y1-y2*y1-y2 + x1-x2*x1-x2))
    2 = arctan2(x1-x2, squarert(z1-z2*z1-z2 + y1-y2*y1-y2)) */
double PhysicsData::_PhysicsFormula::RelAngle3Precise(const DxLib::VECTOR_D &a, const DxLib::VECTOR_D &b, const uint_fast8_t &formulaOrder)
{
    S_d.x = a.x - b.x;
    S_d.y = a.y - b.y; 
    S_d.z = a.z - b.z;

    switch(formulaOrder)
    {
        case 0:
            tempd = atan2(S_d.y, sqrt(S_d.x*S_d.x + S_d.z*S_d.z));
            break;
        case 1:
            tempd = atan2(S_d.z, sqrt(S_d.y*S_d.y + S_d.x*S_d.x));
            break;
        case 2:
            tempd = atan2(S_d.x, sqrt(S_d.z*S_d.z + S_d.y*S_d.y));
            break;
        default:
            if(DisplayErrorRelAngle3Precise)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Switch Error: PhysicsData RelAngle3Precise() \n switch defaulted"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
            }
            DisplayErrorRelAngle3Precise = FALSE;
            tempd = 0.0f;
            break;
    }

    return tempd;
}

/*  * 3rd Arg Default is 0
    0 = arctan2(y1-y2, squarert(x1-x2*x1-x2 + z1-z2*z1-z2))
    1 = arctan2(z1-z2, squarert(y1-y2*y1-y2 + x1-x2*x1-x2))
    2 = arctan2(x1-x2, squarert(z1-z2*z1-z2 + y1-y2*y1-y2)) */
float PhysicsData::_PhysicsFormula::RelAngle3(const DxLib::VECTOR &a, const DxLib::VECTOR &b, const uint_fast8_t &formulaOrder)
{
    S.x = a.x - b.x;
    S.y = a.y - b.y; 
    S.z = a.z - b.z;

    switch(formulaOrder)
    {
        case 0:
            tempd = atan2((double)S.y, sqrt((double)S.x*S.x + (double)S.z*S.z));
            break;
        case 1:
            tempd = atan2((double)S.z, sqrt((double)S.y*S.y + (double)S.x*S.x));
            break;
        case 2:
            tempd = atan2((double)S.x, sqrt((double)S.z*S.z + (double)S.y*S.y));
            break;
        default:
            if(DisplayErrorRelAngle3)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Switch Error: PhysicsData RelAngle3() \n switch defaulted"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
            }
            DisplayErrorRelAngle3 = FALSE;
            tempd = 0.0f;
            break;
    }

    tempd = round(tempd * 10000000)/10000000;
    tempf = (float)tempd;

    return tempf;
}

/*  * 3rd Arg Default is 0
    0 = arctan2(y1-y2, squarert(x1-x2*x1-x2 + z1-z2*z1-z2))
    1 = arctan2(z1-z2, squarert(y1-y2*y1-y2 + x1-x2*x1-x2))
    2 = arctan2(x1-x2, squarert(z1-z2*z1-z2 + y1-y2*y1-y2)) */
float PhysicsData::_PhysicsFormula::RelAngle3Fast(const DxLib::VECTOR &a, const DxLib::VECTOR &b, const uint_fast8_t &formulaOrder)
{
    S.x = a.x - b.x;
    S.y = a.y - b.y; 
    S.z = a.z - b.z;
    
    switch(formulaOrder)
    {
        case 0:
            tempf = atan2f(S.y, sqrtf(S.x*S.x + S.z*S.z));
            break;
        case 1:
            tempf = atan2f(S.z, sqrtf(S.y*S.y + S.x*S.x));
            break;
        case 2:
            tempf = atan2f(S.x, sqrtf(S.z*S.z + S.y*S.y));
            break;
        default:
            if(DisplayErrorRelAngle3Fast)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Switch Error: PhysicsData RelAngle3Fast() \n switch defaulted"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
            }
            DisplayErrorRelAngle3Fast = FALSE;
            tempf = 0.0f;
            break;
    }

    return tempf;
}

/*  * 3rd Arg Default is 0
    0 = arctan2(x1-x2, z1-z2)
    1 = arctan2(x1-x2, y1-y2)
    2 = arctan2(y1-y2, z1-z2)
    3 = arctan2(y1-y2, x1-x2)
    4 = arctan2(z1-z2, x1-x2)
    5 = arctan2(z1-z2, y1-y2) */
double PhysicsData::_PhysicsFormula::RelAngle2Precise(const DxLib::VECTOR_D &a, const DxLib::VECTOR_D &b, const uint_fast8_t &formulaOrder)
{
    S_d.x = a.x - b.x;
    S_d.y = a.y - b.y;
    S_d.z = a.z - b.z;

    switch(formulaOrder)
    {
        case 0:
            tempd = atan2(S_d.x,S_d.z);
            break;
        case 1:
            tempd = atan2(S_d.x,S_d.y);
            break;
        case 2:
            tempd = atan2(S_d.y,S_d.z);
            break;
        case 3:
            tempd = atan2(S_d.y,S_d.x);
            break;
        case 4:
            tempd = atan2(S_d.z,S_d.x);
            break;
        case 5:
            tempd = atan2(S_d.z,S_d.y);
            break;
        default:
            if(DisplayErrorRelAngle2Precise)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Switch Error: PhysicsData RelAngle2Precise() \n switch defaulted"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
            }
            DisplayErrorRelAngle2Precise = FALSE;
            tempd = 0.0f;
            break;
    }

    return tempd;
}

/*  * 3rd Arg Default is 0
    0 = arctan2(x1-x2, z1-z2)
    1 = arctan2(x1-x2, y1-y2)
    2 = arctan2(y1-y2, z1-z2)
    3 = arctan2(y1-y2, x1-x2)
    4 = arctan2(z1-z2, x1-x2)
    5 = arctan2(z1-z2, y1-y2) */
float PhysicsData::_PhysicsFormula::RelAngle2(const DxLib::VECTOR &a, const DxLib::VECTOR &b, const uint_fast8_t &formulaOrder)
{
    S.x = a.x - b.x;
    S.y = a.y - b.y;
    S.z = a.z - b.z;

    switch(formulaOrder)
    {
        case 0:
            tempd = atan2((double)S.x,(double)S.z); 
            break;
        case 1:
            tempd = atan2((double)S.x,(double)S.y); 
            break;
        case 2:
            tempd = atan2((double)S.y,(double)S.z); 
            break;
        case 3:
            tempd = atan2((double)S.y,(double)S.x); 
            break;
        case 4:
            tempd = atan2((double)S.z,(double)S.x); 
            break;
        case 5:
            tempd = atan2((double)S.z,(double)S.y); 
            break;
        default:
            if(DisplayErrorRelAngle2)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Switch Error: PhysicsData RelAngle2() \n switch defaulted"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
            }
            DisplayErrorRelAngle2 = FALSE;
            tempd = 0.0f;
            break;
    }

    tempd = round(tempd * 10000000)/10000000;
    tempf = (float)tempd;

    return tempf;
}

/*  * 3rd Arg Default is 0
    0 = arctan2(x1-x2, z1-z2)
    1 = arctan2(x1-x2, y1-y2)
    2 = arctan2(y1-y2, z1-z2)
    3 = arctan2(y1-y2, x1-x2)
    4 = arctan2(z1-z2, x1-x2)
    5 = arctan2(z1-z2, y1-y2) */
float PhysicsData::_PhysicsFormula::RelAngle2Fast(const DxLib::VECTOR &a, const DxLib::VECTOR &b, const uint_fast8_t &formulaOrder)
{
    S.x = a.x - b.x;
    S.y = a.y - b.y; 
    S.z = a.z - b.z;

    switch(formulaOrder)
    {
        case 0:
            tempf = atan2f(S.x,S.z);
            break;
        case 1:
            tempf = atan2f(S.x,S.y); 
            break;
        case 2:
            tempf = atan2f(S.y,S.z);
            break;
        case 3:
            tempf = atan2f(S.y,S.x);
            break;
        case 4:
            tempf = atan2f(S.z,S.x);
            break;
        case 5:
            tempf = atan2f(S.z,S.y);
            break;
        default:
            if(DisplayErrorRelAngle2Fast)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Switch Error: PhysicsData RelAngle2Fast() \n switch defaulted"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
            }
            DisplayErrorRelAngle2Fast = FALSE;
            tempf = 0.0f;
            break;
    }

    return tempf;
}

DxLib::VECTOR_D PhysicsData::_PhysicsFormula::Cross3Precise(const DxLib::VECTOR_D &a, const DxLib::VECTOR_D &b)
{
	S_d.x = (a.y * b.z - a.z * b.y);
	S_d.y = (a.z * b.x - a.x * b.z);
	S_d.z = (a.x * b.y - a.y * b.x);

	return S_d;
}

DxLib::VECTOR PhysicsData::_PhysicsFormula::Cross3(const DxLib::VECTOR &a, const DxLib::VECTOR &b)
{
	vec_d1.x = a.x;
    vec_d1.y = a.y;
    vec_d1.z = a.z;

    vec_d2.x = b.x;
    vec_d2.y = b.y;
    vec_d2.z = b.z;
    
	S_d.x = (vec_d1.y * vec_d2.z - vec_d1.z * vec_d2.y);
	S_d.y = (vec_d1.z * vec_d2.x - vec_d1.x * vec_d2.z);
	S_d.z = (vec_d1.x * vec_d2.y - vec_d1.y * vec_d2.x);

    S_d.x = round(S_d.x * 10000000)/10000000;
    S_d.y = round(S_d.y * 10000000)/10000000;
    S_d.z = round(S_d.z * 10000000)/10000000;

    S.x = (float)S_d.x;
    S.y = (float)S_d.y;
    S.z = (float)S_d.z;

	return S;
}

DxLib::VECTOR PhysicsData::_PhysicsFormula::Cross3Fast(const DxLib::VECTOR &a, const DxLib::VECTOR &b)
{
	S.x = (a.y * b.z - a.z * b.y);
	S.y = (a.z * b.x - a.x * b.z);
	S.z = (a.x * b.y - a.y * b.x);

	return S;
}

double PhysicsData::_PhysicsFormula::Dot3Precise(const DxLib::VECTOR_D &a, const DxLib::VECTOR_D &b)
{
    vec_d1.x = a.x;
    vec_d1.y = a.y;
    vec_d1.z = a.z;

    vec_d2.x = b.x;
    vec_d2.y = b.y;
    vec_d2.z = b.z;

	tempd = (vec_d1.x * vec_d2.x + vec_d1.y * vec_d2.y + vec_d1.z * vec_d2.z);

	return tempd;
}

float PhysicsData::_PhysicsFormula::Dot3(const DxLib::VECTOR &a, const DxLib::VECTOR &b)
{
    vec_d1.x = a.x;
    vec_d1.y = a.y;
    vec_d1.z = a.z;

    vec_d2.x = b.x;
    vec_d2.y = b.y;
    vec_d2.z = b.z;

	tempd = (vec_d1.x * vec_d2.x + vec_d1.y * vec_d2.y + vec_d1.z * vec_d2.z);

    tempd = round(tempd * 10000000)/10000000;
    tempf = (float)tempd;

	return tempf;
}

float PhysicsData::_PhysicsFormula::Dot3Fast(const DxLib::VECTOR &a, const DxLib::VECTOR &b)
{
	tempf = (a.x * b.x + a.y * b.y + a.z * b.z);

	return tempf;
}

float PhysicsData::_PhysicsFormula::Distance3(const DxLib::VECTOR &a, const DxLib::VECTOR &b)
{
    vec_d1 = VGetD(a.x,a.y,a.z);
    vec_d2 = VGetD(b.x,b.y,b.z);
    tempd = sqrt(((vec_d1.x - vec_d2.x)*(vec_d1.x - vec_d2.x)) + ((vec_d1.y - vec_d2.y)*(vec_d1.y - vec_d2.y)) + ((vec_d1.z - vec_d2.z)*(vec_d1.z - vec_d2.z)));
    tempd = round(tempd*10000000)/10000000;
    tempf = (float)tempd;
    return tempf;
}

float PhysicsData::_PhysicsFormula::Distance2(const float &x1, const float &x2, const float &y1, const float &y2)
{
    vec_d1.x = x1;
    vec_d1.y = y1;
    vec_d2.x = x2;
    vec_d2.y = y2;
    tempd = sqrt(((vec_d1.x - vec_d2.x)*(vec_d1.x - vec_d2.x)) + ((vec_d1.y - vec_d2.y)*(vec_d1.y - vec_d2.y)));
    tempd = round(tempd*10000000)/10000000;
    tempf = (float)tempd;
    return tempf;
}

float PhysicsData::_PhysicsFormula::Distance1(const float &a, const float &b)
{
    vec_d1.x = a;
    vec_d2.x = b;
    tempd = sqrt(((vec_d1.x - vec_d2.x)*(vec_d1.x - vec_d2.x)));
    tempd = round(tempd*10000000)/10000000;
    tempf = (float)tempd;
    return tempf;
}

//Returns converted Vector
DxLib::VECTOR_D PhysicsData::_PhysicsFormula::ConvertVector(const DxLib::VECTOR &input)
{
    S_d.x = input.x;
    S_d.y = input.y;
    S_d.z = input.z;

    return S_d;
}

//Returns converted Vector
DxLib::VECTOR PhysicsData::_PhysicsFormula::ConvertVector(const DxLib::VECTOR_D &input)
{
    S_d.x = input.x;
    S_d.y = input.y;
    S_d.z = input.z;

    S_d.x = round(S_d.x * 10000000)/10000000;
    S_d.y = round(S_d.y * 10000000)/10000000;
    S_d.z = round(S_d.z * 10000000)/10000000;
    
    S.x = (float)S_d.x;
    S.y = (float)S_d.y;
    S.z = (float)S_d.z;

    return S;
}

//'L' / 'R'
void PhysicsData::Spin(float &angle, const char &L_or_R_Direction, const uint_fast8_t &totalRotPoints)
{
    if(L_or_R_Direction == 'R') angle += DX_PI_F/totalRotPoints;
    if(L_or_R_Direction == 'L') angle -= DX_PI_F/totalRotPoints;
}

//'L' / 'R'
void PhysicsData::Spin(double &angle, const char &L_or_R_Direction, const uint_fast8_t &totalRotPoints)
{
    if(L_or_R_Direction == 'R') angle += DX_PI_F/totalRotPoints;
    if(L_or_R_Direction == 'L') angle -= DX_PI_F/totalRotPoints;
}

/*  Singular axis movement
    Fling Directions: FLING_DOWN, FLING_UP, FLING_RIGHT, FLING_LEFT
    - able to GetLastValue() [ velocity, inertia, gravity ] */
bool PhysicsData::Fling(int_fast16_t &position, int_fast16_t destination, const uint_fast8_t ENUM_FLING_DIRECTION, uint_fast16_t speed, float grav, float iMulti)
{
    gravity.x = grav;
    gravity.y = grav;

    switch(ENUM_FLING_DIRECTION)
    {
        case FLING_DOWN:
            if(position <= destination-1)
            {
                if(destination - position > 200) {velocity.y = ((speed*8)*gravity.y)*iMulti; position += (int)floor(velocity.y);}
                if(destination - position > 40) {velocity.y = (speed*4)*gravity.y; position += (int)floor(velocity.y);}
                if(destination - position > 15) {velocity.y = (speed*2)*gravity.y; position += (int)floor(velocity.y);}
                if(destination - position > 1) {velocity.y = ((speed*1)*gravity.y)*iMulti; position += (int)floor(velocity.y);}
            }
            if(position == destination-1) position++;
            break;
        case FLING_UP:
            if(position >= destination+1)
            {
                if(position - destination > 200) {velocity.y = ((speed*8)*gravity.y)*iMulti; position -= (int)floor(velocity.y);}
                if(position - destination > 40) {velocity.y = (speed*4)*gravity.y; position -= (int)floor(velocity.y);}
                if(position - destination > 15) {velocity.y = (speed*2)*gravity.y; position -= (int)floor(velocity.y);}
                if(position - destination > 1) {velocity.y = ((speed*1)*gravity.y)*iMulti; position -= (int)floor(velocity.y);}
            }
            if(position == destination+1) position--;
            break;
        case FLING_LEFT:
            if(position >= destination+1)
            {
                if(position - destination > 200) {velocity.x = ((speed*8)*gravity.x)*iMulti; position -= (int)floor(velocity.x);}
                if(position - destination > 40) {velocity.x = (speed*4)*gravity.x; position -= (int)floor(velocity.x);}
                if(position - destination > 15) {velocity.x = (speed*2)*gravity.x; position -= (int)floor(velocity.x);}
                if(position - destination > 1) {velocity.x = ((speed*1)*gravity.x)*iMulti; position -= (int)floor(velocity.x);}
            }
            if(position == destination+1) position--;
            break;
        case FLING_RIGHT:
            if(position <= destination-1)
            {
                if(destination - position > 200) {velocity.x = ((speed*8)*gravity.x)*iMulti; position += (int)floor(velocity.x);}
                if(destination - position > 40) {velocity.x = (speed*4)*gravity.x; position += (int)floor(velocity.x);}
                if(destination - position > 15) {velocity.x = (speed*2)*gravity.x; position += (int)floor(velocity.x);}
                if(destination - position > 1) {velocity.x = ((speed*1)*gravity.x)*iMulti; position += (int)floor(velocity.x);}
            }
            if(position == destination-1) position++;
            break;
    }
    if(position != destination) return true;
    else return false;
}

// GetLast: LAST_VELOCITY_X, LAST_VELOCITY_Y 
void PhysicsData::Propel(float &x, float &y, const float &angle2D, const float &magnitude)
{
    tempd = sin((double)angle2D)*magnitude;
    tempd = round(tempd * 10000000)/10000000;
    velocity.x = (float)tempd;

    tempd = cos((double)angle2D)*magnitude;
    tempd = round(tempd * 10000000)/10000000;
    velocity.y = (float)tempd;
    
    x += velocity.x;
    y -= velocity.y;
}

// GetLast: LAST_VELOCITY_X, LAST_VELOCITY_Y 
void PhysicsData::PropelFast(float &x, float &y, const float &angle2D, const float &magnitude)
{
    tempf = sin(angle2D)*magnitude;
    velocity.x = tempf;

    tempf = cos(angle2D)*magnitude;
    velocity.y = tempf;
    
    x += velocity.x;
    y -= velocity.y;
}

/*  assign last velocity to Body.Vel if you want to use this with Manipulate()
    - GetLast: LAST_VELOCITY_X, LAST_VELOCITY_Y */  
void PhysicsData::Propel(PhysicsBody_t &Body, const float &magnitude, const float &offsetH, const float &offsetV)
{
    switch(Body.Enable3D)
    {
        case FALSE:

            tempd = sin((double)Body.Angle + offsetH)*magnitude;
            tempd = round(tempd * 10000000)/10000000;
            velocity.x = (float)tempd;

            tempd = cos((double)Body.Angle + offsetH)*magnitude;
            tempd = round(tempd * 10000000)/10000000;
            velocity.y = (float)tempd;
            
            Body.Pos.x += velocity.x;
            Body.Pos.y -= velocity.y;
            break;

        case TRUE:

            tempd = sin((double)Body.Rot.y + offsetH)*magnitude;
            tempd = round(tempd * 10000000)/10000000;
            velocity.x = (float)tempd;

            tempd = cos((double)Body.Rot.y + offsetH)*magnitude;
            tempd = round(tempd * 10000000)/10000000;
            velocity.z = (float)tempd;

            tempd = sin((double)Body.Rot.x + offsetV)*magnitude;
            tempd = round(tempd * 10000000)/10000000;
            velocity.y = (float)tempd;
            
            Body.Pos.x -= velocity.x; 
            Body.Pos.z -= velocity.z; 
            Body.Pos.y += velocity.y;
            break;
    }
}

/*  assign last velocity to Body.Vel if you want to use this with Manipulate()
    - GetLast: LAST_VELOCITY_X, LAST_VELOCITY_Y */  
void PhysicsData::PropelFast(PhysicsBody_t &Body, const float &magnitude, const float &offsetH, const float &offsetV)
{
    switch(Body.Enable3D)
    {
        case FALSE:

            tempf = sin(Body.Angle + offsetH)*magnitude;
            velocity.x = tempf;

            tempf = cos(Body.Angle + offsetH)*magnitude;
            velocity.y = tempf;
            
            Body.Pos.x += velocity.x;
            Body.Pos.y -= velocity.y;
            break;

        case TRUE:

            tempf = sin(Body.Rot.y + offsetH)*magnitude;
            velocity.x = tempf;

            tempf = cos(Body.Rot.y + offsetH)*magnitude;
            velocity.z = tempf;

            tempf = sin(Body.Rot.x + offsetV)*magnitude;
            velocity.y = tempf;
            
            Body.Pos.x -= velocity.x; 
            Body.Pos.z -= velocity.z; 
            Body.Pos.y += velocity.y;
            break;
    }
}

/*  Singular Axis application
    - Add/Sub/Reset Accel as necessary
    - Convert the end value to negative as necessary */
float PhysicsData::Accelerate(float &vel, const float &velBase, const float &velMax, const float &accel, const float &grav)
{
    tempd = velBase;
    tempd = tempd + (accel * grav);
    if(tempd > velMax) tempd = velMax;
    if(tempd != velMax && tempd != velBase) tempd = round(tempd*10000000)/10000000;
    tempf = (float)tempd;

    vel = tempf;
    return tempf;
}

/*  Singular Axis application
    - Add/Sub/Reset Accel beforehand
    - Convert the end value to negative as necessary 
    AXIS_X, AXIS_Y, AXIS_Z*/
float PhysicsData::Accelerate(PhysicsBody_t &Body, const uint_fast8_t &ENUM_AXIS)
{
    switch(ENUM_AXIS)
    {
        case AXIS_X:
            vel = &Body.Vel.x;
            break;
        case AXIS_Y:
            vel = &Body.Vel.y;
            break;
        case AXIS_Z:
            vel = &Body.Vel.z;
            break;
        default:
            if(DisplayErrorAxisEnum)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Enum Error: PhysicsData Accelerate()"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
                DisplayErrorAxisEnum = FALSE;
            }
            return 0.0f;
    }

    Body.Accel = Body.Accel + Body.AccelRate;
    tempf = Body.VelBase + Body.Accel;

    if(tempf > Body.VelMax) tempf = Body.VelMax;
    *vel = tempf;

    return tempf;
}

void PhysicsData::Manipulate(int_fast16_t &x, int_fast16_t &y, float &velX, float &velY, PhysicsLastTime_t &Last, const uint_fast32_t &interval, const float &gravX, const float &gravY, const float &decay)
{
    gravity.x = gravX;
    gravity.y = gravY;
    velocity.x = (velX*decay)*gravity.x;
    velocity.y = (velY*decay)*gravity.y;

    if(Delta.Time(Last, interval))
    {
        if(velX > 0) velX = floor(velocity.x);
        if(velX < 0) velX = ceil(velocity.x);
        if(velY > 0) velY = floor(velocity.y);
        if(velY < 0) velY = ceil(velocity.y);
    }

    x += (int)velX;
    y += (int)velY;
}

void PhysicsData::Manipulate(float &x, float &y, float &velX, float &velY, PhysicsLastTime_t &Last, const uint_fast32_t &interval, const float &gravX, const float &gravY, const float &decay)
{
    gravity.x = gravX;
    gravity.y = gravY;
    velocity.x = (velX*decay)*gravity.x;
    velocity.y = (velY*decay)*gravity.y;

    if(Delta.Time(Last, interval))
    {
        if(velX > 0) velX = floor(velocity.x * 1000)/1000;
        if(velX < 0) velX = ceil(velocity.x * 1000)/1000;
        if(velY > 0) velY = floor(velocity.y * 1000)/1000;
        if(velY < 0) velY = ceil(velocity.y * 1000)/1000;
    }

    x += velX;
    y += velY;
}      

/*  Uses world_gravity
    Frequency of force decay is set by Body.Interval
    - Optional gravity-snap args to prevent perpetual axis flips
    - Be mindful of world_grav xyz pos and snap range, set unused grav axis pos off grid (-10000)
    - GetLastValue: [Gravity will return 0 on contact]
    LAST_VELOCITY_X, LAST_VELOCITY_Y, LAST_VELOCITY_Z, 
    LAST_GRAVITY_X, LAST_GRAVITY_Y, LAST_GRAVITY_Z */
void PhysicsData::Manipulate(PhysicsBody_t &Body, const uint_fast8_t &snapX, const uint_fast8_t &snapY, const uint_fast8_t &snapZ)
{//diminishing pull with range
    gravity.x = (world_gravity_multi.x) * (GRAV_PULL_RATE * (((world_gravity_range.x + sqrtf(world_gravity_pos.x*world_gravity_pos.x)) - (sqrtf((Body.Pos.x - world_gravity_pos.x) * (Body.Pos.x - world_gravity_pos.x)) + sqrtf(world_gravity_pos.x*world_gravity_pos.x))) / world_gravity_range.x));
    gravity.y = (world_gravity_multi.y) * (GRAV_PULL_RATE * (((world_gravity_range.y + sqrtf(world_gravity_pos.y*world_gravity_pos.y)) - (sqrtf((Body.Pos.y - world_gravity_pos.y) * (Body.Pos.y - world_gravity_pos.y)) + sqrtf(world_gravity_pos.y*world_gravity_pos.y))) / world_gravity_range.y));
    gravity.z = (world_gravity_multi.z) * (GRAV_PULL_RATE * (((world_gravity_range.z + sqrtf(world_gravity_pos.z*world_gravity_pos.z)) - (sqrtf((Body.Pos.z - world_gravity_pos.z) * (Body.Pos.z - world_gravity_pos.z)) + sqrtf(world_gravity_pos.z*world_gravity_pos.z))) / world_gravity_range.z));

    if(Delta.Time(Body.Time, Body.Interval)) //F = M x S <- actual physicists please ignore this, thanks
    {
        ForceDecay(Body,AXIS_X);
        ForceDecay(Body,AXIS_Y);
        ForceDecay(Body,AXIS_Z);

        if(Body.Grounded) Body.Vel = VGet(Body.Vel.x * Body.FrictionRatio, Body.Vel.y * Body.FrictionRatio, Body.Vel.z * Body.FrictionRatio);
    }
    
    gravity = VGet(gravity.x + Body.gForce.x, gravity.y + Body.gForce.y, gravity.z + Body.gForce.z);
  
    if(Body.Pos.x > world_gravity_pos.x) gravity.x *= -1;
    if(Body.Pos.y > world_gravity_pos.y) gravity.y *= -1;
    if(Body.Pos.z > world_gravity_pos.z) gravity.z *= -1;

    velocity = VGet(Body.Vel.x + gravity.x, Body.Vel.y + gravity.y, Body.Vel.z + gravity.z);

    Body.Pos = VGet(Body.Pos.x + velocity.x, Body.Pos.y + velocity.y, Body.Pos.z + velocity.z);
    
    if(Formula.Distance1(Body.Pos.x,world_gravity_pos.x) < snapX) gravity.x = 0.0f, Body.Pos.x = world_gravity_pos.x, Body.gForce.x = 0.0f;
    if(Formula.Distance1(Body.Pos.y,world_gravity_pos.y) < snapY) gravity.y = 0.0f, Body.Pos.y = world_gravity_pos.y, Body.gForce.y = 0.0f;
    if(Formula.Distance1(Body.Pos.z,world_gravity_pos.z) < snapZ) gravity.z = 0.0f, Body.Pos.z = world_gravity_pos.z, Body.gForce.z = 0.0f;
}

/* from Propel() / Fling() / Manipulate()
GetLastValue:
LAST_VELOCITY_X, LAST_VELOCITY_Y, LAST_VELOCITY_Z,
LAST_GRAVITY_X, LAST_GRAVITY_Y, LAST_GRAVITY_Z */
float PhysicsData::GetLastValue(const uint_fast8_t &ENUM_LAST)
{
    switch(ENUM_LAST)
    {
        case LAST_VELOCITY_X:
            return velocity.x;
        case LAST_VELOCITY_Y:
            return velocity.y;
        case LAST_VELOCITY_Z:
            return velocity.z;
        case LAST_GRAVITY_X: 
            return gravity.x;
        case LAST_GRAVITY_Y:
            return gravity.y;
        case LAST_GRAVITY_Z:
            return gravity.z;
        default:
            if(DisplayErrorGetEnum)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Enum Error: PhysicsData GetLastValue()"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
                DisplayErrorGetEnum = FALSE;
            }
            return 0.0f;
    }
}

/*  0.0 = (No Gravity)
    a multiplier of 1x applies the full power gravity pull at close range
    higher than 1x increases the strength of the field's Pull */
void PhysicsData::SetWorldGravityMulti(const float &x, const float &y, const float &z)
{
    world_gravity_multi = VGet(x,y,z);
}

void PhysicsData::SetWorldGravityPos(const float &x, const float &y, const float &z)
{
    world_gravity_pos = VGet(x,y,z);
}

/*  Range that the axial gravity should extend
    ie: x + 500range = a field in which zero-g happens when 500 from the x 
    - Don't set to zero (divide error) */
void PhysicsData::SetWorldGravityRange(const float &x, const float &y, const float &z)
{
    if(x == 0 || y == 0 || z == 0)
    {
        if(DisplayErrorGravRange)
        {
            MessageBox
            (
                NULL,
                TEXT("Grav Error: PhysicsData SetWorldGravityRange() \nValue set to zero, divide error"),
                TEXT("Error"),
                MB_OK | MB_ICONERROR 
            );
            DisplayErrorGravRange = FALSE;
        }
    }

    world_gravity_range = VGet(x,y,z);
}

float PhysicsData::GetWorldGravityMulti(const uint_fast8_t &ENUM_AXIS)
{
    switch(ENUM_AXIS)
    {
        case AXIS_X:
            return world_gravity_multi.x;
        case AXIS_Y:
            return world_gravity_multi.y;
        case AXIS_Z:
            return world_gravity_multi.z;
        default:
            if(DisplayErrorAxisEnum)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Enum Error: PhysicsData GetWorldGravityMulti()"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
                DisplayErrorAxisEnum = FALSE;
            }
            return 0.0f;
    }
}

float PhysicsData::GetWorldGravityPos(const uint_fast8_t &ENUM_AXIS)
{
    switch(ENUM_AXIS)
    {
        case AXIS_X:
            return world_gravity_pos.x;
        case AXIS_Y:
            return world_gravity_pos.y;
        case AXIS_Z:
            return world_gravity_pos.z;
        default:
            if(DisplayErrorAxisEnum)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Enum Error: PhysicsData GetWorldGravityPos()"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
                DisplayErrorAxisEnum = FALSE;
            }
            return 0.0f;
    }
}

float PhysicsData::GetWorldGravityRange(const uint_fast8_t &ENUM_AXIS)
{
    switch(ENUM_AXIS)
    {
        case AXIS_X:
            return world_gravity_range.x;
        case AXIS_Y:
            return world_gravity_range.y;
        case AXIS_Z:
            return world_gravity_range.z;
        default:
            if(DisplayErrorAxisEnum)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Enum Error: PhysicsData GetWorldGravityRange()"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
                DisplayErrorAxisEnum = FALSE;
            }
            return 0.0f;
    }
}


//center axis coords first, Rota is center by default
void PhysicsData::DrawHitBox(const int_fast16_t &x, const int_fast16_t &y, const int_fast16_t &w, const int_fast16_t &h, const int_fast32_t &color, const uint_fast8_t &fillFlag)
{
    DrawBox(x - w, y - h, x + w, y + h, color, fillFlag);
}

//center axis coords first, Rota is center by default
void PhysicsData::DrawHitCircle(const int_fast16_t &x, const int_fast16_t &y, const int_fast16_t &collRadius, const int_fast32_t &color, const uint_fast8_t &fillFlag)
{
    DrawCircle(x, y, collRadius, color, fillFlag);
}

//see you on the other side, slick!
int PhysicsData::ForceDecay(PhysicsBody_t &Body, const uint_fast8_t &ENUM_AXIS)
{
    switch(ENUM_AXIS) //i'll never get a programmer job
    {//but this is kinda fun
        case AXIS_X:
            tempd = Body.Vel.x;
            vel = &Body.Vel.x;
            gForce = &Body.gForce.x;
            grav = &gravity.x;
            break;
        case AXIS_Y:
            tempd = Body.Vel.y;
            vel = &Body.Vel.y;
            gForce = &Body.gForce.y;
            grav = &gravity.y;
            break;
        case AXIS_Z:
            tempd = Body.Vel.z;
            vel = &Body.Vel.z;
            gForce = &Body.gForce.z;
            grav = &gravity.z;
            break;
        default:
            if(DisplayErrorAxisEnum)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Enum Error: PhysicsData ForceDecay()\n Private function"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
                DisplayErrorAxisEnum = FALSE;
            }
            return -1;
    }

    if(*vel > 0.00f) 
    {
        *vel = (float)floor((tempd * Body.MassRatio) * 10000000)/10000000;
        *gForce -= GRAV_PULL_RATE;
        if(*vel < 0.3f) *vel = 0.00f;
    }
    else if(*vel < 0.00f) 
    {
        *vel = (float)ceil((tempd * Body.MassRatio) * 10000000)/10000000;
        *gForce -= GRAV_PULL_RATE;
        if(*vel > -0.3f) *vel = 0.00f;
    }

    *gForce += *grav/2;
    if(*gForce > Body.TermVel) *gForce = Body.TermVel;
    if(*gForce < 0.0004f) *gForce = 0.00f;

    return 0; //things happened
}