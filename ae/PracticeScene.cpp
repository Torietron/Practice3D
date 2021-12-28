#include "DxLib.h"
#include <cmath>
#include "SceneControl.h"
#include "ScreenControl.h"
#include "PracticeScene.h"
#include "KeyPoll.h"
#include "MousePoll.h"

#define SPHERES 2

static const float ROTATE_SPEED = DX_PI_F/45;
static const float MOVEMENT_SPEED = DX_PI_F/5;

typedef struct {
	VECTOR v;
	float r;
	bool active;
} Sphere_t;

static int_fast8_t Selected = -1; //-1 = no target
static uint_fast8_t pIndex = 0, AnimSet = 0, Destroyed = 0, fluxReverse = FALSE; 
static uint_fast8_t pReverse = FALSE, pJump = FALSE, pGrounded = TRUE, CameraLock = TRUE, TargetLock = FALSE;
static uint_fast8_t SDFlag[SPHERES] = {0};
static int_fast32_t pPace = 0;
static int ModelH = 0, TargetH, Light;
static int AttachIndex = 0;
static float TotalTime, PlayTime = 0.0f, vOffset = 0.46f, cZoom = 0.0f;
static float anchorY = 0.0f, angleV = 0.0f, angleH = 0.0f, flux = 0.0f; 
static float targetX = 0.0f, targetY = 10.0f, targetZ = 0.0f, markerSize = 0.0f;
static VECTOR Camera, Player, pRot, pOffset, cRot, S;

MV1_COLL_RESULT_POLY_DIM HitPolyDim[SPHERES];
Sphere_t sphere[SPHERES];

extern bool debugflag;
extern ScreenControl Screen;
extern MousePoll Mouse;
extern KeyPoll Key;
extern SceneControl Scene;

DxLib::VECTOR SetCross(const VECTOR &a, const VECTOR &b);
float Dot3(const VECTOR &a, const VECTOR &b);
float Dot2(const float &x, const float &z);

void PracticeScene::Init()
{
    Camera = VGet(0.0f, 20.0f, -20.0f);
    Player = VGet(42.0f, 0.0f, 52.0f);
    pRot = VGet(0.0f, (DX_PI_F/5) * -1, 0.0f);
    cRot = VGet(0.0f, (DX_PI_F/5) * -1, 0.0f);

    SDFlag[0] = 1, SDFlag[1] = 1;
    sphere[0].v = VGet(-0.5f,10.0f,-46.0f), sphere[0].r = 6.0f, sphere[0].active = true;
    sphere[1].v = VGet(-40.0f,10.0f,-60.0f), sphere[1].r = 6.0f, sphere[1].active = true;
}

void PracticeScene::Load()
{
    if(ModelH != 0) //if already set
    {
        MV1DetachAnim(ModelH,AttachIndex);
        MV1DeleteModel(ModelH);
    }
    SetCameraNearFar(0.1f, 1000.0f);
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    TargetH = LoadGraph(_T("core/ph3.png"));
    Light = CreatePointLightHandle(VGet(0.0f,3000.0f,0.0f),3000.0f,0.2f,0.002f,0.0f);
    Screen.CursorH = LoadGraph(_T("core/ph7.png"));
    SetLightPositionHandle(Light,VGet(0.0f,500.0f,0.0f));
    ModelH = MV1LoadModel(_T("dat/Lat/LatMikuVer2.3_SailorWinter.pmd"));
    AttachIndex = MV1AttachAnim(ModelH, 0, -1, FALSE);
    TotalTime = MV1GetAttachAnimTotalTime(ModelH,AttachIndex);
    MV1SetupCollInfo(ModelH, -1, 1, 1, 1);
    pPace = 0;
}

void PracticeScene::End()
{
    MV1DetachAnim(ModelH,AttachIndex);
    MV1DeleteModel(ModelH);
}

void PracticeScene::Update()
{
    //Controls
    {
        Key.Update();
        Mouse.Update();
            
        //Player
        if(Key.Poll[KEY_INPUT_Q] >= 1) pRot.y -= ROTATE_SPEED;
        if(Key.Poll[KEY_INPUT_E] >= 1) pRot.y += ROTATE_SPEED;
        if(Key.Poll[KEY_INPUT_A] >= 1)
        {
            Player.z -= cos(pRot.y-(DX_PI_F/2))*MOVEMENT_SPEED;
            Player.x -= sin(pRot.y-(DX_PI_F/2))*MOVEMENT_SPEED;
            pRot.x = DX_PI_F*2;
            if(pGrounded == TRUE) pIndex = 1;
            else pIndex = 2;
            pReverse = FALSE;
            pOffset.y = (DX_PI_F/2) * -1;
        }
        if(Key.Poll[KEY_INPUT_D] >= 1)
        {
            Player.z -= cos(pRot.y+(DX_PI_F/2))*MOVEMENT_SPEED;
            Player.x -= sin(pRot.y+(DX_PI_F/2))*MOVEMENT_SPEED;
            pRot.x = DX_PI_F*2;
            if(pGrounded == TRUE) pIndex = 1;
            else pIndex = 2;
            pReverse = FALSE;
            pOffset.y = DX_PI_F/2;
        }
        if(Key.Poll[KEY_INPUT_W] >= 1) 
        {
            Player.z -= cos(pRot.y)*MOVEMENT_SPEED;
            Player.x -= sin(pRot.y)*MOVEMENT_SPEED;
            pRot.x = DX_PI_F*2;
            if(pGrounded == TRUE) pIndex = 1;
            else pIndex = 2;
            pReverse = FALSE;
            pOffset.y = 0;
            if(Key.Poll[KEY_INPUT_A] >= 1) pOffset.y = (DX_PI_F/4) * -1;
            if(Key.Poll[KEY_INPUT_D] >= 1) pOffset.y = DX_PI_F/4;
        }
        if(Key.Poll[KEY_INPUT_S] >= 1) 
        {
            Player.z += cos(pRot.y)*(MOVEMENT_SPEED*0.75f);
            Player.x += sin(pRot.y)*(MOVEMENT_SPEED*0.75f);
            pRot.x = DX_PI_F/10; //Center of gravity would've been visually inconsistent
            if(pGrounded == TRUE) pIndex = 1;
            else pIndex = 2;
            pReverse = TRUE;
            pOffset.y = 0;
            if(Key.Poll[KEY_INPUT_A] >= 1) pOffset.y = DX_PI_F/7;
            if(Key.Poll[KEY_INPUT_D] >= 1) pOffset.y = (DX_PI_F/7)* -1;
        }
        if(Key.Poll[KEY_INPUT_SPACE] == 1)
        {
            if(pGrounded == TRUE) 
            {
                anchorY = Player.y;
                pPace = 0;
                pJump = TRUE;
                pGrounded = FALSE;
                pRot.x = DX_PI_F*2;
                pIndex = 2;
            }
        }
        if(Key.Poll[KEY_INPUT_W] == 0 && Key.Poll[KEY_INPUT_S] == 0
        && Key.Poll[KEY_INPUT_A] == 0 && Key.Poll[KEY_INPUT_D] == 0)
        {
            if(pGrounded == TRUE) pIndex = 0, pRot.x = DX_PI_F*2, pReverse = FALSE;
            if(pGrounded == FALSE) pIndex = 2, pRot.x = DX_PI_F*2, pReverse = FALSE;
            pOffset.y = 0.0f;
        }

        //Camera
        //keep the camera a set distance from player
        //cam's circular anchored point around the player is controlled by cRot.y
        if(Key.Poll[KEY_INPUT_J] >= 1)
        {
            pRot.y -= ROTATE_SPEED;
            cRot.y = pRot.y;
        }
        if(Key.Poll[KEY_INPUT_K] >= 1)
        {
            pRot.y += ROTATE_SPEED; 
            cRot.y = pRot.y;
        }
        if(Mouse.Poll[MOUSE_INPUT_RIGHT] == 1)
        {
            if(CameraLock == TRUE) CameraLock = FALSE;
            else CameraLock = TRUE, pRot.y = cRot.y;
        }
        if(Mouse.Moved() && CameraLock == FALSE && Screen.Cursor == FALSE)
        {
            cRot.y -= (ROTATE_SPEED*Mouse.GetDeltaX())/30;

            vOffset += (ROTATE_SPEED*Mouse.GetDeltaY())/80;
            if(vOffset > 0.81f) vOffset = 0.81f;
            if(vOffset < -0.41f) vOffset = -0.41f;

            if((Mouse.x > Screen.Width * 0.8f  || Mouse.x < 0 + Screen.Width * 0.2f)
            || (Mouse.y > Screen.Height * 0.8f || Mouse.y < 0 + Screen.Height * 0.2f))
            {
                Mouse.Reset(Screen.Width/2,Screen.Height/2);
            }
            
        }
        else if(Mouse.Moved() && CameraLock == TRUE && Screen.Cursor == FALSE)
        {
            pRot.y -= (ROTATE_SPEED*Mouse.GetDeltaX())/30;
            cRot.y = pRot.y;

            vOffset += (ROTATE_SPEED*Mouse.GetDeltaY())/80;
            if(vOffset > 0.81f) vOffset = 0.81f;
            if(vOffset < -0.41f) vOffset = -0.41f;

            if((Mouse.x > Screen.Width * 0.8f  || Mouse.x < 0 + Screen.Width * 0.2f)
            || (Mouse.y > Screen.Height * 0.8f || Mouse.y < 0 + Screen.Height * 0.2f))
            {
                Mouse.Reset(Screen.Width/2,Screen.Height/2);
            }
            
        }

        //Pick Target
        if(Key.Poll[KEY_INPUT_LSHIFT] == 1) Selected = -1; //deselect
        if(Key.Poll[KEY_INPUT_TAB] == 1 && Destroyed != SPHERES)
        {
            for(uint_fast8_t i = 0; i < SPHERES; i++)
            {
                Selected = Selected + 1;
                if(sphere[Selected].active) break;
            }
            TargetLock = TRUE;
            if(Selected > (SPHERES - Destroyed) - 1) Selected = -1;
        }
        if(Selected == -1) TargetLock = FALSE;

        //Handle TargetLock
        if(TargetLock == TRUE)
        {
            if(sphere[Selected].active)
            {
                S.x = Player.x - sphere[Selected].v.x;
                S.z = Player.z - sphere[Selected].v.z;
                angleH = atan2f(S.x,S.z); 
                pRot.y = angleH;
                cRot.y = pRot.y - DX_PI_F/10;;
            }
            else Selected = -1, TargetLock = FALSE;
        }

        Camera.z = Player.z + cos(cRot.y)*40;
        Camera.x = Player.x + sin(cRot.y)*40;
        
        //horizontal lock formula
        S.x = Player.x - Camera.x;
        S.y = Player.y - Camera.y; 
        S.z = Player.z - Camera.z;
        angleH = atan2f(S.x,S.z); 

        //vertical lock formula
        S.x = Camera.x - Player.x;
        S.y = Camera.y - Player.y; 
        S.z = Camera.z - Player.z;
        angleV = (atan2f(S.y, sqrtf(S.x*S.x + S.z*S.z)) - vOffset);
    }
    
    //Simple Jump
    if(pJump == TRUE && pGrounded == FALSE)
    {
        Player.y = anchorY + sin(DX_PI_F*2/90*pPace)*17;
        pPace++, pRot.x = DX_PI_F*2;
        if(Player.y <= 0.3f && pPace > 4) pJump = FALSE, pGrounded = TRUE, Player.y = 0.0f;
    }

    //3DAnimation Handler
    {
        //Check&Update
        if(AnimSet != pIndex)
        {
            MV1DetachAnim(ModelH,AttachIndex);
            AttachIndex = MV1AttachAnim(ModelH, pIndex, -1, FALSE);
            TotalTime = MV1GetAttachAnimTotalTime(ModelH,AttachIndex);
            PlayTime = 0.0f;
            AnimSet = pIndex; 
        }

        //Advance frame time
        if(pReverse == TRUE)
        {
            PlayTime -= 0.31f;
            if(PlayTime <= 0) PlayTime = TotalTime;
        }
        else
        {
            PlayTime += 0.39f;
            if(PlayTime >= TotalTime) PlayTime = 0.0f;
        }

        //Update active frame
        MV1SetAttachAnimTime(ModelH,AttachIndex,PlayTime); 
    }

    //Update ModelH position/pRotation
    MV1SetPosition(ModelH,VGet(Player.x,Player.y,Player.z));
    MV1SetRotationXYZ(ModelH,VGet(pRot.x,(pRot.y+pOffset.y),pRot.z));

    //Update Camera
    SetCameraPositionAndAngle(Camera,angleV,angleH,0.0f);

    //Collision
    MV1RefreshCollInfo(ModelH,-1);
    for(uint_fast8_t i = 0; i < SPHERES; i++)
    {
        HitPolyDim[i] = MV1CollCheck_Sphere(ModelH, -1, sphere[i].v, sphere[i].r);
        if(HitPolyDim[i].HitNum >= 1)
        {
            SDFlag[i] = 0;
            sphere[i].active = false;
        }
        MV1CollResultPolyDimTerminate(HitPolyDim[i]);
    }
    
}

void PracticeScene::Draw()
{
    for(uint_fast8_t i = 0; i < SPHERES; i++)
    {
        if(SDFlag[i] == 1)
        {
            DrawSphere3D(sphere[i].v, sphere[i].r, 200, GetColor(200,0,120), GetColor(100,0,60), TRUE);
        }
    }
    SetGlobalAmbientLight(GetColorF(0.0f,0.2f,0.0f,0.0f));
    DrawCone3D(VGet(-10.0f,-30.0f,10.0f),VGet(-10.0f,-40.0f,10.0f),1000.0f,32,GetColor(40,30,70),GetColor(90,150,120),TRUE);
    MV1SetAttachAnimBlendRate(ModelH,AttachIndex,0.9f);
    if(TargetLock == TRUE)
    {
        if(flux > 2.00f) fluxReverse = TRUE;
        if(flux <= 0.00f) fluxReverse = FALSE;
        if(fluxReverse == FALSE) flux += 0.03f;
        else flux -= 0.03f;
        markerSize = ((Dot2(Player.x,Player.z)-Dot2(sphere[Selected].v.x,sphere[Selected].v.z))*.00005f)+1.6f;
        DrawBillboard3D(VGet(sphere[Selected].v.x,(sphere[Selected].v.y+16.0f+flux),sphere[Selected].v.z),1.0f,1.0f,markerSize,0.0f,TargetH,TRUE);
    }
    MV1DrawModel(ModelH);
    Screen.DrawCursor(Screen.CursorH);
    if(debugflag)
    {
        DrawFormatString(0,20,GetColor(255,255,255),"x=%.1f y=%.1f z=%.1f",Player.x,Player.y,Player.z);
        DrawFormatString(0,40,GetColor(255,255,255),"angleV=%.2f, angleH=%.2f",angleV, angleH);
        DrawFormatString(0,60,-256,"delta_x=%.2f, mouse-x=%d",Mouse.GetDeltaX(),Mouse.x);
        DrawFormatString(0,80,-1,"Target=%d",Selected);
    }   
}

DxLib::VECTOR SetCross(const VECTOR &a, const VECTOR &b)
{
	VECTOR c;
	c.x = (a.y * b.z - a.z * b.y);
	c.y = (a.z * b.x - a.x * b.z);
	c.z = (a.x * b.y - a.y * b.x);
	return c;
}

float Dot3(const VECTOR &a, const VECTOR &b)
{
	float dot;
	dot = (a.x * b.x + a.y * b.y + a.z * b.z);
	return dot;
}

float Dot2(const float &x, const float &z)
{
	float dot;
	dot = (x*x+z*z);
	return dot;
}