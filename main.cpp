#include "DxLib.h"
#include <cstdint>
#include <cmath>
#include "MousePoll.h"
#include "KeyPoll.h"
#include "SceneControl.h"
#include "ScreenControl.h"
#include "ModelData.h"
#include "PlayerData.h"
#include "EnemyData.h"

bool debugflag = true;
ScreenControl Screen(640,480);
SceneControl Scene(PRACTICE_SCENE);
MousePoll Mouse;
KeyPoll Key;
ModelData Models;
PlayerData Player;
EnemyData Enemy;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
	//Window Init
	SetMainWindowText(_T("Test"));
	if(debugflag) Screen.ShowFPS = TRUE;
	Screen.C3D.Enabled = TRUE;
	Screen.Init();
	
	//Systems Check and Load
	if(DxLib_Init() == -1 || Scene.Init() == -1) return -1;

	//Main Loop
	while(ProcessMessage() >= 0) //Check app's process state
	{
	    ClearDrawScreen();
		{
			{//Update
				if(Screen.ShowFPS) Screen.CountFPS();
				if(Screen.Update() == 1) Scene.Load(); //Reload on mode change
				Scene.Update();
			}
			
			{//Draw
				Scene.Draw();
				if(Screen.ShowFPS) Screen.DrawFPS();
				if(Screen.Cursor) Screen.DrawCursor();
			}
		}
		ScreenFlip();

		if(Screen.LimitFPS) Screen.Wait(); //Quickly adjustable limit
		if(Key.Poll[KEY_INPUT_ESCAPE] == 1) break;
	}

	Scene.End();
	Screen.End();
	return 0;					
}
