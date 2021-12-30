#include "DxLib.h"
#include <cmath>
#include "SceneControl.h"
#include "ScreenControl.h"
#include "PracticeScene.h"
#include "KeyPoll.h"
#include "MousePoll.h"
#include "ModelData.h"
#include "PlayerData.h"
#include "EnemyData.h"

#define SPHERES 2

static uint_fast8_t Destroyed = 0, fluxReverse = FALSE; 
static uint_fast8_t SDFlag[SPHERES] = {0};
static int TargetH, Light;
static float cZoom = 0.0f, flux = 0.0f; 
static float targetX = 0.0f, targetY = 10.0f, targetZ = 0.0f, markerSize = 0.0f;

MV1_COLL_RESULT_POLY_DIM HitPolyDim[SPHERES];
Sphere_t sphere[SPHERES];

extern bool debugflag;
extern ScreenControl Screen;
extern MousePoll Mouse;
extern KeyPoll Key;
extern SceneControl Scene;
extern ModelData Models;
extern PlayerData Player;
extern EnemyData Enemy;

//temp
extern int_fast8_t Selected;
extern uint_fast8_t TargetLock;

DxLib::VECTOR SetCross(const VECTOR &a, const VECTOR &b);
float Dot3(const VECTOR &a, const VECTOR &b);
float Dot2(const float &x, const float &z);

void PracticeScene::Init()
{
    Screen.C3D.Pos = VGet(0.0f, 20.0f, -20.0f);
    Player.MMD.Pos = VGet(42.0f, 0.0f, 52.0f);
    Player.MMD.Rot = VGet(0.0f, (DX_PI_F/5) * -1, 0.0f);
    Screen.C3D.Anchor = ((DX_PI_F/5) * -1);

    SDFlag[0] = 1, SDFlag[1] = 1;
    sphere[0].v = VGet(-0.5f,10.0f,-46.0f), sphere[0].r = 6.0f, sphere[0].active = true;
    sphere[1].v = VGet(-40.0f,10.0f,-60.0f), sphere[1].r = 6.0f, sphere[1].active = true;
}

void PracticeScene::Load()
{
    if(Player.MMD.ModelH != 0) //if already set
    {
        MV1DetachAnim(Player.MMD.ModelH,Player.MMD.AttachIndex);
        MV1DeleteModel(Player.MMD.ModelH);
    }
    SetCameraNearFar(0.1f, 1000.0f);
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    TargetH = LoadGraph(_T("core/ph3.png"));
    Light = CreatePointLightHandle(VGet(0.0f,3000.0f,0.0f),3000.0f,0.2f,0.002f,0.0f);
    Screen.CursorH[0] = LoadGraph(_T("core/ph7.png"));
    SetLightPositionHandle(Light,VGet(0.0f,500.0f,0.0f));
    Player.MMD.ModelH = MV1LoadModel(_T("dat/Lat/LatMikuVer2.3_SailorWinter.pmd"));
    Player.MMD.AttachIndex = MV1AttachAnim(Player.MMD.ModelH, 0, -1, FALSE);
    Player.MMD.TotalTime = MV1GetAttachAnimTotalTime(Player.MMD.ModelH,Player.MMD.AttachIndex);
    MV1SetupCollInfo(Player.MMD.ModelH, -1, 1, 1, 1);
    Player.MMD.Pace = 0;
}

void PracticeScene::End()
{
    MV1DetachAnim(Player.MMD.ModelH,Player.MMD.AttachIndex);
    MV1DeleteModel(Player.MMD.ModelH);
}

void PracticeScene::Update()
{
    Key.Update();
    Mouse.Update();
    Player.Update(sphere,Destroyed,SPHERES);
    Models.Update(Player.MMD);

    //Update Camera
    Screen.C3D.Apply();

    //Collision
    MV1RefreshCollInfo(Player.MMD.ModelH,-1);
    for(uint_fast8_t i = 0; i < SPHERES; i++)
    {
        HitPolyDim[i] = MV1CollCheck_Sphere(Player.MMD.ModelH, -1, sphere[i].v, sphere[i].r);
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
    MV1SetAttachAnimBlendRate(Player.MMD.ModelH,Player.MMD.AttachIndex,0.9f);
    if(TargetLock == TRUE)
    {
        if(flux > 2.00f) fluxReverse = TRUE;
        if(flux <= 0.00f) fluxReverse = FALSE;
        if(fluxReverse == FALSE) flux += 0.03f;
        else flux -= 0.03f;
        markerSize = ((Dot2(Player.MMD.Pos.x,Player.MMD.Pos.z)-Dot2(sphere[Selected].v.x,sphere[Selected].v.z))*.00005f)+1.6f;
        DrawBillboard3D(VGet(sphere[Selected].v.x,(sphere[Selected].v.y+16.0f+flux),sphere[Selected].v.z),1.0f,1.0f,markerSize,0.0f,TargetH,TRUE);
    }
    MV1DrawModel(Player.MMD.ModelH);
    if(debugflag)
    {
        DrawFormatString(0,20,GetColor(255,255,255),"x=%.1f y=%.1f z=%.1f",Player.MMD.Pos.x,Player.MMD.Pos.y,Player.MMD.Pos.z);
        DrawFormatString(0,40,GetColor(255,255,255),"angleV=%.2f, angleH=%.2f",Screen.C3D.AngleV, Screen.C3D.AngleH);
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