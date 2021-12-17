#include "DxLib.h"
#include <cstdint>

uint_fast8_t WinMode = FALSE;
int_fast16_t Width = 640, Height = 480;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
	//window init
	ChangeWindowMode(WinMode);
    SetWindowSizeChangeEnableFlag(TRUE, TRUE);
    SetWindowSizeExtendRate(1.0);
	SetMainWindowText("Test");
	SetGraphMode(Width, Height, 16);

	//check DXlibrary
	if( DxLib_Init() == -1 ) return -1;
	
	//preload setup
	SetDrawScreen(DX_SCREEN_BACK);

	//main loop
	while(ProcessMessage() >= 0) //check app's process state
	{
	    ClearDrawScreen();
		{
			//Update
			{
				//code
			}

			//Draw
			{
				LoadGraphScreen(Width/2,Height/2-20,_T("core/ph0.png"),TRUE);
			}
		}
		ScreenFlip();

		//Window Management
		if(CheckHitKey(KEY_INPUT_RALT) == 1 && CheckHitKey(KEY_INPUT_RETURN) == 1) 
		{
			if(WinMode == FALSE) WinMode = TRUE;
			else WinMode = FALSE;
			ChangeWindowMode(WinMode);
		}
		if(CheckHitKey(KEY_INPUT_ESCAPE)) break;
	}
	DxLib_End();
	return 0;					
}
