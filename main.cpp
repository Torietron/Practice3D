#include "DxLib.h"
#include <cstdint>
#include <cmath>
#include "MousePoll.h"
#include "KeyPoll.h"
#include "SceneControl.h"
#include "ScreenControl.h"

bool debugflag = true;

ScreenControl Screen(640,480);
SceneControl Scene(PRACTICE_SCENE);
MousePoll Mouse;
KeyPoll Key;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
	//Window Init
	SetMainWindowText(_T("Test"));
	Screen.Init();

	//Systems Check and Load
	if(DxLib_Init() == -1 || Scene.Init() == -1) return -1;

	//Main Loop
	while(ProcessMessage() >= 0) //Check app's process state
	{
	    ClearDrawScreen();
		{
			{//Update
				if(debugflag) Screen.CountFps();
				Scene.Update();
			}
			
			{//Draw
				if(debugflag) Screen.DrawFps();
				Scene.Draw();
			}
		}
		ScreenFlip();

		if(Screen.Lock == TRUE) Screen.Wait();
		if(Key.Poll[KEY_INPUT_ESCAPE] == 1) break;
	}

	Scene.End();
	Screen.End();
	return 0;					
}
