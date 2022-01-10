#include "DxLib.h"
#include <cstdint>
#include "ModelData.h"

static uint_fast32_t dTime; 
static float fTime;

ModelData::ModelData(float rate)
:play_rate(rate)
{
}

void ModelData::Update(MMD_t &m)
{
    //In the case that AutoBlend is enabled and finds no secondary anim (LastIndex) to blend with
    if(m.LastIndex == 0 && m.AutoBlend == TRUE)
    {
        //uses currently set animation as its default secondary
        m.LastIndex = MV1AttachAnim(m.ModelH,m.AnimSet, -1, FALSE);
        m.LastPlayTime = 0.0f;
    }

    //Check and Update - With AutoBlend
    if(m.AnimSet != m.AnimIndex && m.AutoBlend == TRUE)
    {
        m.BlendDecay = 0.20f; //initial blending rate
        MV1DetachAnim(m.ModelH,m.LastIndex);
        MV1DetachAnim(m.ModelH,m.AttachIndex);
        m.LastIndex = MV1AttachAnim(m.ModelH, m.AnimSet, -1, FALSE);
        m.LastPlayTime = m.PlayTime;
        m.AttachIndex = MV1AttachAnim(m.ModelH, m.AnimIndex, -1, FALSE);
        m.TotalTime = MV1GetAttachAnimTotalTime(m.ModelH,m.AttachIndex);
        m.PlayTime = 0.0f;
        m.AnimSet = m.AnimIndex; 
    }
    //Check and Update - Without AutoBlend
    if(m.AnimSet != m.AnimIndex && m.AutoBlend == FALSE)
    {
        if(m.LastIndex != 0) //if previously set
        {
            MV1DetachAnim(m.ModelH,m.LastIndex); 
            m.LastIndex = 0, m.LastPlayTime = 0.0f;  
        }
        MV1DetachAnim(m.ModelH,m.AttachIndex);
        m.AttachIndex = MV1AttachAnim(m.ModelH, m.AnimIndex, -1, FALSE);
        m.TotalTime = MV1GetAttachAnimTotalTime(m.ModelH,m.AttachIndex);
        m.PlayTime = 0.0f;
        m.AnimSet = m.AnimIndex;
    }

    //Advance through the animation
    if(m.Reverse)
    {
        m.PlayTime -= (play_rate - m.PlayOffset);
        if(m.PlayTime <= 0) m.PlayTime = m.TotalTime;
    }
    else
    {
        m.PlayTime += play_rate;
        if(m.PlayTime >= m.TotalTime) m.PlayTime = 0.0f;
    }

    //Apply new time point to the currently attached animation
    MV1SetAttachAnimTime(m.ModelH,m.AttachIndex,m.PlayTime); 
    //For AutoBlending - apply the old playback time point to the previous animation
    if(m.AutoBlend)
    {
        MV1SetAttachAnimTime(m.ModelH,m.LastIndex,m.LastPlayTime); 
    }

    //Update spatial data
    MV1SetPosition(m.ModelH,m.Body.Pos);
    MV1SetRotationXYZ(m.ModelH,VGet((m.Body.Rot.x+m.RotOffset.x),(m.Body.Rot.y+m.RotOffset.y),(m.Body.Rot.z+m.RotOffset.z)));
    MV1RefreshCollInfo(m.ModelH,-1);
}

void ModelData::Update(MQO_t &m)
{
    MV1SetPosition(m.ModelH,m.Pos);
    MV1SetRotationXYZ(m.ModelH,m.Rot);
    MV1RefreshCollInfo(m.ModelH,-1);
}

int ModelData::Update(Sprite3D_t &m, const float &framerate)
{
    if(m.SpriteMax == 0) return -1;

    dTime = GetNowCount();
    fTime = 1000 / framerate;
    if(dTime - m.AnimTime > fTime)
    {
        m.AnimTime = GetNowCount();
        m.SpriteIndex++;
        if(m.SpriteIndex > m.SpriteMax) m.SpriteIndex = 0;
        m.SpritePtr = &m.SpriteH[m.SpriteIndex];
        return 1;
    }

    return 0;
}

//Blend using the animation in BlendIndex
void ModelData::ManualBlend(MMD_t &m, const float &blendRate)
{
    MV1SetAttachAnimBlendRate(m.ModelH,m.BlendIndex,blendRate);
}

/*  Set AutoBlend to TRUE for smoother transistions
    - AutoBlend requires an animation to be loaded into IdleIndex 
    - AutoBlend uses and updates LastIndex as necessary */
void ModelData::Draw(MMD_t &m, const float &blendRate1, const float &blendRate2)
{
    switch(m.AutoBlend)
    {
        case FALSE:

            if(m.LastIndex != 0) MV1SetAttachAnimBlendRate(m.ModelH,m.LastIndex,0.0f);
            break;

        case TRUE:

            MV1SetAttachAnimBlendRate(m.ModelH,m.AttachIndex,blendRate1-m.BlendDecay/2);
            if(m.BlendDecay > 0.00f)
            {
                MV1SetAttachAnimBlendRate(m.ModelH,m.LastIndex,((m.BlendDecay/2)+blendRate2));
                m.BlendDecay -= 0.02f;
            }
            else MV1SetAttachAnimBlendRate(m.ModelH,m.LastIndex,blendRate2);
            break;
    }

    MV1DrawModel(m.ModelH);
}

void ModelData::Draw(const MQO_t &m)
{
    MV1DrawModel(m.ModelH);
}

void ModelData::Draw(const Sprite3D_t &m, const float &y1, const float &y2, const float &y3, const float &y4)
{
    switch(m.EnableModi)
    {
        case FALSE:

            DrawBillboard3D(m.Body.Pos,m.cx,m.cy,m.Size,m.Angle,*m.SpritePtr,TRUE);
            break;

        case TRUE:

            DrawModiBillboard3D
            (
                m.Body.Pos,
                m.a - m.Flux, y1,
                m.b + m.Flux, y2,
                m.c + m.Flux, y3,
                m.d - m.Flux, y4,
                *m.SpritePtr,TRUE
            );
            break;
    }
}

void ModelData::SetPlayRate(const float &a)
{
    play_rate = a;
}

float ModelData::GetPlayRate()
{
    return play_rate;
}
