#include "DxLib.h"
#include <cmath>
#include <cstdint>
#include "ScreenControl.h"
#include "MousePoll.h"
#include "KeyPoll.h"
#include "ModelData.h"
#include "Interface.h"
#include "PhysicsData.h"
#include "PlayerData.h"
#include "EnemyData.h"

static const float ROTATE_SPEED = DX_PI_F/45;
static const float MOVEMENT_SPEED = DX_PI_F/5;
static VECTOR S;

int_fast8_t Selected = -1; //-1 = no target
uint_least8_t CameraLock = TRUE, TargetLock = FALSE;
int MarkerH;
float markerSize = 0.0f;

extern ScreenControl Screen;
extern MousePoll Mouse;
extern KeyPoll Key;
extern ModelData Model;
extern PhysicsData Physics;
extern Interface Ui;

PlayerData::PlayerData()
{
    Jump = FALSE, Grounded = TRUE;
    MMD.AttachIndex = 0;
    MMD.AnimSet = 0;
    MMD.AnimIndex = 0;
    MMD.Pace = 0;
    MMD.PlayTime = 0.0f;
    MMD.PlayOffset = 0.0f;
    MMD.Reverse = false, MMD.Event = false;
}

void PlayerData::Load()
{
    if(MMD.ModelH != 0) //if already set
    {
        MV1DetachAnim(MMD.ModelH,MMD.AttachIndex);
        MV1DeleteModel(MMD.ModelH);
    }
    MMD.ModelH = MV1LoadModel(_T("dat/Lat/LatMikuVer2.3_SailorWinter.pmd"));
    MMD.AttachIndex = MV1AttachAnim(MMD.ModelH, 0, -1, FALSE);
    MMD.TotalTime = MV1GetAttachAnimTotalTime(MMD.ModelH,MMD.AttachIndex);
    MV1SetupCollInfo(MMD.ModelH, -1, 1, 1, 1);
    MMD.Pace = 0;

    MarkerH = LoadGraph(_T("core/ph3.png"));
}

void PlayerData::Update(Sphere_t *sObj, int_fast16_t Destroyed, const int_fast16_t MAX)
{
    if(Key.Poll[KEY_INPUT_Q] >= 1) MMD.Rot.y -= ROTATE_SPEED;
    if(Key.Poll[KEY_INPUT_E] >= 1) MMD.Rot.y += ROTATE_SPEED;
    if(Key.Poll[KEY_INPUT_A] >= 1)
    {
        MMD.Pos.z -= cos(MMD.Rot.y-(DX_PI_F/2))*MOVEMENT_SPEED;
        MMD.Pos.x -= sin(MMD.Rot.y-(DX_PI_F/2))*MOVEMENT_SPEED;
        MMD.Rot.x = DX_PI_F*2;
        if(Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = (DX_PI_F/2) * -1;
    }
    if(Key.Poll[KEY_INPUT_D] >= 1)
    {
        MMD.Pos.z -= cos(MMD.Rot.y+(DX_PI_F/2))*MOVEMENT_SPEED;
        MMD.Pos.x -= sin(MMD.Rot.y+(DX_PI_F/2))*MOVEMENT_SPEED;
        MMD.Rot.x = DX_PI_F*2;
        if(Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = DX_PI_F/2;
    }
    if(Key.Poll[KEY_INPUT_W] >= 1) 
    {
        MMD.Pos.z -= cos(MMD.Rot.y)*MOVEMENT_SPEED;
        MMD.Pos.x -= sin(MMD.Rot.y)*MOVEMENT_SPEED;
        MMD.Rot.x = DX_PI_F*2;
        if(Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = 0;
        if(Key.Poll[KEY_INPUT_A] >= 1) MMD.RotOffset.y = (DX_PI_F/4) * -1;
        if(Key.Poll[KEY_INPUT_D] >= 1) MMD.RotOffset.y = DX_PI_F/4;
    }
    if(Key.Poll[KEY_INPUT_S] >= 1) 
    {
        MMD.Pos.z += cos(MMD.Rot.y)*(MOVEMENT_SPEED*0.75f);
        MMD.Pos.x += sin(MMD.Rot.y)*(MOVEMENT_SPEED*0.75f);
        MMD.Rot.x = DX_PI_F/10; //Center of gravity would've been visually inconsistent
        if(Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = TRUE;
        MMD.RotOffset.y = 0;
        if(Key.Poll[KEY_INPUT_A] >= 1) MMD.RotOffset.y = DX_PI_F/7;
        if(Key.Poll[KEY_INPUT_D] >= 1) MMD.RotOffset.y = (DX_PI_F/7)* -1;
    }
    if(Key.Poll[KEY_INPUT_SPACE] == 1)
    {
        if(Grounded == TRUE) 
        {
            anchor_y = MMD.Pos.y;
            MMD.Pace = 0;
            Jump = TRUE;
            Grounded = FALSE;
            MMD.Rot.x = DX_PI_F*2;
            MMD.AnimIndex = 2;
        }
    }
    if(Key.Poll[KEY_INPUT_W] == 0 && Key.Poll[KEY_INPUT_S] == 0
    && Key.Poll[KEY_INPUT_A] == 0 && Key.Poll[KEY_INPUT_D] == 0)
    {
        if(Grounded == TRUE) MMD.AnimIndex = 0, MMD.Rot.x = DX_PI_F*2, MMD.Reverse = FALSE;
        if(Grounded == FALSE) MMD.AnimIndex = 2, MMD.Rot.x = DX_PI_F*2, MMD.Reverse = FALSE;
        MMD.RotOffset.y = 0.0f;
    }

    //Camera
    //keep the camera a set distance from player
    if(Key.Poll[KEY_INPUT_J] >= 1)
    {
        MMD.Rot.y -= ROTATE_SPEED;
        Screen.C3D.Anchor = MMD.Rot.y;
    }
    if(Key.Poll[KEY_INPUT_K] >= 1)
    {
        MMD.Rot.y += ROTATE_SPEED; 
        Screen.C3D.Anchor = MMD.Rot.y;
    }
    if(Mouse.Poll[MOUSE_INPUT_RIGHT] == 1)
    {
        if(CameraLock == TRUE) CameraLock = FALSE;
        else CameraLock = TRUE, MMD.Rot.y = Screen.C3D.Anchor;
    }
    if(Mouse.Moved() && CameraLock == FALSE && Screen.Cursor == FALSE)
    {
        Screen.C3D.Anchor -= (ROTATE_SPEED*Mouse.GetDeltaX())/30;

        Screen.C3D.OffsetV += (ROTATE_SPEED*Mouse.GetDeltaY())/80;
        if(Screen.C3D.OffsetV > 0.81f) Screen.C3D.OffsetV = 0.81f;
        if(Screen.C3D.OffsetV < -0.41f) Screen.C3D.OffsetV = -0.41f;

        if((Mouse.x > Screen.Width * 0.8f  || Mouse.x < 0 + Screen.Width * 0.2f)
        || (Mouse.y > Screen.Height * 0.8f || Mouse.y < 0 + Screen.Height * 0.2f))
        {
            Mouse.Reset(Screen.Width/2,Screen.Height/2);
        }
        
    }
    else if(Mouse.Moved() && CameraLock == TRUE && Screen.Cursor == FALSE)
    {
        MMD.Rot.y -= (ROTATE_SPEED*Mouse.GetDeltaX())/30;
        Screen.C3D.Anchor = MMD.Rot.y;

        Screen.C3D.OffsetV += (ROTATE_SPEED*Mouse.GetDeltaY())/80;
        if(Screen.C3D.OffsetV > 0.81f) Screen.C3D.OffsetV = 0.81f;
        if(Screen.C3D.OffsetV < -0.41f) Screen.C3D.OffsetV = -0.41f;

        if((Mouse.x > Screen.Width * 0.8f  || Mouse.x < 0 + Screen.Width * 0.2f)
        || (Mouse.y > Screen.Height * 0.8f || Mouse.y < 0 + Screen.Height * 0.2f))
        {
            Mouse.Reset(Screen.Width/2,Screen.Height/2);
        }
    }

    //Pick Target
    if(Key.Poll[KEY_INPUT_LSHIFT] == 1) Selected = -1; //deselect
    if(Key.Poll[KEY_INPUT_TAB] == 1 && Destroyed != MAX)
    {
        for(uint_fast8_t i = 0; i < MAX; i++)
        {
            Selected = Selected + 1;
            if(sObj[Selected].Active) break;
        }
        TargetLock = TRUE;
        if(Selected > (MAX - Destroyed) - 1) Selected = -1;
    }
    if(Selected == -1) TargetLock = FALSE;

    //Handle TargetLock
    if(TargetLock == TRUE)
    {
        if(sObj[Selected].Active)
        {
            S.x = MMD.Pos.x - sObj[Selected].Pos.x;
            S.z = MMD.Pos.z - sObj[Selected].Pos.z;
            Screen.C3D.AngleH = atan2f(S.x,S.z); 
            MMD.Rot.y = Screen.C3D.AngleH;
            Screen.C3D.Anchor = MMD.Rot.y - DX_PI_F/10;;
        }
        else Selected = -1, TargetLock = FALSE;
    }

    Screen.C3D.Pos.z = MMD.Pos.z + cos(Screen.C3D.Anchor)*40;
    Screen.C3D.Pos.x = MMD.Pos.x + sin(Screen.C3D.Anchor)*40;
    
    //horizontal lock formula
    S.x = MMD.Pos.x - Screen.C3D.Pos.x;
    S.z = MMD.Pos.z - Screen.C3D.Pos.z;
    Screen.C3D.AngleH = atan2f(S.x,S.z); 

    //vertical lock formula
    S.x = Screen.C3D.Pos.x - MMD.Pos.x;
    S.y = Screen.C3D.Pos.y - MMD.Pos.y; 
    S.z = Screen.C3D.Pos.z - MMD.Pos.z;
    Screen.C3D.AngleV = (atan2f(S.y, sqrtf(S.x*S.x + S.z*S.z)) - Screen.C3D.OffsetV);
    
    //Simple Jump
    if(Jump == TRUE && Grounded == FALSE)
    {
        MMD.Pos.y = anchor_y + sin(DX_PI_F*2/90*MMD.Pace)*17;
        MMD.Pace++, MMD.Rot.x = DX_PI_F*2;
        if(MMD.Pos.y <= 0.3f && MMD.Pace > 4) Jump = FALSE, Grounded = TRUE, MMD.Pos.y = 0.0f;
    }
    
    Model.Update(MMD);
}

void PlayerData::Draw(Sphere_t *sObj)
{
    if(TargetLock == TRUE)
    {
        markerSize = ((Physics.Formula.Dot2(MMD.Pos.x,MMD.Pos.z)-Physics.Formula.Dot2(sObj[Selected].Pos.x,sObj[Selected].Pos.z))*.00005f)+1.6f;
        Ui.DrawMarker3D(sObj[Selected].Pos,markerSize,MarkerH);
    }

    Model.Draw(MMD);
}