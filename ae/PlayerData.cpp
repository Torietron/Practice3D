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
#define SPELL_ONE_CAST_TIME 90

static const float ROTATE_SPEED = DX_PI_F/45;
static const float MOVEMENT_SPEED = DX_PI_F/5;

static uint_least8_t CameraLock = TRUE, TargetLock = FALSE;
static uint_fast8_t SpellDFlag[MAXSPELLS] = {0};
static Spell_t SpellObj[MAXSPELLS];
static PhysicsLastTime_t Cast[2];

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
    CastingTime = 0, Selected = -1; //-1 = no target
    isCasting = FALSE, Jump = FALSE, MMD.Body.Grounded = TRUE;

    MMD.Body.Enable3D = TRUE;
    MMD.AttachIndex = 0;
    MMD.AnimSet = 0;
    MMD.AnimIndex = 0;
    MMD.PlayTime = 0.0f;
    MMD.PlayOffset = 0.0f;
    MMD.Reverse = FALSE, MMD.Event = FALSE;
    MMD.AutoBlend = TRUE;

    MMD.Body.VelBase = 1.5f, MMD.Body.VelMax = 5.0f;
    MMD.Body.AccelRate = 0.35f; MMD.Body.TermVel = 3.0f;
    MMD.Body.MassRatio = 0.8f, MMD.Body.Interval = 50;

    MainCircle.a = -31, MainCircle.b = 29, MainCircle.c = 33, MainCircle.d = -33;
    MainCircle.EnableModi = TRUE;
    MainCircle.Body.Enable3D = TRUE;

    MiniCircle.cx = 0.0f, MiniCircle.cy = -0.5f;
    MiniCircle.Angle = 0.0f;
    MiniCircle.Size = 21.0f;
    MiniCircle.EnableModi = FALSE;
    MiniCircle.Body.Enable3D = TRUE;

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
    MMD.AttachIndex = MV1AttachAnim(MMD.ModelH, 0, -1, FALSE);
    MMD.TotalTime = MV1GetAttachAnimTotalTime(MMD.ModelH,MMD.AttachIndex);
    MV1SetupCollInfo(MMD.ModelH, -1, 1, 1, 1);

    Marker.SpriteH[0] = LoadGraph(_T("core/ph3.png"));
    Marker.SpritePtr = &Marker.SpriteH[0];

    MainCircle.SpriteH[0] = LoadGraph(_T("dat/cc35-0.png"));
    MainCircle.SpriteH[1] = LoadGraph(_T("dat/cc35-1.png"));
    MainCircle.SpriteH[2] = LoadGraph(_T("dat/cc35-2.png"));
    MainCircle.SpriteH[3] = LoadGraph(_T("dat/cc35-3.png"));
    MainCircle.SpriteH[4] = LoadGraph(_T("dat/cc35-4.png"));
    MainCircle.SpriteH[5] = LoadGraph(_T("dat/cc35-5.png"));
    MainCircle.SpriteH[6] = LoadGraph(_T("dat/cc35-6.png"));
    MainCircle.SpriteH[7] = LoadGraph(_T("dat/cc35-7.png"));
    MainCircle.SpriteH[8] = LoadGraph(_T("dat/cc35-8.png"));
    MainCircle.SpriteH[9] = LoadGraph(_T("dat/cc35-9.png"));
    MainCircle.SpriteH[10] = LoadGraph(_T("dat/cc35-10.png"));
    MainCircle.SpriteH[11] = LoadGraph(_T("dat/cc35-11.png"));
    MainCircle.SpriteH[12] = LoadGraph(_T("dat/cc35-12.png"));
    MainCircle.SpriteH[13] = LoadGraph(_T("dat/cc35-13.png"));
    MainCircle.SpriteH[14] = LoadGraph(_T("dat/cc35-14.png"));
    MainCircle.SpriteH[15] = LoadGraph(_T("dat/cc35-15.png"));

    MainCircle.SpritePtr = &MainCircle.SpriteH[0];
    MainCircle.SpriteIndex = 0;
    MainCircle.SpriteMax = 16;

    MiniCircle.SpritePtr = &MainCircle.SpriteH[0];
}

void PlayerData::Update(const Sphere_t *sObj, int_fast16_t Destroyed, const int_fast16_t MAX)
{
    Last = VGet(Pos->x, Pos->y, Pos->z);

    if(Key.Poll[KEY_INPUT_Q] >= 1 && isCasting == FALSE) Rot->y -= ROTATE_SPEED;
    if(Key.Poll[KEY_INPUT_E] >= 1 && isCasting == FALSE) Rot->y += ROTATE_SPEED;
    if(Key.Poll[KEY_INPUT_A] >= 1 && isCasting == FALSE)
    {
        Physics.Propel(MMD.Body,MOVEMENT_SPEED,-DX_PI_F/2);
        MMD.RotOffset.x = DX_PI_F*2;
        if(MMD.Body.Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = (DX_PI_F/2) * -1;
    }
    if(Key.Poll[KEY_INPUT_D] >= 1 && isCasting == FALSE)
    {
        Physics.Propel(MMD.Body,MOVEMENT_SPEED,+DX_PI_F/2);
        MMD.RotOffset.x = DX_PI_F*2;
        if(MMD.Body.Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = DX_PI_F/2;
    }
    
    if(Key.Poll[KEY_INPUT_W] >= 1 && isCasting == FALSE) 
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
    
    if(Key.Poll[KEY_INPUT_S] >= 1 && isCasting == FALSE) 
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
    
    if(Key.Poll[KEY_INPUT_SPACE] == 1 && isCasting == FALSE)
    {
        if(MMD.Body.Grounded == TRUE) 
        {
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
        if(MMD.Body.Grounded == TRUE && isCasting == FALSE) MMD.AnimIndex = 0, MMD.RotOffset.x = DX_PI_F*2, MMD.Reverse = FALSE;
        if(MMD.Body.Grounded == FALSE && isCasting == FALSE) MMD.AnimIndex = 2, MMD.RotOffset.x = DX_PI_F*2, MMD.Reverse = FALSE;
        MMD.RotOffset.y = 0.0f;
    }

    //Camera
    //keep the camera a set distance from player
    if(Key.Poll[KEY_INPUT_J] >= 1 && isCasting == FALSE)
    {
        Rot->y -= ROTATE_SPEED;
        Screen.C3D.Anchor = Rot->y;
    }
    if(Key.Poll[KEY_INPUT_K] >= 1 && isCasting == FALSE)
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

    //Caster control
    if(Key.Poll[KEY_INPUT_1] >= 1 && TargetLock == TRUE && MMD.Body.Grounded == TRUE) 
    {
        isCasting = TRUE;
        MMD.AnimIndex = 4;
        MMD.RotOffset.y = 0;
        MMD.RotOffset.x = 0;

        MainCircle.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);

        MiniCircle.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
        MiniCircle.Body.Rot = VGet(Rot->x,Rot->y+DX_PI_F,Rot->z);
        Physics.PropelFast(MiniCircle.Body,4);

        if(Physics.Delta.Time(Cast[0],5)) CastingTime++;
        if(CastingTime >= SPELL_ONE_CAST_TIME)
        {
            CreateSpell(sObj[Selected]);
            Key.Poll[KEY_INPUT_1] = 0;
            CastingTime = 0;
        } 
        
        if(Model.Update(MainCircle,55) == 1) MiniCircle.SpritePtr = &MainCircle.SpriteH[MainCircle.SpriteIndex];
    }

    //Reset time on release
    if(Key.Poll[KEY_INPUT_1] == 0) 
    {
        isCasting = FALSE;
        CastingTime = 0;
        MainCircle.Flux = 0.0f;
    }

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

    if(isCasting == TRUE)
    {
        Model.Draw(MainCircle,11.0f,12.0f,-9.0f,-8.0f);
        Model.Draw(MiniCircle);
    }

    Model.Draw(MMD);
}