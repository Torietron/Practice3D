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
#define UNIQUE_SPELLS 3
#define SPELL_ONE_CAST_TIME 22
#define SPELL_TWO_CAST_TIME 29
#define SPELL_THREE_CAST_TIME 8

static const float ROTATE_SPEED = DX_PI_F/45;
static const float MOVEMENT_SPEED = DX_PI_F/5;

static bool DisplayErrorPlayerState = TRUE;
static bool DisplayErrorCreateSpell = TRUE;
static uint_least8_t CameraLock = TRUE, TargetLock = FALSE;
static uint_fast8_t SpellDFlag[MAXSPELLS] = {0};
static Spell_t SpellObj[MAXSPELLS];
static PhysicsLastTime_t Cast[UNIQUE_SPELLS];

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
    SpeedBonus = 0.0f;
    isCasting = FALSE, Jump = FALSE, Blinked = FALSE, Morphed = FALSE;
    MMD.Body.Grounded = TRUE;
    GCD.Event = FALSE;

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
    MMD.Body.gFloor.y = 0.0f;
    MMD.Body.Radius = 1.5f;

    Sig.EnableModi = TRUE;
    Sig.Body.Enable3D = TRUE;
    Sig.a = -31, Sig.b = 29, Sig.c = 33, Sig.d = -33;

    MainCircle.a = -31, MainCircle.b = 29, MainCircle.c = 33, MainCircle.d = -33;
    MainCircle.EnableModi = TRUE;
    MainCircle.Body.Enable3D = TRUE;

    MiniCircle.cx = 0.33f, MiniCircle.cy = 0.3f;
    MiniCircle.Angle = 0.0f;
    MiniCircle.Size = 13.0f;
    MiniCircle.EnableModi = FALSE;
    MiniCircle.Body.Enable3D = TRUE;

    EnergyWisp.cx = 0.5f, EnergyWisp.cy = -1.0f;
    EnergyWisp.Angle = 0.0f;
    EnergyWisp.Size = 11.0f;
    EnergyWisp.EnableModi = FALSE;
    EnergyWisp.Body.Enable3D = TRUE;

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

    Sig.SpriteH[0] = LoadGraph(_T("dat/c5b.png"));
    LoadDivGraph(_T("core/ph8.png"),120,11,11,500,500,EnergyWisp.SpriteH);
    LoadDivGraph(_T("dat/cc36k.png"),117,11,11,405,405,MainCircle.SpriteH);
    EnergyWisp.SpriteMax = 119;
    EnergyWisp.SpriteIndex = 0;
    MainCircle.SpriteMax = 116;
    MainCircle.SpriteIndex = 0;

    Sig.SpritePtr = &Sig.SpriteH[0];
    Sig.SpritePtr_D = &Sig.SpritePtr;
    EnergyWisp.SpritePtr = &EnergyWisp.SpriteH[0];
    EnergyWisp.SpritePtr_D = &EnergyWisp.SpritePtr;
    MainCircle.SpritePtr = &MainCircle.SpriteH[0];

    MiniCircle.SpritePtr_D = &MainCircle.SpritePtr;

    GCD.Time = GetNowCount();
}

void PlayerData::Update(const Sphere_t *sObj, int_fast16_t Destroyed, const int_fast16_t MAX)
{
    Last = VGet(Pos->x, Pos->y, Pos->z);
    
    //Player Movement controls
    if(Key.Poll[KEY_INPUT_Q] >= 1 && isCasting == FALSE) Rot->y -= ROTATE_SPEED;
    if(Key.Poll[KEY_INPUT_E] >= 1 && isCasting == FALSE) Rot->y += ROTATE_SPEED;
    if(Key.Poll[KEY_INPUT_A] >= 1 && isCasting == FALSE)
    {
        Physics.Propel(MMD.Body,MOVEMENT_SPEED+SpeedBonus,-DX_PI_F/2);
        MMD.RotOffset.x = DX_PI_F*2;
        if(MMD.Body.Grounded == TRUE) MMD.AnimIndex = 1; //keep this anim change in mind if autoblend is off
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = (DX_PI_F/2) * -1;
    }
    if(Key.Poll[KEY_INPUT_D] >= 1 && isCasting == FALSE)
    {
        Physics.Propel(MMD.Body,MOVEMENT_SPEED+SpeedBonus,+DX_PI_F/2);
        MMD.RotOffset.x = DX_PI_F*2;
        if(MMD.Body.Grounded == TRUE) MMD.AnimIndex = 1;
        else MMD.AnimIndex = 2;
        MMD.Reverse = FALSE;
        MMD.RotOffset.y = DX_PI_F/2;
    }
    
    if(Key.Poll[KEY_INPUT_W] >= 1 && isCasting == FALSE) 
    {
        Physics.Propel(MMD.Body,MOVEMENT_SPEED+SpeedBonus);
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
        Physics.Propel(MMD.Body,MOVEMENT_SPEED*0.75f+SpeedBonus,DX_PI_F);
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
            if(sObj[Selected].Active) 
            {
                Target = &sObj[Selected].Body.Pos; 
                break;
            }
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
            Screen.C3D.AngleH = Physics.Formula.RelAngle2(MMD.Body.Pos, sObj[Selected].Body.Pos);
            Rot->y = Screen.C3D.AngleH;
            Screen.C3D.Anchor = Rot->y - DX_PI_F/10;
            Ui.UpdateFlux();
        }
        else Selected = -1, TargetLock = FALSE;
    }

    Screen.C3D.Pos.z = Pos->z + cosf(Screen.C3D.Anchor)*40;
    Screen.C3D.Pos.x = Pos->x + sinf(Screen.C3D.Anchor)*40;
    
    //horizontal lock formula
    Screen.C3D.AngleH = Physics.Formula.RelAngle2(MMD.Body.Pos, Screen.C3D.Pos);

    //vertical lock formula
    Screen.C3D.AngleV = Physics.Formula.RelAngle3(Screen.C3D.Pos, MMD.Body.Pos) - Screen.C3D.OffsetV;
    
    //Handle Jump
    if(Jump == TRUE && MMD.Body.Grounded == FALSE)
    {
        Physics.Manipulate(MMD.Body);
        MMD.RotOffset.x = DX_PI_F*2;
        Physics.GravityFloor(MMD.Body,AXIS_Y);
        if(Physics.GetLastValue(LAST_GRAVITY_Y) == 0) Jump = FALSE, MMD.Body.Grounded = TRUE;
    }

    //Casting cooldown control
    if(GCD.Event)
    {
        if(Physics.Delta.Time(GCD,300))
        {
            GCD.Event = FALSE;
        }
    }
    if(Cancelled.Event)
    {
        if(Physics.Delta.Time(Cancelled,225))
        {
            Cancelled.Event = FALSE;
        }
    }

    //Casting Control
    //Spell One
    if(GCD.Event == FALSE && Key.Poll[KEY_INPUT_1] >= 4 && TargetLock == TRUE && MMD.Body.Grounded == TRUE && Cancelled.Event == FALSE && Blinked == FALSE) 
    {
        if(Key.Poll[KEY_INPUT_2] >= 1 || Key.Poll[KEY_INPUT_3] >= 1) Cancelled.Event = TRUE; //check for cancels
        else if(Cancelled.Event == FALSE)
        {
            if(MMD.State == 0) SetState(1);

            if(Cast[1].Count > 0 || Cast[2].Count > 0) //reset if another spell was active and unfinished
            {
                Cast[1].Count = 0;
                Cast[2].Count = 0;
                CastingTime = 0; 
            }
            if(Physics.Delta.Time(Cast[0],5)) CastingTime++;
            if(CastingTime >= SPELL_ONE_CAST_TIME)
            {
                GCD.Event = TRUE;
                GCD.Time = GetNowCount();
                CreateSpell(0);
                Key.Poll[KEY_INPUT_1] = 0;
                Key.Poll[KEY_INPUT_2] = 0;
                Key.Poll[KEY_INPUT_3] = 0;
                CastingTime = 0;
                Cast[0].Count = 0;
                if(Morphed) SetState(2);
                else SetState(0);
            }

            if(Morphed == TRUE && MMD.State == 2) SetState(3);
            if(Morphed == FALSE && MMD.State == 3) SetState(1);
            //Model.RunManualBlend(MMD,0.115f,0.001f);
            Model.Update(MainCircle,28);
        }
    }
    //Spell Two
    else if(GCD.Event == FALSE && Key.Poll[KEY_INPUT_2] >= 4 && TargetLock == TRUE && MMD.Body.Grounded == TRUE && Cancelled.Event == FALSE && Blinked == FALSE) 
    {
        if(Key.Poll[KEY_INPUT_1] >= 1 || Key.Poll[KEY_INPUT_3] >= 1) Cancelled.Event = TRUE; //check for cancels
        else if(Cancelled.Event == FALSE)
        {
            if(MMD.State == 0) SetState(1);

            if(Cast[0].Count > 0 || Cast[2].Count > 0)  //reset if another spell was active and unfinished
            {
                Cast[0].Count = 0;
                Cast[2].Count = 0;
                CastingTime = 0; 
            }
            if(Physics.Delta.Time(Cast[1],5)) CastingTime++;
            if(CastingTime >= SPELL_TWO_CAST_TIME)
            {
                GCD.Event = TRUE;
                GCD.Time = GetNowCount();
                CreateSpell(1);
                Key.Poll[KEY_INPUT_1] = 0;
                Key.Poll[KEY_INPUT_2] = 0;
                Key.Poll[KEY_INPUT_3] = 0;
                CastingTime = 0;
                Cast[1].Count = 0;
                if(Morphed) SetState(2);
                else SetState(0);
            }
            
            if(Morphed == TRUE && MMD.State == 2) SetState(3);
            if(Morphed == FALSE && MMD.State == 3) SetState(1);
            Model.RunManualBlend(MMD,0.115f,0.001f);
            Model.Update(MainCircle,28);
        }
    }
    //Spell Three
    else if(Key.Poll[KEY_INPUT_3] >= 4 && MMD.Body.Grounded == TRUE && Cancelled.Event == FALSE && Morphed == FALSE) 
    {
        if(Key.Poll[KEY_INPUT_1] >= 1 || Key.Poll[KEY_INPUT_2] >= 1) Cancelled.Event = TRUE; //check for cancels
        else if(Cancelled.Event == FALSE)
        {
            if(MMD.State != 2) SetState(2);

            if(Cast[0].Count > 0 || Cast[1].Count > 0)  //reset time if another spell was active and unfinished
            {
                Cast[0].Count = 0;
                Cast[1].Count = 0;
                CastingTime = 0; 
            }
            if(Physics.Delta.Time(Cast[2],5)) CastingTime++;
            if(CastingTime >= SPELL_THREE_CAST_TIME)
            {
                GCD.Event = TRUE;
                GCD.Time = GetNowCount(); 
                CreateSpell(2);
                Key.Poll[KEY_INPUT_1] = 0;
                Key.Poll[KEY_INPUT_2] = 0;
                Key.Poll[KEY_INPUT_3] = 0;
                CastingTime = 0;
                Cast[2].Count = 0;
            }

            Model.RunManualBlend(MMD,3.115f,1.001f,29.084999f,7.039001f);
            Model.Update(MainCircle,28);
        }
    }

    //Handle key release
    if(MMD.State > 0 && ((Key.Poll[KEY_INPUT_1] == 0 && Key.Poll[KEY_INPUT_2] == 0 && Key.Poll[KEY_INPUT_3] == 0) || (TargetLock == FALSE && Key.Poll[KEY_INPUT_3] == 0)) )
    {
        if(Morphed == TRUE)
        {
            SetState(2);
        }
        else if(Cast[2].Count > 0 && Morphed == FALSE)
        {
            if(Blinked == FALSE)
            {
                BlinkDecay = 2.0f;
                if(Key.Poll[KEY_INPUT_S] >=1) 
                {
                    BlinkOffset.y += DX_PI_F;
                }
                else BlinkOffset.y = 0.0f;
                BlinkRot.y = MMD.RotOffset.y;
                Blinked = TRUE;
            }
        }
        else
        {
            SetState(0);
            for(uint_fast8_t i = 0; i < UNIQUE_SPELLS; i++)
            {
                Cast[i].Count = 0;
            }
        }
    }

    //Handle spell cancellation - why are you pushing all the keys tho
    if(MMD.State > 0 && Cancelled.Event) 
    {
        Cancelled.Time = GetNowCount();
        SetState(0);
        for(uint_fast8_t i = 0; i < UNIQUE_SPELLS; i++)
        {
            Cast[i].Count = 0;
        }
    }

    UpdateSpells();
    Model.Update(MMD);
}

/*  0 = Idle/Normal
    1 = Normal Casting 
    2 = Morphed/Energy Form 
    3 = Morphed Casting*/
int PlayerData::SetState(const uint_fast8_t &state)
{
    switch(state)
    {
        case 0:

            isCasting = FALSE;
            if(MMD.AutoBlend == FALSE) {MMD.AutoBlend = TRUE; Model.EndManualBlend(MMD);}
            CastingTime = 0;
            MainCircle.Flux = 0.0f;
            MainCircle.SpriteIndex = 0;

            MMD.State = 0;

            return 0;
        
        case 1:

            isCasting = TRUE;
            if(MMD.AutoBlend) MMD.AutoBlend = FALSE;
            else Model.EndManualBlend(MMD);
            MMD.AnimIndex = 4;
            MMD.RotOffset.y = 0;
            MMD.RotOffset.x = 0;
            Sig.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
            MainCircle.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
            MiniCircle.Body.Pos = VGet(Target->x,Target->y,Target->z);
            MiniCircle.Body.Rot = VGet(Rot->x,Rot->y+DX_PI_F,Rot->z);

            Physics.PropelFast(MiniCircle.Body,20);
            Model.SetManualBlend(MMD,0,0.05f,4,0.03f);
            MMD.State = 1;

            return 1;
        
        case 2:

            isCasting = FALSE;
            if(MMD.AutoBlend) MMD.AutoBlend = FALSE;
            else Model.EndManualBlend(MMD);
            CastingTime = 0;
            Sig.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
            MainCircle.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);

            Model.SetManualBlend(MMD,0,0.05f,2,0.03f);
            MMD.State = 2;

            return 2;

        case 3:

            isCasting = FALSE;
            if(MMD.AutoBlend) MMD.AutoBlend = FALSE;
            else Model.EndManualBlend(MMD);
            Sig.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
            MainCircle.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
            MiniCircle.Body.Pos = VGet(Target->x,Target->y,Target->z);
            MiniCircle.Body.Rot = VGet(Rot->x,Rot->y+DX_PI_F,Rot->z);

            Physics.PropelFast(MiniCircle.Body,20);
            Model.SetManualBlend(MMD,0,0.05f,4,0.03f);
            MMD.State = 3;

            return 3;

        default:

            if(DisplayErrorPlayerState)
            {
                MessageBox
                (
                    NULL,
                    TEXT("State Error: PlayerData SetState()\n switch defaulted."),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
                DisplayErrorPlayerState = FALSE;
            }

            return -1;
    }
}

//I will have to make models later
int PlayerData::CreateSpell(const uint_fast8_t &spelltype)
{
    switch(spelltype)
    {
        case 0:
            
            for(uint_fast8_t i = 0; i < MAXSPELLS; i++)
            {
                if(SpellDFlag[i] == 0)
                {
                    SpellObj[i].Color = Ui.Cyan;
                    SpellObj[i].Body.Radius = 2.4f;
                    SpellObj[i].PolyLevel = 1;
                    SpellObj[i].Body.Enable3D = TRUE;
                    SpellObj[i].Body.Pos = VGet(Pos->x, Pos->y + 21.0f, Pos->z);
                    SpellObj[i].Body.Rot = VGet(Rot->x, Rot->y, Rot->z);
                    SpellObj[i].Body.Rot.x = Physics.Formula.RelAngle3(*Target,SpellObj[i].Body.Pos);
                    SpellDFlag[i] = 1;

                    SpellObj[i+1].Color = Ui.Cyan;
                    SpellObj[i+1].Body.Radius = 2.4f;
                    SpellObj[i+1].PolyLevel = 1;
                    SpellObj[i+1].Body.Enable3D = TRUE;
                    SpellObj[i+1].Body.Pos = VGet(Pos->x, Pos->y + 8.0f, Pos->z);
                    SpellObj[i+1].Body.Rot = VGet(Rot->x, Rot->y, Rot->z);
                    Physics.Propel(SpellObj[i+1].Body,20,+DX_PI_F/2);
                    SpellObj[i+1].Body.Rot.y = Physics.Formula.RelAngle2(SpellObj[i+1].Body.Pos,*Target);
                    SpellObj[i+1].Body.Rot.x = Physics.Formula.RelAngle3(*Target,SpellObj[i+1].Body.Pos);
                    SpellDFlag[i+1] = 1;

                    SpellObj[i+2].Color = Ui.Cyan;
                    SpellObj[i+2].Body.Radius = 2.4f;
                    SpellObj[i+2].PolyLevel = 1;
                    SpellObj[i+2].Body.Enable3D = TRUE;
                    SpellObj[i+2].Body.Pos = VGet(Pos->x, Pos->y + 1.0f, Pos->z);
                    SpellObj[i+2].Body.Rot = VGet(Rot->x, Rot->y, Rot->z);
                    Physics.Propel(SpellObj[i+2].Body,20,-DX_PI_F/2);
                    SpellObj[i+2].Body.Rot.y = Physics.Formula.RelAngle2(SpellObj[i+2].Body.Pos,*Target);
                    SpellObj[i+2].Body.Rot.x = Physics.Formula.RelAngle3(*Target,SpellObj[i+2].Body.Pos);
                    SpellDFlag[i+2] = 1;
                    break;
                }
            }
            return 0;

        case 1:

            for(uint_fast8_t i = 0; i < MAXSPELLS; i++)
            {
                if(SpellDFlag[i] == 0)
                {
                    SpellObj[i].Color = Ui.Red;
                    SpellObj[i].Body.Radius = 10.0f;
                    SpellObj[i].PolyLevel = 128;
                    SpellObj[i].Body.Enable3D = TRUE;
                    SpellObj[i].Probe.Enable3D = TRUE;
                    SpellObj[i].Body.Pos = VGet(Pos->x, Pos->y+11.0f, Pos->z);
                    SpellObj[i].Body.Rot = VGet(Rot->x, Rot->y, Rot->z);
                    SpellObj[i].Body.Rot.y = Physics.Formula.RelAngle2(SpellObj[i].Body.Pos,*Target);
                    SpellObj[i].Body.Rot.x = Physics.Formula.RelAngle3(*Target,SpellObj[i].Body.Pos);
                    SpellDFlag[i] = 2;
                    break;
                }
            }
            return 1;
        
        case 2:

            //Mighty Morphin' Miku Rangers?
            Morphed = TRUE;
            SpeedBonus = 4.0f;
            EnergyWisp.Angle = 0.0f;
            return 2;

        default:

            if(DisplayErrorCreateSpell)
            {
                MessageBox
                (
                    NULL,
                    TEXT("Spell Error: PlayerData CreateSpell()\n switch defaulted."),
                    TEXT("Error"),
                    MB_OK | MB_ICONERROR 
                );
                DisplayErrorCreateSpell = FALSE;
            }

            return -1;
    }
}

void PlayerData::UpdateSpells()
{
    for(uint_fast8_t i = 0; i < MAXSPELLS; i++)
    {
        if(SpellDFlag[i] == 1)
        {
            Physics.PropelFast(SpellObj[i].Body,MOVEMENT_SPEED*9);
            if(SpellObj[i].Body.Pos.x > 3000.0f || SpellObj[i].Body.Pos.x < -3000.0f) SpellDFlag[i] = 0;
            if(SpellObj[i].Body.Pos.z > 3000.0f || SpellObj[i].Body.Pos.z < -3000.0f) SpellDFlag[i] = 0;
            if(SpellObj[i].Body.Pos.y > 3000.0f || SpellObj[i].Body.Pos.y < -3000.0f) SpellDFlag[i] = 0;
        }

        if(SpellDFlag[i] == 2)
        {
            SpellObj[i].Probe.Pos = VGet(SpellObj[i].Body.Pos.x, SpellObj[i].Body.Pos.y+3, SpellObj[i].Body.Pos.z);
            SpellObj[i].Probe.Rot = VGet(SpellObj[i].Body.Rot.x+DX_PI_F/4, SpellObj[i].Body.Rot.y+DX_PI_F/3, SpellObj[i].Body.Rot.z);
            Physics.PropelFast(SpellObj[i].Probe,1.2f);
            Physics.PropelFast(SpellObj[i].Body,MOVEMENT_SPEED*3.0f);
            if(SpellObj[i].Body.Pos.x > 2000.0f || SpellObj[i].Body.Pos.x < -2000.0f) SpellDFlag[i] = 0;
            if(SpellObj[i].Body.Pos.z > 2000.0f || SpellObj[i].Body.Pos.z < -2000.0f) SpellDFlag[i] = 0;
            if(SpellObj[i].Body.Pos.y > 2000.0f || SpellObj[i].Body.Pos.y < -2000.0f) SpellDFlag[i] = 0;
        }
    }

    if(MMD.State == 2 || MMD.State == 3)
    {
        if(Morphed)
        {
            if(Physics.Delta.Time(Cast[2],90)) SpeedBonus -= 0.14f;
            if(SpeedBonus < 0.5f) SpeedBonus = 0.0f, Morphed = FALSE;
            Sig.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
            EnergyWisp.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
            MainCircle.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
            if(SpeedBonus < 1.2f) Model.Update(MainCircle,28);
            Model.Update(EnergyWisp,48,(int)floorf(SpeedBonus*2.5f));
            Cast[2].Count = 0;

            if(TargetLock)
            {
                MiniCircle.Body.Pos = VGet(Target->x,Target->y,Target->z);
                MiniCircle.Body.Rot = VGet(Rot->x,Rot->y+DX_PI_F,Rot->z);
                Physics.PropelFast(MiniCircle.Body,20);
            }
        }
        else if(Blinked)
        {
            if(BlinkDecay <= 0.0f) 
            {
                BlinkDecay = 0.0f;
                Cast[2].Count = 0;
                SetState(0);
                for(uint_fast8_t i = 0; i < UNIQUE_SPELLS; i++)
                {
                    Cast[i].Count = 0;
                }
                BlinkOffset.y = 0.0f;
                Blinked = FALSE;
            }
            else 
            {
                BlinkDecay -= 0.1f;
                Sig.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
                EnergyWisp.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
                Physics.PropelFast(MMD.Body,3.2f,BlinkRot.y+BlinkOffset.y);
                Model.Update(EnergyWisp,48,4);
            }   
        }
        else
        {
            Sig.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
            MainCircle.Body.Pos = VGet(Pos->x,Pos->y,Pos->z);
        }
    }
}

void PlayerData::Draw(const Sphere_t *sObj)
{
    if(TargetLock == TRUE)
    {
        Marker.SizeOffset = Physics.Formula.Distance3(*Pos,sObj[Selected].Body.Pos) *.006f;
        Marker.Size = 1.6f + Marker.SizeOffset;
        Ui.DrawMarker3D(Marker, sObj[Selected].Body.Pos, (16.0f + Ui.Flux), 0.0f, 0.0f, 1.05f - (Marker.Size*.03f));
    }

    for(uint_fast8_t i = 0; i < MAXSPELLS; i++)
    {
        if(SpellDFlag[i] == 1)
        {
            DrawSphere3D(SpellObj[i].Body.Pos,SpellObj[i].Body.Radius,SpellObj[i].PolyLevel,SpellObj[i].Color,Ui.Black,FALSE);
            DrawCapsule3D(SpellObj[i].Body.Pos,VGet(SpellObj[i].Body.Pos.x,SpellObj[i].Body.Pos.y-0.40f,SpellObj[i].Body.Pos.z),SpellObj[i].Body.Radius,SpellObj[i].PolyLevel,Ui.Turquoise,Ui.Black,FALSE);
        }
        if(SpellDFlag[i] == 2)
        {
            DrawCone3D(SpellObj[i].Body.Pos,SpellObj[i].Probe.Pos,SpellObj[i].Body.Radius,SpellObj[i].PolyLevel,SpellObj[i].Color,Ui.Black,FALSE);
        }
    }

    if(isCasting == TRUE && MMD.State == 1)
    {
        Model.Draw(Sig,11.0f,12.0f,-9.0f,-8.0f);
        Model.Draw(MainCircle,11.0f,12.0f,-9.0f,-8.0f);
        Model.Draw(MiniCircle);
    }

    if(isCasting == FALSE && (MMD.State == 2 || MMD.State == 3))
    {
        if(Morphed == FALSE && Blinked == FALSE) Model.Draw(MainCircle,11.0f,12.0f,-9.0f,-8.0f);
        if(Morphed == FALSE || SpeedBonus > 0.0f) Model.Draw(Sig,11.0f,12.0f,-9.0f,-8.0f);
    }

    if(Morphed == TRUE && TargetLock == TRUE && MMD.State == 3) Model.Draw(MiniCircle);
    if(Morphed == FALSE && Blinked == FALSE) Model.Draw(MMD);
    if(Morphed == TRUE || Blinked == TRUE) Model.Draw(EnergyWisp);
    if(Morphed == TRUE && SpeedBonus < 1.2f) Model.Draw(MainCircle,11.0f,12.0f,-9.0f,-8.0f);
}