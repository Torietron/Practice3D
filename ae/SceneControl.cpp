#include "DxLib.h"
#include "SceneControl.h"
#include "NoScene.h"
#include "DebugScene.h"
#include "PracticeScene.h"

NoScene noscene;
DebugScene debugscene;
PracticeScene practicescene;

SceneControl::SceneControl(uint_fast8_t startup)
:nScene(startup)
{
    lScene = NO_SCENE;
    sPtr = &noscene;
}

void SceneControl::Change(uint_fast8_t nextscene)
{
    nScene = nextscene;
}

int SceneControl::Init()
{
    if(SetDrawScreen(DX_SCREEN_BACK) != 0 || ScreenFlip() != 0 
    || ProcessMessage() != 0 || ClearDrawScreen() != 0) return -1;
    else return 0;
}

int SceneControl::End()
{
    sPtr->End();
    return 0;
}

int SceneControl::Update()
{
    if(this->lScene != this->nScene)
    {
        sPtr->End();
        switch(this->nScene)
        {
            case DEBUG_SCENE:
                sPtr = &debugscene;
                break;
            case PRACTICE_SCENE:
                sPtr = &practicescene;
                break;
        }
        this->lScene = this->nScene;
        sPtr->Init();
    }
    sPtr->Update();
    return 0;
}

int SceneControl::Draw()
{
    sPtr->Draw();
    return 0;
}