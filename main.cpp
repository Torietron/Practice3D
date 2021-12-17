#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cmath>

static const float ROTATE_SPEED = DX_PI_F/90;
uint_fast8_t WinMode = TRUE, NewScreen = TRUE;
int_fast16_t Width = 640, Height = 480;

void rotate(float &x, float &y, const float angle, const float mikuX, const float mikuY);

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
	int ModelH;
	NewScreen = TRUE;
	
	float cameraX = 0, cameraZ =-20;
	const float targetX = 0, targetZ = 0;

	//main loop
	while(ProcessMessage() >= 0) //check app's process state
	{
	    ClearDrawScreen();
		{
			//Update
			{
				if(NewScreen == TRUE)
				{
					ModelH = MV1LoadModel("dat/Lat/LatMikuVer2.3_Normal.pmd");
					SetCameraNearFar(0.1f, 1000.0f);
					SetCameraPositionAndTarget_UpVecY(VGet(0, 10, -20),VGet(0.0f, 10.0f, 0.0f));
					NewScreen = FALSE;
				}

				if(CheckHitKey(KEY_INPUT_LEFT) == 1) rotate(cameraX, cameraZ, +ROTATE_SPEED, targetX, targetZ);
				if(CheckHitKey(KEY_INPUT_RIGHT) == 1) rotate(cameraX, cameraZ, -ROTATE_SPEED, targetX, targetZ);
				SetCameraPositionAndTarget_UpVecY(VGet(cameraX, 10, cameraZ), VGet(targetX, 10.0f, targetZ));
			}

			//Draw
			{
				MV1DrawModel(ModelH);
			}
		}
		ScreenFlip();

		//Window Management
		if(CheckHitKey(KEY_INPUT_RALT) == 1 && CheckHitKey(KEY_INPUT_RETURN) == 1) 
		{
			if(WinMode == FALSE) WinMode = TRUE;
			else WinMode = FALSE;
			ChangeWindowMode(WinMode);
			NewScreen = TRUE;
		}
		if(CheckHitKey(KEY_INPUT_ESCAPE)) break;
	}
	DxLib_End();
	return 0;					
}

void rotate(float &x, float &y, const float angle, const float tX, const float tY)
{
	float ox = x-tX;
	float oy = y-tY;
	x = ox * cos(angle) + oy * sin(angle);
	y = ox * sin(angle) + oy * cos(angle);
	x += tX;
	y += tY;
}
