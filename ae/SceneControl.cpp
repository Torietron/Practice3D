#include "DxLib.h"
#include "SceneControl.h"
#include "NoScene.h"
#include "DebugScene.h"
#include "PracticeScene.h"

NoScene noscene;
DebugScene debugscene;
PracticeScene practicescene;

SceneControl::SceneControl(uint_least8_t startup)
:nScene(startup)
{
    lScene = NO_SCENE;
    sPtr = &noscene;
}

void SceneControl::Change(uint_least8_t nextscene)
{
    nScene = nextscene;
}

int SceneControl::Init()
{
    if(SetDrawScreen(DX_SCREEN_BACK) != 0 || ClearDrawScreen() != 0
    || ProcessMessage() != 0 || ScreenFlip() != 0 ) return -1;
    else return 0;
}

int SceneControl::Load()
{
    sPtr->Load();
    return 0;
}

int SceneControl::End()
{
    sPtr->End();
    return 0;
}

//Return 1 on scene change
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
            default:
                sPtr = &noscene;
                break;
        }
        this->lScene = this->nScene;
        sPtr->Init();
        sPtr->Load();
        sPtr->Update();

        if(sPtr == &noscene)
        {
            MessageBox
            (
                NULL,
                TEXT("Scene Error."),
                TEXT("Error"),
                MB_OK | MB_ICONERROR 
            );
            return -1;
        }
        else return 1;
    }
    sPtr->Update();
    return 0;
}

int SceneControl::Draw()
{
    sPtr->Draw();
    return 0;
}