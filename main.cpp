#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cmath>

static const float ROTATE_SPEED = DX_PI_F/45;
static const float MOVEMENT_SPEED = DX_PI_F/10;
uint_fast8_t WinMode = TRUE, NewScreen = TRUE;
int_fast16_t Width = 640, Height = 480;

void pRotate(float &x, float &y, const float angle, const float tX, const float tY);

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
	int_fast16_t tempX, tempY;
	int_fast32_t pPace = 0;
	int ModelH, EnvH1, EnvH2, SkyH;
	int AttachIndex;
	float TotalTime, PlayTime = 0.0f;
	float anchorY = 0.0f, angleV, angleH; 
	float targetX = 0.0f, targetY = 10.0f, targetZ = 0.0f;
	VECTOR Camera, Player, pRot, cRot, S;
	Camera = VGet(0.0f, 20.0f, -20.0f);
	Player = VGet(0.0f, 0.0f, 0.0f);
	pRot = VGet(0.0f, DX_PI_F/3, 0.0f);
	cRot = VGet(0.0f, DX_PI_F/3, 0.0f);
	SetMousePoint((int)Player.x,(int)Player.z);

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
					EnvH1 = MV1LoadModel(_T("dat/batokin-island/batokin_island5.mqo"));
					//EnvH2 = MV1LoadModel(_T("dat/batokin-island/batokin_island5.x"));
					SkyH = MV1LoadModel(_T("dat/batokin-island/skydome.x"));
					SetCameraNearFar(0.1f, 1000.0f);
					SetCameraPositionAndTarget_UpVecY(VGet(Camera.x, Camera.y, Camera.z),VGet(targetX, targetY, targetZ));
					AttachIndex = MV1AttachAnim(ModelH, 0, -1, FALSE);
					TotalTime = MV1GetAttachAnimTotalTime(ModelH,AttachIndex);
					NewScreen = FALSE;
				}
				
				//Controls
				{
					if(CheckHitKey(KEY_INPUT_A) == 1) pRot.y -= ROTATE_SPEED;
					if(CheckHitKey(KEY_INPUT_D) == 1) pRot.y += ROTATE_SPEED;
					if(CheckHitKey(KEY_INPUT_W) == 1) 
					{
						Player.z -= cos(pRot.y)*MOVEMENT_SPEED;
						Player.x -= sin(pRot.y)*MOVEMENT_SPEED;
						pRot.x = DX_PI_F*2;
						if(pGrounded == TRUE) pDir = 1;
						else pDir = 2;
						pReverse = FALSE;
					}
					if(CheckHitKey(KEY_INPUT_S) == 1) 
					{
						Player.z += cos(pRot.y)*(MOVEMENT_SPEED*0.75f);
						Player.x += sin(pRot.y)*(MOVEMENT_SPEED*0.75f);
						pRot.x = DX_PI_F/10; //Center of gravity would've been visually inconsistent
						if(pGrounded == TRUE) pDir = 1;
						else pDir = 2;
						pReverse = TRUE;
					}
					if(CheckHitKey(KEY_INPUT_SPACE) == 1)
					{
						if(pGrounded == TRUE) 
						{
							anchorY = Player.y;
							pPace = 0;
							pJump = TRUE;
							pGrounded = FALSE;
							pRot.x = DX_PI_F*2;
							pDir = 2;
						}
					}
					if(CheckHitKey(KEY_INPUT_W) == 0 && CheckHitKey(KEY_INPUT_S) == 0)
					{
						if(pGrounded == TRUE) pDir = 0, pRot.x = DX_PI_F*2, pReverse = FALSE;
						if(pGrounded == FALSE) pDir = 2, pRot.x = DX_PI_F*2, pReverse = FALSE;
					}

					//Camera
					Camera.z = Player.z + cos(cRot.y)*40;
					Camera.x = Player.x + sin(cRot.y)*40;
					if(CheckHitKey(KEY_INPUT_J) == 1) cRot.y -= ROTATE_SPEED;
					if(CheckHitKey(KEY_INPUT_K) == 1) cRot.y += ROTATE_SPEED;
					
					S.x = Player.x - Camera.x;
					S.y = Player.y - Camera.y; 
					S.z = Player.z - Camera.z;

					angleH = atan2f(S.x,S.z);
					//angleV = atan2f(S.x,S.y);

					angleV = ((atan2f(Camera.y,Player.y)) * 0.25) - .39;
					tempY = atan2f(S.z,S.y);
				}
				
				//Simple Jump
				if(pJump == TRUE && pGrounded == FALSE)
				{
					Player.y = anchorY + sin(DX_PI_F*2/90*pPace)*17;
					pPace++, pRot.x = DX_PI_F*2;
					if(Player.y <= 0.3f && pPace > 4) pJump = FALSE, pGrounded = TRUE, Player.y = 0.0f;
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

				//Update ModelH position/pRotation
				MV1SetPosition(ModelH,VGet(Player.x,Player.y,Player.z));
				MV1SetRotationXYZ(ModelH,VGet(pRot.x,pRot.y,pRot.z));

				//Update Camera
				SetCameraPositionAndAngle(Camera,angleV,angleH,0.0f);
			}

			//Draw
			{
				MV1DrawModel(ModelH);
				MV1DrawModel(SkyH);
				MV1DrawModel(EnvH1);
				//MV1DrawModel(EnvH2);
				DrawFormatString(0,20,GetColor(255,255,255),"x=%.1f y=%.1f z=%.1f",Player.x,Player.y,Player.z);
				DrawFormatString(0,40,GetColor(255,255,255),"angleV=%.2f, angleH=%.2f",angleV, angleH);
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

void pRotate(float &x, float &y, const float angle, const float tX, const float tY)
{
	float ox = x-tX;
	float oy = y-tY;
	x = ox * cos(angle) + oy * sin(angle);
	y = ox * sin(angle) + oy * cos(angle);
	x += tX;
	y += tY;
}
