#include "DxLib.h"
#include <cstdint>
#include "ModelData.h"

ModelData::ModelData(float rate)
:play_rate(rate)
{
}

void ModelData::Update(MMD_t &m)
{
    //Check and Update
    if(m.AnimSet != m.AnimIndex)
    {

        m.BlendDecay = 0.20f;
        MV1DetachAnim(m.ModelH,m.LastIndex);
        MV1DetachAnim(m.ModelH,m.AttachIndex);
        m.LastIndex = MV1AttachAnim(m.ModelH, m.AnimSet, -1, FALSE);
        m.LastPlayTime = m.PlayTime;
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

    //Update the playback time point
    MV1SetAttachAnimTime(m.ModelH,m.AttachIndex,m.PlayTime); 
    MV1SetAttachAnimTime(m.ModelH,m.LastIndex,m.LastPlayTime); 

    //Update spatial data
    MV1SetPosition(m.ModelH,m.Body.Pos);
    MV1SetRotationXYZ(m.ModelH,VGet(m.Rot.x,(m.Rot.y+m.RotOffset.y),m.Rot.z));
    MV1RefreshCollInfo(m.ModelH,-1);
}

void ModelData::Update(MQO_t &m)
{
    MV1SetPosition(m.ModelH,m.Pos);
    MV1SetRotationXYZ(m.ModelH,m.Rot);
    MV1RefreshCollInfo(m.ModelH,-1);
}

//Blend using the animation in BlendIndex
void ModelData::ManualBlend(MMD_t &m, const float &blendRate)
{
    MV1SetAttachAnimBlendRate(m.ModelH,m.BlendIndex,blendRate);
}

//Set AutoBlend to TRUE for smoother transistions
void ModelData::Draw(MMD_t &m, const float &blendRate1, const float &blendRate2)
{
    if(m.AutoBlend == TRUE)
    {
        MV1SetAttachAnimBlendRate(m.ModelH,m.AttachIndex,blendRate1-m.BlendDecay/2);
        if(m.BlendDecay > 0.00f)
        {
            MV1SetAttachAnimBlendRate(m.ModelH,m.LastIndex,((m.BlendDecay/2)+blendRate2));
            m.BlendDecay -= 0.02f;
        }
        else MV1SetAttachAnimBlendRate(m.ModelH,m.IdleIndex,blendRate2);
    }
    MV1DrawModel(m.ModelH);
}

void ModelData::Draw(const MQO_t &m)
{
    MV1DrawModel(m.ModelH);
}

void ModelData::SetPlayRate(float a)
{
    play_rate = a;
}

float ModelData::GetPlayRate()
{
    return play_rate;
}
