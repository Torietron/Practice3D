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
static int_fast8_t Selected = -1; //-1 = no target
static uint_least8_t CameraLock = TRUE, TargetLock = FALSE;

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
    Marker.Size = 0.0f;
    Marker.Angle = 0.0f;
}

void PlayerData::Load()
{
    if(MMD.ModelH != 0) //if already set
    {
        MV1DetachAnim(MMD.ModelH,MMD.AttachIndex);
        MV1DeleteModel(MMD.ModelH);
    }
    MMD.ModelH = MV1LoadModel(_T("dat/Lat/LatMikuVer2.3_SailorWinter.pmd"));
    MMD.IdleIndex = MV1AttachAnim(MMD.ModelH, 0, -1, FALSE);
    MMD.AttachIndex = MV1AttachAnim(MMD.ModelH, 0, -1, FALSE);
    MMD.TotalTime = MV1GetAttachAnimTotalTime(MMD.ModelH,MMD.AttachIndex);
    MMD.BlendDecay = 0.0f;
    MMD.AutoBlend = TRUE;
    MV1SetupCollInfo(MMD.ModelH, -1, 1, 1, 1);
    MMD.Pace = 0;

    Marker.SpriteH = LoadGraph(_T("core/ph3.png"));
}

void PlayerData::Update(Sphere_t *sObj, int_fast16_t Destroyed, const int_fast16_t MAX)
{
    if(Key.Poll[KEY_INPUT_Q] >= 1) MMD.Rot.y -= ROTATE_SPEED;
    if(Key.Poll[KEY_INPUT_E] >= 1) MMD.Rot.y += ROTATE_SPEED;
    if(Key.Poll[KEY_INPUT_A] >= 1)
    {
        MMD.Body.Pos.z -= cos(MMD.Rot.y-(DX_PI_F/2))*MOVEMENT_SPEED;
        MMD.Body.Pos.x -= sin(MMD.Rot.y-(DX_PI_F/2))*MOVEMENT_SPEED;
        MMD.Rot.x = DX_PI_F*2;
        if(Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = (DX_PI_F/2) * -1;
    }
    if(Key.Poll[KEY_INPUT_D] >= 1)
    {
        MMD.Body.Pos.z -= cos(MMD.Rot.y+(DX_PI_F/2))*MOVEMENT_SPEED;
        MMD.Body.Pos.x -= sin(MMD.Rot.y+(DX_PI_F/2))*MOVEMENT_SPEED;
        MMD.Rot.x = DX_PI_F*2;
        if(Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = DX_PI_F/2;
    }
    if(Key.Poll[KEY_INPUT_W] >= 1) 
    {
        MMD.Body.Pos.z -= cos(MMD.Rot.y)*MOVEMENT_SPEED;
        MMD.Body.Pos.x -= sin(MMD.Rot.y)*MOVEMENT_SPEED;
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
        MMD.Body.Pos.z += cos(MMD.Rot.y)*(MOVEMENT_SPEED*0.75f);
        MMD.Body.Pos.x += sin(MMD.Rot.y)*(MOVEMENT_SPEED*0.75f);
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
            anchor_y = MMD.Body.Pos.y;
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
            Screen.C3D.AngleH = Physics.Formula.RelAngle2(MMD.Body.Pos, sObj[Selected].Pos);
            MMD.Rot.y = Screen.C3D.AngleH;
            Screen.C3D.Anchor = MMD.Rot.y - DX_PI_F/10;
            Ui.UpdateFlux();
        }
        else Selected = -1, TargetLock = FALSE;
    }

    Screen.C3D.Pos.z = MMD.Body.Pos.z + cos(Screen.C3D.Anchor)*40;
    Screen.C3D.Pos.x = MMD.Body.Pos.x + sin(Screen.C3D.Anchor)*40;
    
    //horizontal lock formula
    Screen.C3D.AngleH = Physics.Formula.RelAngle2(MMD.Body.Pos, Screen.C3D.Pos);

    //vertical lock formula
    Screen.C3D.AngleV = Physics.Formula.RelAngle3(Screen.C3D.Pos, MMD.Body.Pos) - Screen.C3D.OffsetV;
    
    //Simple Jump
    if(Jump == TRUE && Grounded == FALSE)
    {
        MMD.Body.Pos.y = anchor_y + sin(DX_PI_F*2/90*MMD.Pace)*17;
        MMD.Pace++, MMD.Rot.x = DX_PI_F*2;
        if(MMD.Body.Pos.y <= 0.3f && MMD.Pace > 4) Jump = FALSE, Grounded = TRUE, MMD.Body.Pos.y = 0.0f;
    }
    
    Model.Update(MMD);
}

void PlayerData::Draw(Sphere_t *sObj)
{
    if(TargetLock == TRUE)
    {
        Marker.Size = ((Physics.Formula.Dot2(MMD.Body.Pos.x,MMD.Body.Pos.z)-Physics.Formula.Dot2(sObj[Selected].Pos.x, sObj[Selected].Pos.z))*.00005f)+1.6f;
        Ui.DrawMarker3D(Marker, sObj[Selected].Pos, (16.0f + Ui.Flux));
    }

    Model.Draw(MMD);
}