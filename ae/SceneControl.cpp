#include "DxLib.h"
#include "SceneControl.h"
#include "SceneVirt.h"
#include "NoScene.h"
#include "DebugScene.h"

NoScene noscene;
DebugScene debugscene;
SceneVirt *sPtr;

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

void SceneControl::Init()
{
    //code
}

void SceneControl::End()
{
    //code
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