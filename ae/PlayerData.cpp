#include "DxLib.h"
#include <cstdint>
#include "PlayerData.h"

PlayerData::PlayerData()
{
    MMD.AttachIndex = 0;
    MMD.AnimSet = 0;
    MMD.AnimIndex = 0;
    MMD.Pace = 0;
    MMD.PlayTime = 0.0f;
    MMD.PlayOffset = 0.0f;
    MMD.Reverse = false, MMD.Flag = false;
}

void PlayerData::Update()
{
    //code
}

