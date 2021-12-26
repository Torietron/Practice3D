#include "DxLib.h"
#include <cstdint>
#include <cmath>
#include "MousePoll.h"
#include "KeyPoll.h"
#include "SceneControl.h"

bool debugflag = true;
uint_fast8_t WinMode = FALSE, NewScreen = TRUE;
int_fast16_t Width = 640, Height = 480;

MousePoll Mouse;
KeyPoll Key;
SceneControl Scene(PRACTICE_SCENE);
//FpsControl Fps;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
	//Window Init
	ChangeWindowMode(WinMode);
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);
	SetWindowSizeExtendRate(1.0);
	SetMainWindowText(_T("Test"));
	SetGraphMode(Width, Height, 16);

	//Systems Check
	if(DxLib_Init() == -1 || Scene.Init() == -1) return -1;
	
	//Preload Setup
	SetDrawScreen(DX_SCREEN_BACK);
	SetCameraNearFar(0.1f, 1000.0f);
	NewScreen = TRUE;

	//Main Loop
	while(ProcessMessage() >= 0) //Check app's process state
	{
	    ClearDrawScreen();
		{
			//Update
			{
				//if(debugflag) Fps.Update();
				Scene.Update();
			}
			
			//Draw
			{
				//if(debugflag) Fps.Draw();
				Scene.Draw();
			}
			
		}
		ScreenFlip();

		//Window Management
		if(Key.Poll[KEY_INPUT_RALT] == 1 && Key.Poll[KEY_INPUT_RETURN] == 1) 
		{
			if(WinMode == FALSE) WinMode = TRUE;
			else WinMode = FALSE;
			NewScreen = TRUE;
		}
		if(Key.Poll[KEY_INPUT_ESCAPE] == 1) break;
	}
	Scene.End();
	DxLib_End();
	return 0;					
}
