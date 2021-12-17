#include "DxLib.h"
#include <cstdint>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{

	SetGraphMode(640, 480, 16);

	if( DxLib_Init() == -1 ) return -1;
	
	//preload setup
	SetDrawScreen(DX_SCREEN_BACK);

	while(1)
	{
	    ClearDrawScreen();
		{

		}
		ScreenFlip();
		if(ProcessMessage() < 0 ) break;
		if(CheckHitKey(KEY_INPUT_ESCAPE)) break;
	}
	DxLib_End();
	return 0;					
}
