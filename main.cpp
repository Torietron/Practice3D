#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cmath>

static const float ROTATE_SPEED = DX_PI_F/45;
static const float MOVEMENT_SPEED = DX_PI_F/10;
uint_fast8_t WinMode = TRUE, NewScreen = TRUE;
int_fast16_t Width = 640, Height = 480;

void rotate(float &x, float &y, const float angle, const float tX, const float tY);

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
	//Window Init
	ChangeWindowMode(WinMode);
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);
	SetWindowSizeExtendRate(1.0);
	SetMainWindowText(_T("Test"));
	SetGraphMode(Width, Height, 16);

	//Check DXlibrary
	if( DxLib_Init() == -1 ) return -1;
	
	//Preload Setup
	SetDrawScreen(DX_SCREEN_BACK);
	NewScreen = TRUE;
	uint_fast8_t pDir = 0, AnimSet = 0, pReverse = FALSE, pJump = FALSE, pGrounded = TRUE;
	int_fast32_t pPace = 0;
	int ModelH;
	int AttachIndex;
	float TotalTime, PlayTime = 0.0f;
	float playerX = 0.0f, playerY = 0.0f, playerZ = 0.0f, anchorY = 0.0f; 
	float rotX = 0.0f, rotY = DX_PI_F/3, rotZ = 0.0f;
	float cameraX = 0.0f, cameraY = 10.0f, cameraZ =-20.0f;
	const float targetX = 0.0f, targetY = 10.0f, targetZ = 0.0f;

	//Main Loop
	while(ProcessMessage() >= 0) //Check app's process state
	{
	    ClearDrawScreen();
		{
			//Update
			{
				//On screen event
				if(NewScreen == TRUE)
				{
					ModelH = MV1LoadModel(_T("dat/Lat/LatMikuVer2.3_Normal.pmd"));
					SetCameraNearFar(0.1f, 1000.0f);
					SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 10.0f, -20.0f),VGet(0.0f, 10.0f, 0.0f));
					AttachIndex = MV1AttachAnim(ModelH, 0, -1, FALSE);
					TotalTime = MV1GetAttachAnimTotalTime(ModelH,AttachIndex);
					NewScreen = FALSE;
				}
				
				//Controls
				{
					if(CheckHitKey(KEY_INPUT_A) == 1) rotY -= ROTATE_SPEED;
					if(CheckHitKey(KEY_INPUT_D) == 1) rotY += ROTATE_SPEED;
					if(CheckHitKey(KEY_INPUT_W) == 1) 
					{
						playerZ -= cos(rotY)*MOVEMENT_SPEED;
						playerX -= sin(rotY)*MOVEMENT_SPEED;
						rotX = DX_PI_F*2;
						if(pGrounded == TRUE) pDir = 1;
						else pDir = 2;
						pReverse = FALSE;
					}
					if(CheckHitKey(KEY_INPUT_S) == 1) 
					{
						playerZ += cos(rotY)*(MOVEMENT_SPEED*0.75f);
						playerX += sin(rotY)*(MOVEMENT_SPEED*0.75f);
						rotX = DX_PI_F/10; //Center of gravity would've been visually inconsistent
						if(pGrounded == TRUE) pDir = 1;
						else pDir = 2;
						pReverse = TRUE;
					}
					if(CheckHitKey(KEY_INPUT_SPACE) == 1)
					{
						if(pGrounded == TRUE) 
						{
							anchorY = playerY;
							pPace = 0;
							pJump = TRUE;
							pGrounded = FALSE;
							rotX = DX_PI_F*2;
							pDir = 2;
						}
					}
					if(CheckHitKey(KEY_INPUT_W) == 0 && CheckHitKey(KEY_INPUT_S) == 0)
					{
						if(pGrounded == TRUE) pDir = 0, rotX = DX_PI_F*2, pReverse = FALSE;
						if(pGrounded == FALSE) pDir = 2, rotX = DX_PI_F*2, pReverse = FALSE;
					}
				}
				
				//Simple Jump
				if(pJump == TRUE && pGrounded == FALSE)
				{
					playerY = anchorY + sin(DX_PI_F*2/90*pPace)*17;
					pPace++, rotX = DX_PI_F*2;
					if(playerY <= 0.3f && pPace > 4) pJump = FALSE, pGrounded = TRUE, playerY = 0.0f;
				}

				//3DAnimation Handler
				{
					//Check&Update
					if(AnimSet != pDir)
					{
						MV1DetachAnim(ModelH,AttachIndex);
						AttachIndex = MV1AttachAnim(ModelH, pDir, -1, FALSE);
						TotalTime = MV1GetAttachAnimTotalTime(ModelH,AttachIndex);
						PlayTime = 0.0f;
						AnimSet = pDir; 
					}

					//Advance frame time
					if(pReverse == TRUE)
					{
						PlayTime -= 0.31f;
						if(PlayTime <= 0) PlayTime = TotalTime;
					}
					else
					{
						PlayTime += 0.39f;
						if(PlayTime >= TotalTime) PlayTime = 0.0f;
					}

					//Update active frame
					MV1SetAttachAnimTime(ModelH,AttachIndex,PlayTime); 
				}

				//Update ModelH position/rotation
				MV1SetPosition(ModelH,VGet(playerX,playerY,playerZ));
				MV1SetRotationXYZ(ModelH,VGet(rotX,rotY,0.0f));

				//Update Camera
				//SetCameraPositionAndTarget_UpVecY(VGet(cameraX, cameraY, cameraZ), VGet(targetX, targetY, targetZ));
			}

			//Draw
			{
				MV1DrawModel(ModelH);
				DrawFormatString(0,20,GetColor(255,255,255),"x=%.1f y=%.1f z=%.1f",playerX,playerY,playerZ);
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
