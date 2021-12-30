#include "DxLib.h"
#include <cstdint>
#include "ModelData.h"

ModelData::ModelData(float rate)
:play_rate(rate)
{
    //code
}

void ModelData::Update(MMD_t &m)
{
    //Check and Update
    if(m.AnimSet != m.AnimIndex)
    {
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

    //Update the playback time point
    MV1SetAttachAnimTime(m.ModelH,m.AttachIndex,m.PlayTime); 

    //Update spatial data
    MV1SetPosition(m.ModelH,VGet(m.Pos.x,m.Pos.y,m.Pos.z));
    MV1SetRotationXYZ(m.ModelH,VGet(m.Rot.x,(m.Rot.y+m.RotOffset.y),m.Rot.z));
    MV1RefreshCollInfo(m.ModelH,-1);
}

void ModelData::Update(X_t &m)
{
    //Check and Update
    if(m.AnimSet != m.AnimIndex)
    {
        MV1DetachAnim(m.ModelH,m.AttachIndex);
        m.AttachIndex = MV1AttachAnim(m.ModelH, m.AnimIndex, -1, FALSE);
        m.TotalTime = MV1GetAttachAnimTotalTime(m.ModelH,m.AttachIndex);
        m.PlayTime = 0.0f;
        m.AnimSet = m.AnimIndex; 
    }
}

void ModelData::Update(MQO_t &m)
{
    //code
}

void ModelData::Draw(MMD_t &m, float rate)
{
    MV1SetAttachAnimBlendRate(m.ModelH,m.AttachIndex,rate);
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
