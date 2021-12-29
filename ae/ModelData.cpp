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

    Animate(m);
}

void ModelData::SetPlayRate(float a)
{
    play_rate = a;
}

float ModelData::GetPlayRate()
{
    return play_rate;
}

void ModelData::Animate(MMD_t &m)
{
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
}
