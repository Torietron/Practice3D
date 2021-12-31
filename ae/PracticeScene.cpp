#include "DxLib.h"
#include <cmath>
#include "SceneControl.h"
#include "ScreenControl.h"
#include "PracticeScene.h"
#include "KeyPoll.h"
#include "MousePoll.h"
#include "ModelData.h"
#include "PhysicsData.h"
#include "PlayerData.h"
#include "EnemyData.h"

#define SPHERES 2

static uint_fast16_t Destroyed = 0;
static uint_fast16_t SDFlag[SPHERES] = {0};
static int Light;

static MV1_COLL_RESULT_POLY_DIM HitPolyDim[SPHERES];
static Sphere_t Sphere[SPHERES];

extern bool debugflag;
extern ScreenControl Screen;
extern SceneControl Scene;
extern MousePoll Mouse;
extern KeyPoll Key;
extern ModelData Model;
extern PhysicsData Physics;
extern PlayerData Player;
extern EnemyData Enemy;

void PracticeScene::Init()
{
    Screen.C3D.Pos = VGet(0.0f, 20.0f, -20.0f);
    Player.MMD.Pos = VGet(42.0f, 0.0f, 52.0f);
    Player.MMD.Rot = VGet(0.0f, (DX_PI_F/5) * -1, 0.0f);
    Screen.C3D.Anchor = ((DX_PI_F/5) * -1);

    SDFlag[0] = 1, SDFlag[1] = 1;
    Sphere[0].v = VGet(-0.5f,10.0f,-46.0f), Sphere[0].r = 6.0f, Sphere[0].active = true;
    Sphere[1].v = VGet(-40.0f,10.0f,-60.0f), Sphere[1].r = 6.0f, Sphere[1].active = true;
}

void PracticeScene::Load()
{
    Light = CreatePointLightHandle(VGet(0.0f,3000.0f,0.0f),3000.0f,0.2f,0.002f,0.0f);
    SetLightPositionHandle(Light,VGet(0.0f,500.0f,0.0f));

    Screen.CursorH[0] = LoadGraph(_T("core/ph7.png"));

    Player.Load();
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
    Player.Update(Sphere,Destroyed,SPHERES);
    Model.Update(Player.MMD);

    //Update Camera
    Screen.C3D.Apply();

    //Collision
    for(uint_fast8_t i = 0; i < SPHERES; i++)
    {
        if(Physics.SphereCollision3D(Player.MMD.ModelH,Sphere[i].v,Sphere[i].r,HitPolyDim[i]))
        {
            SDFlag[i] = 0;
            Sphere[i].active = false;
        }
    }
}

void PracticeScene::Draw()
{
    Enemy.Draw(Sphere, SPHERES, SDFlag);

    SetGlobalAmbientLight(GetColorF(0.0f,0.2f,0.0f,0.0f));
    DrawCone3D(VGet(-10.0f,-30.0f,10.0f),VGet(-10.0f,-40.0f,10.0f),1000.0f,32,GetColor(40,30,70),GetColor(90,150,120),TRUE);

    Player.Draw(Sphere);
    
    if(debugflag)
    {
        DrawFormatString(0,20,GetColor(255,255,255),"x=%.1f y=%.1f z=%.1f",Player.MMD.Pos.x,Player.MMD.Pos.y,Player.MMD.Pos.z);
        DrawFormatString(0,40,GetColor(255,255,255),"angleV=%.2f, angleH=%.2f",Screen.C3D.AngleV, Screen.C3D.AngleH);
        DrawFormatString(0,60,-256,"delta_x=%.2f, mouse-x=%d",Mouse.GetDeltaX(),Mouse.x);
        //DrawFormatString(0,80,-1,"Target=%d",Selected);
    }   
}