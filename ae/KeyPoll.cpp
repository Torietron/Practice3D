#include "DxLib.h"
#include "KeyPoll.h"
#include <cstdint>

KeyPoll::KeyPoll()
{
	for(uint_fast16_t i = 0; i < 256; i++)
	{
		Poll[i] = 0;
	}  
}

int KeyPoll::Update()
{
	char tempKey[256];
	GetHitKeyStateAll(tempKey);
	for(uint_fast16_t i = 0; i < 256; i++){
		if(tempKey[i] != 0){
			Poll[i]++;
		}
		else {
			Poll[i] = 0;
		}
	}
	return 0;
}