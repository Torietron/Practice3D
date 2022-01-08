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

#define MAXSPELLS 200

static const float ROTATE_SPEED = DX_PI_F/45;
static const float MOVEMENT_SPEED = DX_PI_F/5;
static uint_least8_t CameraLock = TRUE, TargetLock = FALSE;

static uint_fast8_t SpellDFlag[MAXSPELLS] = {0};
static Spell_t SpellObj[MAXSPELLS];

extern ScreenControl Screen;
extern MousePoll Mouse;
extern KeyPoll Key;
extern ModelData Model;
extern PhysicsData Physics;
extern Interface Ui;

PlayerData::PlayerData()
{
    Pos = &MMD.Body.Pos;
    Rot = &MMD.Body.Rot;
    Selected = -1; //-1 = no target
    isCasting = FALSE;
    Jump = FALSE, MMD.Body.Grounded = TRUE;
    MMD.Body.Enable3D = TRUE;
    MMD.AttachIndex = 0;
    MMD.AnimSet = 0;
    MMD.AnimIndex = 0;
    MMD.Pace = 0;
    MMD.PlayTime = 0.0f;
    MMD.PlayOffset = 0.0f;
    MMD.Reverse = FALSE, MMD.Event = FALSE;
    Marker.Size = 0.0f;
    Marker.Angle = 0.0f;
    MMD.Body.VelBase = 1.5f, MMD.Body.VelMax = 5.0f;
    MMD.Body.AccelRate = 0.35f; MMD.Body.TermVel = 3.0f;
    MMD.Body.MassRatio = 0.8f, MMD.Body.Interval = 50;
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

void PlayerData::Update(const Sphere_t *sObj, int_fast16_t Destroyed, const int_fast16_t MAX)
{
    
    if(Key.Poll[KEY_INPUT_Q] >= 1) Rot->y -= ROTATE_SPEED;
    if(Key.Poll[KEY_INPUT_E] >= 1) Rot->y += ROTATE_SPEED;
    if(Key.Poll[KEY_INPUT_A] >= 1)
    {
        Physics.Propel(MMD.Body,MOVEMENT_SPEED,-DX_PI_F/2);
        MMD.RotOffset.x = DX_PI_F*2;
        if(MMD.Body.Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = (DX_PI_F/2) * -1;
    }
    if(Key.Poll[KEY_INPUT_D] >= 1)
    {
        Physics.Propel(MMD.Body,MOVEMENT_SPEED,+DX_PI_F/2);
        MMD.RotOffset.x = DX_PI_F*2;
        if(MMD.Body.Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = DX_PI_F/2;
    }
    
    if(Key.Poll[KEY_INPUT_W] >= 1) 
    {
        Physics.Propel(MMD.Body,MOVEMENT_SPEED);
        MMD.RotOffset.x = DX_PI_F*2;
        if(MMD.Body.Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = 0;
        if(Key.Poll[KEY_INPUT_A] >= 1) MMD.RotOffset.y = (DX_PI_F/4) * -1;
        if(Key.Poll[KEY_INPUT_D] >= 1) MMD.RotOffset.y = DX_PI_F/4;
    }
    
    if(Key.Poll[KEY_INPUT_S] >= 1) 
    {
        Physics.Propel(MMD.Body,MOVEMENT_SPEED*0.75f,DX_PI_F);
        MMD.RotOffset.x = DX_PI_F/10; //Center of gravity would've been visually inconsistent
        if(MMD.Body.Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = TRUE;
        MMD.RotOffset.y = 0;
        if(Key.Poll[KEY_INPUT_A] >= 1) MMD.RotOffset.y = DX_PI_F/7;
        if(Key.Poll[KEY_INPUT_D] >= 1) MMD.RotOffset.y = (DX_PI_F/7)* -1;
    }
    
    if(Key.Poll[KEY_INPUT_SPACE] == 1)
    {
        if(MMD.Body.Grounded == TRUE) 
        {   
            anchor_y = Pos->y;
            MMD.Pace = 0;
            Jump = TRUE;
            MMD.Body.Grounded = FALSE;
            Rot->x = DX_PI_F*2;
            MMD.AnimIndex = 2;
            Physics.Accelerate(MMD.Body,AXIS_Y);
            MMD.Body.Accel = 0.0f;
        }
    }
    if(Key.Poll[KEY_INPUT_W] == 0 && Key.Poll[KEY_INPUT_S] == 0
    && Key.Poll[KEY_INPUT_A] == 0 && Key.Poll[KEY_INPUT_D] == 0)
    {
        if(MMD.Body.Grounded == TRUE) MMD.AnimIndex = 0, MMD.RotOffset.x = DX_PI_F*2, MMD.Reverse = FALSE;
        if(MMD.Body.Grounded == FALSE) MMD.AnimIndex = 2, MMD.RotOffset.x = DX_PI_F*2, MMD.Reverse = FALSE;
        MMD.RotOffset.y = 0.0f;
    }

    //Camera
    //keep the camera a set distance from player
    if(Key.Poll[KEY_INPUT_J] >= 1)
    {
        Rot->y -= ROTATE_SPEED;
        Screen.C3D.Anchor = Rot->y;
    }
    if(Key.Poll[KEY_INPUT_K] >= 1)
    {
        Rot->y += ROTATE_SPEED; 
        Screen.C3D.Anchor = Rot->y;
    }
    if(Mouse.Poll[MOUSE_INPUT_RIGHT] == 1)
    {
        if(CameraLock == TRUE) CameraLock = FALSE;
        else CameraLock = TRUE, Rot->y = Screen.C3D.Anchor;
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
        Rot->y -= (ROTATE_SPEED*Mouse.GetDeltaX())/30;
        Screen.C3D.Anchor = Rot->y;

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
            Rot->y = Screen.C3D.AngleH;
            Screen.C3D.Anchor = Rot->y - DX_PI_F/10;
            Ui.UpdateFlux();
        }
        else Selected = -1, TargetLock = FALSE;
    }

    Screen.C3D.Pos.z = Pos->z + cos(Screen.C3D.Anchor)*40;
    Screen.C3D.Pos.x = Pos->x + sin(Screen.C3D.Anchor)*40;
    
    //horizontal lock formula
    Screen.C3D.AngleH = Physics.Formula.RelAngle2(MMD.Body.Pos, Screen.C3D.Pos);

    //vertical lock formula
    Screen.C3D.AngleV = Physics.Formula.RelAngle3(Screen.C3D.Pos, MMD.Body.Pos) - Screen.C3D.OffsetV;
    
    //Handle Jump
    if(Jump == TRUE && MMD.Body.Grounded == FALSE)
    {
        Physics.Manipulate(MMD.Body);
        MMD.RotOffset.x = DX_PI_F*2;
        if(Physics.GetLastValue(LAST_GRAVITY_Y) == 0) Jump = FALSE, MMD.Body.Grounded = TRUE;
    }

    if(Key.Poll[KEY_INPUT_1] == 1 && TargetLock == TRUE) CreateSpell(sObj[Selected]);
    UpdateSpells();
    Model.Update(MMD);
}

//I will have to make models later
void PlayerData::CreateSpell(const Sphere_t &sObj)
{
    uint_fast8_t count = 0;
    for(uint_fast8_t i = 0; i < MAXSPELLS; i++)
    {
        if(SpellDFlag[i] == 0)
        {
            SpellObj[i].Color = Ui.Cyan;
            SpellObj[i].Radius = 2.4f;
            SpellObj[i].PolyLevel = 1;
            SpellObj[i].Body.Enable3D = TRUE;
            SpellObj[i].Body.Pos = VGet(Pos->x, Pos->y + 21.0f, Pos->z);
            SpellObj[i].Body.Rot = VGet(Rot->x, Rot->y, Rot->z);
            SpellObj[i].Body.Rot.x = Physics.Formula.RelAngle3(sObj.Pos,SpellObj[i].Body.Pos);
            SpellDFlag[i] = 1;

            SpellObj[i+1].Color = Ui.Cyan;
            SpellObj[i+1].Radius = 2.4f;
            SpellObj[i+1].PolyLevel = 1;
            SpellObj[i+1].Body.Enable3D = TRUE;
            SpellObj[i+1].Body.Pos = VGet(Pos->x, Pos->y + 8.0f, Pos->z);
            SpellObj[i+1].Body.Rot = VGet(Rot->x, Rot->y, Rot->z);
            Physics.Propel(SpellObj[i+1].Body,20,+DX_PI_F/2);
            SpellObj[i+1].Body.Rot.y = Physics.Formula.RelAngle2(SpellObj[i+1].Body.Pos,sObj.Pos);
            SpellObj[i+1].Body.Rot.x = Physics.Formula.RelAngle3(sObj.Pos,SpellObj[i+1].Body.Pos);
            SpellDFlag[i+1] = 1;

            SpellObj[i+2].Color = Ui.Cyan;
            SpellObj[i+2].Radius = 2.4f;
            SpellObj[i+2].PolyLevel = 1;
            SpellObj[i+2].Body.Enable3D = TRUE;
            SpellObj[i+2].Body.Pos = VGet(Pos->x, Pos->y + 1.0f, Pos->z);
            SpellObj[i+2].Body.Rot = VGet(Rot->x, Rot->y, Rot->z);
            Physics.Propel(SpellObj[i+2].Body,20,-DX_PI_F/2);
            SpellObj[i+2].Body.Rot.y = Physics.Formula.RelAngle2(SpellObj[i+2].Body.Pos,sObj.Pos);
            SpellObj[i+2].Body.Rot.x = Physics.Formula.RelAngle3(sObj.Pos,SpellObj[i+2].Body.Pos);
            SpellDFlag[i+2] = 1;
            break;
        }
    }
}

void PlayerData::UpdateSpells()
{
    for(uint_fast8_t i = 0; i < MAXSPELLS; i++)
    {
        if(SpellDFlag[i] == 1)
        {
            Physics.PropelFast(SpellObj[i].Body,MOVEMENT_SPEED*6);
            if(SpellObj[i].Body.Pos.x > 3000.0f || SpellObj[i].Body.Pos.x < -3000.0f) SpellDFlag[i] = 0;
            if(SpellObj[i].Body.Pos.z > 3000.0f || SpellObj[i].Body.Pos.z < -3000.0f) SpellDFlag[i] = 0;
            if(SpellObj[i].Body.Pos.y > 3000.0f || SpellObj[i].Body.Pos.y < -3000.0f) SpellDFlag[i] = 0;
        }
    }
}

void PlayerData::Draw(const Sphere_t *sObj)
{
    if(TargetLock == TRUE)
    {
        Marker.SizeOffset = Physics.Formula.Distance3(*Pos,sObj[Selected].Pos) *.006f;
        Marker.Size = 1.6f + Marker.SizeOffset;
        Ui.DrawMarker3D(Marker, sObj[Selected].Pos, (16.0f + Ui.Flux), 0.0f, 0.0f, 1.05f - (Marker.Size*.03f));
    }

    for(uint_fast8_t i = 0; i < MAXSPELLS; i++)
    {
        if(SpellDFlag[i] == 1)
        {
            DrawSphere3D(SpellObj[i].Body.Pos,SpellObj[i].Radius,SpellObj[i].PolyLevel,SpellObj[i].Color,Ui.Black,FALSE);
            DrawCapsule3D(SpellObj[i].Body.Pos,VGet(SpellObj[i].Body.Pos.x,SpellObj[i].Body.Pos.y-0.40f,SpellObj[i].Body.Pos.z),SpellObj[i].Radius,SpellObj[i].PolyLevel,Ui.Turquoise,Ui.Black,FALSE);
        }
    }

    Model.Draw(MMD);
}