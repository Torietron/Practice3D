#include "DxLib.h"
#include "SceneControl.h"
#include "DebugScene.h"
#include "KeyPoll.h"
#include "Interface.h"

Interface Ui;
InterfaceOptions_t Options[3] = {
    {50,50,"button1",Ui.Yellow},
    {50,70,"button2",Ui.Yellow},
    {50,90,"button3",Ui.Yellow},
};

extern SceneControl Scene;
extern KeyPoll Key;

void DebugScene::Init()
{
    //code
}

void DebugScene::Load()
{
    //code
}

void DebugScene::End()
{
    //code
}

void DebugScene::Update()
{
    Ui.UpdateMenu(3);
    Ui.UpdateMenu(3,KEY_INPUT_W,KEY_INPUT_S);
}

void DebugScene::Draw()
{
    Ui.DrawMenu(Options,3,Ui.Amber);
    Ui.DrawValue(0,190,Ui.Amber);
}