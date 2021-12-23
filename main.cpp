#include "DxLib.h"
#include <cstdint>
#include <cmath>
#include "MousePoll.h"

typedef struct {
	VECTOR v;
	float r;
} Sphere_t;

static const float ROTATE_SPEED = DX_PI_F/45;
static const float MOVEMENT_SPEED = DX_PI_F/5;
uint_fast8_t WinMode = TRUE, NewScreen = TRUE;
int_fast16_t Width = 640, Height = 480;
MousePoll Mouse;

void Rotate(float &x, float &y, const float angle, const float tX, const float tY);
DxLib::VECTOR SetCross(const VECTOR &a, const VECTOR &b);
void SetDot(const VECTOR &a, const VECTOR &b, float &dot);

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
	SetCameraNearFar(0.1f, 1000.0f);
	NewScreen = TRUE;
	const uint_fast8_t SPHERES = 2;
	uint_fast8_t pDir = 0, AnimSet = 0, pReverse = FALSE, pJump = FALSE, pGrounded = TRUE;
	uint_fast8_t SDFlag[SPHERES] = {0};
	int_fast32_t pPace = 0;
	int ModelH = 0, EnvH1, EnvH2, SkyH, Light;
	int AttachIndex = 0;
	float TotalTime, PlayTime = 0.0f, vOffset = 0.46f;
	float anchorY = 0.0f, angleV = 0.0f, angleH = 0.0f; 
	float targetX = 0.0f, targetY = 10.0f, targetZ = 0.0f;
	VECTOR Camera, Player, pRot, cRot, S;
	Camera = VGet(0.0f, 20.0f, -20.0f);
	Player = VGet(0.0f, 0.0f, 0.0f);
	pRot = VGet(0.0f, DX_PI_F/3, 0.0f);
	cRot = VGet(0.0f, DX_PI_F/3, 0.0f);
	MV1_COLL_RESULT_POLY_DIM HitPolyDim[SPHERES];
	Sphere_t sphere[SPHERES];
	SDFlag[0] = 1, SDFlag[1] = 1;
	sphere[0].v = VGet(-0.5f,10.0f,-46.0f), sphere[0].r = 6.0f;
	sphere[1].v = VGet(-40.0f,10.0f,-60.0f), sphere[1].r = 6.0f;

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
					if(ModelH != 0) //if already set
					{
						MV1DetachAnim(ModelH,AttachIndex);
						MV1DeleteModel(ModelH);
					}
					SetMouseDispFlag(FALSE);
					SetCameraNearFar(0.1f, 1000.0f);
					SetUseZBuffer3D(TRUE);
					SetWriteZBuffer3D(TRUE);
					Light = CreatePointLightHandle(VGet(0.0f,3000.0f,0.0f),3000.0f,0.2f,0.002f,0.0f);
					SetLightPositionHandle(Light,VGet(0.0f,500.0f,0.0f));
					ModelH = MV1LoadModel(_T("dat/Lat/LatMikuVer2.3_SailorWinter.pmd"));
					AttachIndex = MV1AttachAnim(ModelH, 0, -1, FALSE);
					TotalTime = MV1GetAttachAnimTotalTime(ModelH,AttachIndex);
					MV1SetupCollInfo(ModelH, -1, 1, 1, 1);
					pPace = 0;
					NewScreen = FALSE;
				}
				
				//Controls
				{
					Mouse.Update();
					//Player
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
					//keep the camera a set distance from player
					//cam's circular anchored point around the player is controlled by cRot.y
					Camera.z = Player.z + cos(cRot.y)*40;
					Camera.x = Player.x + sin(cRot.y)*40;
					if(CheckHitKey(KEY_INPUT_J) == 1) cRot.y -= ROTATE_SPEED;
					if(CheckHitKey(KEY_INPUT_K) == 1) cRot.y += ROTATE_SPEED;
					if(Mouse.Moved())
					{
						DrawFormatString(0,60,-256,"delta_x=%d, x=%d",Mouse.GetDeltaX(),Mouse.x);
						cRot.y -= (ROTATE_SPEED*Mouse.GetDeltaX())/30;
						//vOffset += (ROTATE_SPEED*Mouse.GetDeltaY())/80;
						if(vOffset > 0.81f) vOffset = 0.81f;
						if(vOffset < -.41f) vOffset = -0.41f;
						
						Mouse.Reset(Width/2,Height/2);
					}
					if(CheckHitKey(KEY_INPUT_DOWN) == 1) Player.y -= 2; //using to test vertical lock, use jump to get back up
					if(CheckHitKey(KEY_INPUT_LEFT) == 1) angleV += ROTATE_SPEED;
					if(CheckHitKey(KEY_INPUT_RIGHT) ==1) angleV -= ROTATE_SPEED;
					
					//horizontal lock formula
					S.x = Player.x - Camera.x;
					S.y = Player.y - Camera.y; 
					S.z = Player.z - Camera.z;
					angleH = atan2f(S.x,S.z); 

					//vertical lock formula
					S.x = Camera.x - Player.x;
					S.y = Camera.y - Player.y; 
					S.z = Camera.z - Player.z;
					angleV = (atan2f(S.y, sqrtf(S.x*S.x + S.z*S.z)) - vOffset);
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

				//Collision
				MV1RefreshCollInfo(ModelH,-1);
				for(uint_fast8_t i = 0; i < SPHERES; i++)
				{
					HitPolyDim[i] = MV1CollCheck_Sphere(ModelH, -1, sphere[i].v, sphere[i].r);
					if(HitPolyDim[i].HitNum >= 1)
					{
						SDFlag[i] = 0;
					}
					MV1CollResultPolyDimTerminate(HitPolyDim[i]);
				}
				
			}

			//Draw
			{
				for(uint_fast8_t i = 0; i < SPHERES; i++)
				{
					if(SDFlag[i] == 1)
					{
						DrawSphere3D(sphere[i].v, sphere[i].r, 200, GetColor(200,0,120), GetColor(100,0,60), TRUE);
					}
				}
				SetGlobalAmbientLight(GetColorF(0.0f,0.2f,0.0f,0.0f));
				DrawCone3D(VGet(-10.0f,-30.0f,10.0f),VGet(-10.0f,-40.0f,10.0f),1000.0f,32,GetColor(40,30,70),GetColor(90,150,120),TRUE);
				MV1DrawModel(ModelH);
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

void Rotate(float &x, float &y, const float angle, const float tX, const float tY)
{
	float ox = x-tX;
	float oy = y-tY;
	x = ox * cos(angle) + oy * sin(angle);
	y = ox * sin(angle) + oy * cos(angle);
	x += tX;
	y += tY;
}

DxLib::VECTOR SetCross(const VECTOR &a, const VECTOR &b)
{
	VECTOR c;
	c.x = (a.y * b.z - a.z * b.y);
	c.y = (a.z * b.x - a.x * b.z);
	c.z = (a.x * b.y - a.y * b.x);
	return c;
}

float SetDot(const VECTOR &a, const VECTOR &b)
{
	float dot;
	dot = (a.x * b.x + a.y * b.y + a.z * b.z);
	return dot;
}
