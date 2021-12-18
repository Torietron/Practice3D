#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cmath>

static const float ROTATE_SPEED = DX_PI_F/90;
static const float MOVEMENT_SPEED = DX_PI_F/50;
uint_fast8_t WinMode = TRUE, NewScreen = TRUE;
int_fast16_t Width = 640, Height = 480;

void rotate(float &x, float &y, const float angle, const float tX, const float tY);

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
	NewScreen = TRUE;
	uint_fast8_t pDir = 0, AnimSet = 0;
	int ModelH;
	int AttachIndex;
	float TotalTime, PlayTime = 0.0f;
	float playerX = 0.0f, playerY = 0.0f, playerZ = 0.0f, rotX = 0.0f, rotY = DX_PI_F/3, rotZ = 0.0f;
	float cameraX = 0.0f, cameraY = 10.0f, cameraZ =-20.0f;
	const float targetX = 0.0f, targetY = 10.0f, targetZ = 0.0f;

	//main loop
	while(ProcessMessage() >= 0) //check app's process state
	{
	    ClearDrawScreen();
		{
			//Update
			{
				//on screen event
				if(NewScreen == TRUE)
				{
					ModelH = MV1LoadModel("dat/Lat/LatMikuVer2.3_Normal.pmd");
					SetCameraNearFar(0.1f, 1000.0f);
					SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 10.0f, -20.0f),VGet(0.0f, 10.0f, 0.0f));
					AttachIndex = MV1AttachAnim(ModelH, 0, -1, FALSE);
					TotalTime = MV1GetAttachAnimTotalTime(ModelH,AttachIndex);
					NewScreen = FALSE;
				}

				//frame
				PlayTime += 0.27f;
				if(PlayTime >= TotalTime) PlayTime = 0.0f;
				
				//controls
				if(CheckHitKey(KEY_INPUT_A) == 1) rotY -= ROTATE_SPEED;
				if(CheckHitKey(KEY_INPUT_D) == 1) rotY += ROTATE_SPEED;
				if(CheckHitKey(KEY_INPUT_W) == 1) 
				{
				playerZ -= cos(rotY)*MOVEMENT_SPEED;
				playerX -= sin(rotY)*MOVEMENT_SPEED;
				//pDir = 1;
				}
				if(CheckHitKey(KEY_INPUT_S) == 1) 
				{
				playerZ += cos(rotY)*MOVEMENT_SPEED;
				playerX += sin(rotY)*MOVEMENT_SPEED;
				//pDir = 1;
				}
				//if(CheckHitKey(KEY_INPUT_W) == 0 && CheckHitKey(KEY_INPUT_S) == 0) pDir = 0;
				//if(CheckHitKey(KEY_INPUT_SPACE) == 1) pDir = 0;
				
				if(AnimSet != pDir)
				{
					AttachIndex = MV1AttachAnim(ModelH, pDir, -1, FALSE);
					TotalTime = MV1GetAttachAnimTotalTime(ModelH,AttachIndex);
					PlayTime = 0.0f;
					AnimSet = pDir; 
				}
				MV1SetAttachAnimTime(ModelH,AttachIndex,PlayTime);
				MV1SetPosition(ModelH,VGet(playerX,playerY,playerZ));
				MV1SetRotationXYZ(ModelH,VGet(0.0f,rotY,0.0f));
				//SetCameraPositionAndTarget_UpVecY(VGet(cameraX, cameraY, cameraZ), VGet(targetX, targetY, targetZ));
				DrawFormatString(0,20,GetColor(255,255,255),"x=%.1f y=%.1f z=%.1f",playerX,playerY,playerZ);
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
