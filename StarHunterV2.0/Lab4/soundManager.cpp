#include "soundManager.h"


soundManager::soundManager(void)
{
	if (!BASS_Init(-1, 44100, BASS_DEVICE_NOTHREAD, NULL))
	{
		MessageBox(NULL,"Couldn't initialize Bass.dll.","Bugger!",MB_OK|MB_ICONEXCLAMATION);
	}else{
		std::cout << "Bass.dll loaded correctly!" << std::endl;
	}

	deathHSPLD = false;
	winHSPLD = false;

	LPSTR m = ".\\music.mp3\0";
	music = BASS_SampleLoad(false,m,0,0,1,0);
	if (!music)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	m=".\\leftfoot.wav\0";
	footleft = BASS_SampleLoad(false,m,0,0,1,0);
	if (!footleft)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	m=".\\rightfoot.wav\0";
	footright = BASS_SampleLoad(false,m,0,0,1,0);
	if (!footright)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	m=".\\heart.mp3\0";
	heart= BASS_SampleLoad(false,m,0,0,1,0);
	if (!heart)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	m=".\\star.wav\0";
	starPing = BASS_SampleLoad(false,m,0,0,1,0);
	if (!starPing)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	m=".\\doh1.wav\0";
	pain = BASS_SampleLoad(false,m,0,0,1,0);
	if (!pain)
		MessageBox(NULL, "Couldn't load sound file.", m , MB_OK|MB_ICONEXCLAMATION);
	
	m=".\\loading.mp3";
	loading = BASS_SampleLoad(false,m,0,0,1,0);
	if (!loading)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	m=".\\death.mp3";
	death = BASS_SampleLoad(false,m,0,0,1,0);
	if (!death)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	m=".\\win.mp3";
	win = BASS_SampleLoad(false,m,0,0,1,0);
	if (!win)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	BASS_Start();
	
}

void soundManager::play(int sound,int loop){
	switch(sound){
	case 0:
		BASS_SamplePlayEx(loading,(DWORD)0,-1,50,-1,loop);
		break;
	case 1:
		BASS_SamplePlayEx(music,(DWORD)0,-1,30,-1,loop);
		break;
	case 2:
		BASS_SamplePlayEx(footleft,(DWORD)0,-1,60,-1,loop);
		break;
	case 3:
		BASS_SamplePlayEx(footright,(DWORD)0,-1,60,-1,loop);
		break;
	case 4:
		BASS_SamplePlayEx(heart,(DWORD)0,-1,50,-1,loop);
		break;
	case 5:
		BASS_SamplePlayEx(starPing,(DWORD)0,-1,50,-1,loop);
		break;
	case 6:
		BASS_SamplePlayEx(pain,(DWORD)0,-1,45,-1,loop);
		break;
	case 7:
		if(!deathHSPLD){
			BASS_SamplePlayEx(death,(DWORD)0,-1,50,-1,loop);
			deathHSPLD=true;
		}else{
		break;
		}
		break;
	case 8:
		if(!winHSPLD){
			BASS_SamplePlayEx(win,(DWORD)0,-1,50,-1,loop);
			winHSPLD=true;
		}else{
		break;
		}
		break;
	default:
		break;
	}
}
void soundManager::stop(int sound){

switch(sound){
	case 0:
		BASS_SampleStop(loading);
		break;
	case 1:
		BASS_SampleStop(music);
		break;
	case 2:
		BASS_SampleStop(footleft);
		break;
	case 3:
		BASS_SampleStop(footright);
		break;
	case 4:
		BASS_SampleStop(heart);
		break;
	case 5:
		BASS_SampleStop(starPing);
		break;
	case 6:
		BASS_SampleStop(pain);
		break;
	case 7:
		BASS_SampleStop(death);
		break;
	case 8:
		BASS_SampleStop(win);
		break;
	default:
		break;
	}

}
soundManager::~soundManager(void)
{
	BASS_SampleStop(loading);
	BASS_SampleStop(music);
	BASS_SampleStop(footleft);
	BASS_SampleStop(footright);
	BASS_SampleStop(heart);
	BASS_SampleStop(starPing);
	BASS_SampleStop(pain);
	BASS_SampleStop(death);
	BASS_SampleStop(win);
	BASS_Stop();
	std::cout << "Sound Stopped..." << std::endl;
	BASS_Free();
	std::cout << "Sound Memory Freed" << std::endl;
}
