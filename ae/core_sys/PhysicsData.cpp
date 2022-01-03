#include "DxLib.h"
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "PhysicsData.h"

static bool DisplayErrorGetEnum = TRUE;
static bool DisplayErrorRelAngle3 = TRUE, DisplayErrorRelAngle3Precise = TRUE, DisplayErrorRelAngle3Fast = TRUE;
static bool DisplayErrorRelAngle2 = TRUE, DisplayErrorRelAngle2Precise = TRUE, DisplayErrorRelAngle2Fast = TRUE;

static uint_fast8_t random;
static int_fast16_t temp, dice1, dice2;
static float tempf, dice1f, dice2f, hTemp1, hTemp2;
static double tempd;
static VECTOR S;
static VECTOR_D S_d, vec_d1, vec_d2;

PhysicsData::PhysicsData(float a)
:Decay(a)
{
    //to check or use with Get()
    velocity_x = 0.00f, velocity_y = 0.00f;
    inertia_x = 0.00f, inertia_y = 0.00f;
    gravity_x = 0.00f, gravity_y = 0.00f;
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

void PhysicsData::_PhysicsFormula::AnchoredAngle(float &x, float &y, double &angle, const float &anchorX, const float &anchorY, const double &anchorAngle, const uint_fast16_t &distance)
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

double PhysicsData::_PhysicsFormula::Dot2Precise(const double &a, const double &b)
{
	tempd = (a*a+b*b);

	return tempd;
}

float PhysicsData::_PhysicsFormula::Dot2(const double &a, const double &b)
{
	tempd = (a*a+b*b);

    tempd = round(tempd * 10000000)/10000000;
    tempf = (float)tempd;

	return tempf;
}

float PhysicsData::_PhysicsFormula::Dot2Fast(const float &a, const float &b)
{
	tempf = (a*a+b*b);
	return tempf;
}

DxLib::VECTOR_D PhysicsData::_PhysicsFormula::ConvertVector(const DxLib::VECTOR &a)
{
    S_d.x = a.x;
    S_d.y = a.y;
    S_d.z = a.z;

    return S_d;
}

DxLib::VECTOR PhysicsData::_PhysicsFormula::ConvertVector(const DxLib::VECTOR_D &a)
{
    S_d.x = a.x;
    S_d.y = a.y;
    S_d.z = a.z;

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
    Fling Directions: FLING_DOWN, FLING_UP, FLING_RIGHT, FLING_LEFT */
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
                if(destination - position > 200) {velocity_y = ((speed*8)*gravity_y)*iMulti; position += (int)floor(velocity_y);}
                if(destination - position > 40) {velocity_y = (speed*4)*gravity_y; position += (int)floor(velocity_y);}
                if(destination - position > 15) {velocity_y = (speed*2)*gravity_y; position += (int)floor(velocity_y);}
                if(destination - position > 1) {velocity_y = ((speed*1)*gravity_y)*iMulti; position += (int)floor(velocity_y);}
            }
            if(position == destination-1) position++;
            break;
        case FLING_UP:
            if(position >= destination+1)
            {
                if(position - destination > 200) {velocity_y = ((speed*8)*gravity_y)*iMulti; position -= (int)floor(velocity_y);}
                if(position - destination > 40) {velocity_y = (speed*4)*gravity_y; position -= (int)floor(velocity_y);}
                if(position - destination > 15) {velocity_y = (speed*2)*gravity_y; position -= (int)floor(velocity_y);}
                if(position - destination > 1) {velocity_y = ((speed*1)*gravity_y)*iMulti; position -= (int)floor(velocity_y);}
            }
            if(position == destination+1) position--;
            break;
        case FLING_LEFT:
            if(position >= destination+1)
            {
                if(position - destination > 200) {velocity_x = ((speed*8)*gravity_x)*iMulti; position -= (int)floor(velocity_x);}
                if(position - destination > 40) {velocity_x = (speed*4)*gravity_x; position -= (int)floor(velocity_x);}
                if(position - destination > 15) {velocity_x = (speed*2)*gravity_x; position -= (int)floor(velocity_x);}
                if(position - destination > 1) {velocity_x = ((speed*1)*gravity_x)*iMulti; position -= (int)floor(velocity_x);}
            }
            if(position == destination+1) position--;
            break;
        case FLING_RIGHT:
            if(position <= destination-1)
            {
                if(destination - position > 200) {velocity_x = ((speed*8)*gravity_x)*iMulti; position += (int)floor(velocity_x);}
                if(destination - position > 40) {velocity_x = (speed*4)*gravity_x; position += (int)floor(velocity_x);}
                if(destination - position > 15) {velocity_x = (speed*2)*gravity_x; position += (int)floor(velocity_x);}
                if(destination - position > 1) {velocity_x = ((speed*1)*gravity_x)*iMulti; position += (int)floor(velocity_x);}
            }
            if(position == destination-1) position++;
            break;
    }
    if(position != destination) return true;
    else return false;
}

void PhysicsData::Propel(float &x, float &y, const double &angle, const uint_fast16_t &magnitude)
{
    tempd = sin(angle)*magnitude;
    tempd = round(tempd * 10000000)/10000000;
    velocity_x = (float)tempd;

    tempd = cos(angle)*magnitude;
    tempd = round(tempd * 10000000)/10000000;
    velocity_y = (float)tempd;
    
    x += velocity_x;
    y -= velocity_y;
}       

//Apply accel
void PhysicsData::Accelerate(float &vel, const float &velBase, const float &velMax, const float &accel, const float &grav)
{
    inertia_x = accel;
    inertia_y = accel;
    gravity_x = grav;
    gravity_y = grav;

    tempd = velBase;
    tempd = tempd + (accel * grav);
    if(tempd > velMax) tempd = velMax;

    tempd = round(tempd*10000000)/10000000;
    velocity_x = (float)tempd;
    velocity_y = (float)tempd;
    vel = (float)tempd;
}

void PhysicsData::Manipulate(int_fast16_t &x, int_fast16_t &y, float &vel_x, float &vel_y, PhysicsLastTime_t &Last, const uint_fast32_t &decayInterval, const float &grav_x, const float &grav_y)
{
    velocity_x = vel_x;
    velocity_y = vel_y;
    gravity_x = grav_x;
    gravity_y = grav_y;
    inertia_x = (velocity_x*Decay)*gravity_x;
    inertia_y = (velocity_y*Decay)*gravity_y;

    if(Delta.Time(Last, decayInterval))
    {
        if(vel_x > 0) vel_x = floor(inertia_x);
        if(vel_x < 0) vel_x = ceil(inertia_x);
        if(vel_y > 0) vel_y = floor(inertia_y);
        if(vel_y < 0) vel_y = ceil(inertia_y);
    }

    x += (int)vel_x;
    y += (int)vel_y;
}

void PhysicsData::Manipulate(float &x, float &y, float &vel_x, float &vel_y, PhysicsLastTime_t &Last, const uint_fast32_t &decayInterval, const float &grav_x, const float &grav_y)
{
    velocity_x = vel_x;
    velocity_y = vel_y;
    gravity_x = grav_x;
    gravity_y = grav_y;
    inertia_x = (velocity_x*Decay)*gravity_x;
    inertia_y = (velocity_y*Decay)*gravity_y;

    if(Delta.Time(Last, decayInterval))
    {
        if(vel_x > 0) vel_x = floor(inertia_x * 1000)/1000;
        if(vel_x < 0) vel_x = ceil(inertia_x * 1000)/1000;
        if(vel_y > 0) vel_y = floor(inertia_y * 1000)/1000;
        if(vel_y < 0) vel_y = ceil(inertia_y * 1000)/1000;
    }

    x += vel_x;
    y += vel_y;
}       

/* Get Values: DECAY, LAST_VELOCITY_X, LAST_VELOCITY_Y, LAST_INERTIA_X, 
LAST_INERTIA_Y, LAST_GRAVITY_X, LAST_GRAVITY_Y */
float PhysicsData::Get(const uint_fast8_t &ENUM_GET)
{
    //welcome to the leaning tower of switchisa
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
        default:
            if(DisplayErrorGetEnum)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Enum Error: PhysicsData Get()"),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
                DisplayErrorGetEnum = FALSE;
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