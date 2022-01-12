#include "DxLib.h"
#include <cstdint>
#include "ModelData.h"

static uint_fast32_t dTime; 
static float fTime, tempf1, tempf2;

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
    MV1SetPosition(m.ModelH,VGet((m.Body.Pos.x+m.PosOffset.x),(m.Body.Pos.y+m.PosOffset.y),(m.Body.Pos.z+m.PosOffset.z)));
    MV1SetRotationXYZ(m.ModelH,VGet((m.Body.Rot.x+m.RotOffset.x),(m.Body.Rot.y+m.RotOffset.y),(m.Body.Rot.z+m.RotOffset.z)));
    MV1RefreshCollInfo(m.ModelH,-1);
}

void ModelData::Update(MQO_t &m)
{
    MV1SetPosition(m.ModelH,m.Pos);
    MV1SetRotationXYZ(m.ModelH,m.Rot);
    MV1RefreshCollInfo(m.ModelH,-1);
}

int ModelData::Update(Sprite3D_t &m, const float &framerate, const uint_fast8_t &skip)
{
    if(m.SpriteMax == 0) return -1;

    dTime = GetNowCount();
    fTime = 1000 / framerate;
    if(dTime - m.AnimTime > fTime)
    {
        m.AnimTime = GetNowCount();
        m.SpriteIndex++;
        m.SpriteIndex += skip;
        if(m.SpriteIndex > m.SpriteMax) m.SpriteIndex = 0;
        m.SpritePtr = &m.SpriteH[m.SpriteIndex];
        m.SpritePtr_D = &m.SpritePtr;
        return 1;
    }

    return 0;
}

/*  Sets pair of animations for blending
    - Distorion occurs if the value exceeds 1.0f or is below 0.0f */
void ModelData::SetManualBlend(MMD_t &m, const uint_fast8_t &index1,  const float &rate1, const uint_fast8_t &index2, const float &rate2)
{
    m.BlendIndex1 = MV1AttachAnim(m.ModelH, index1, -1, FALSE);
    m.BlendIndex2 = MV1AttachAnim(m.ModelH, index2, -1, FALSE);
    MV1SetAttachAnimBlendRate(m.ModelH, m.BlendIndex1, rate1);
    MV1SetAttachAnimBlendRate(m.ModelH, m.BlendIndex2, rate2);
}

/*  Blend variance and transition
    - Distorion occurs if the value exceeds 1.0f or is below 0.0f */
void ModelData::RunManualBlend(MMD_t &m, const float &incrementRate1, const float &incrementRate2, const float &maxRate1, const float &maxRate2, const float &minRate1, const float &minRate2)
{
    tempf1 = MV1GetAttachAnimBlendRate(m.ModelH, m.BlendIndex1);
    tempf2 = MV1GetAttachAnimBlendRate(m.ModelH, m.BlendIndex2);

    tempf1 += incrementRate1;
    if(tempf1 > maxRate1) tempf1 = maxRate1;
    else if(tempf1 < minRate1) tempf1 = minRate1;
    MV1SetAttachAnimBlendRate(m.ModelH, m.BlendIndex1, tempf1);

    tempf2 += incrementRate2;
    if(tempf2 > maxRate2) tempf2 = maxRate2;
    else if(tempf2 < minRate2) tempf2 = minRate2;
    MV1SetAttachAnimBlendRate(m.ModelH, m.BlendIndex2, tempf2);
}

//Detach Blends and Reset AttachIndex to 1.0
void ModelData::EndManualBlend(MMD_t &m)
{
    MV1SetAttachAnimBlendRate(m.ModelH,m.BlendIndex1,0.0f);
    MV1SetAttachAnimBlendRate(m.ModelH,m.BlendIndex2,0.0f);
    MV1DetachAnim(m.ModelH,m.BlendIndex1);
    MV1DetachAnim(m.ModelH,m.BlendIndex2);
    MV1SetAttachAnimBlendRate(m.ModelH,m.AttachIndex,1.0f);
}

/*  Set AutoBlend to TRUE for smoother transitions
    - Add to BlendDecay before calling if you want longer transitions
    - AutoBlend Max/Min should be 1.00f to 0.00f ratios
    - AutoBlend uses and updates LastIndex as necessary */
void ModelData::Draw(MMD_t &m, const float &autoblend_max, const float &autoblend_min)
{
    switch(m.AutoBlend)
    {
        case FALSE:

            if(m.LastIndex != 0) MV1SetAttachAnimBlendRate(m.ModelH,m.LastIndex,0.0f);
            break;

        case TRUE:

            MV1SetAttachAnimBlendRate(m.ModelH,m.AttachIndex,autoblend_max-m.BlendDecay/2);
            if(m.BlendDecay > 0.00f)
            {
                MV1SetAttachAnimBlendRate(m.ModelH,m.LastIndex,((m.BlendDecay/2)+autoblend_min));
                m.BlendDecay -= 0.02f;
            }
            else MV1SetAttachAnimBlendRate(m.ModelH,m.LastIndex,0.0f);
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

            DrawBillboard3D(m.Body.Pos,m.cx,m.cy,m.Size,m.Angle,**m.SpritePtr_D,TRUE);
            break;

        case TRUE:

            DrawModiBillboard3D
            (
                m.Body.Pos,
                m.a - m.Flux, y1,
                m.b + m.Flux, y2,
                m.c + m.Flux, y3,
                m.d - m.Flux, y4,
                **m.SpritePtr_D,TRUE
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
