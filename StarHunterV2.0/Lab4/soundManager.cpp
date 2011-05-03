#include "soundManager.h"


SoundManager::SoundManager(void)
{
	if (!BASS_Init(-1, 44100, BASS_DEVICE_NOTHREAD, NULL))
	{
		MessageBox(NULL,"Couldn't initialize Bass.dll.","Bugger!",MB_OK|MB_ICONEXCLAMATION);
	}else{
		std::cout << "Bass.dll loaded correctly!" << std::endl;
	}

	deathHSPLD = false;
	winHSPLD = false;

	LPSTR m;
	m = ".\\Sounds\\mainmenu.mp3\0";
	menumusic = BASS_SampleLoad(false,m,0,0,1,0);
	if (!menumusic)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	m = ".\\Sounds\\menuselect.wav\0";
	menuselect = BASS_SampleLoad(false,m,0,0,1,0);
	if (!menuselect)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	
	BASS_Start();
	
}

void SoundManager::play(int sound,int loop){
	switch(sound){
	case 0:
		BASS_SamplePlayEx(menumusic,(DWORD)0,-1,50,-1,loop);
		break;
	case 1:
		BASS_SamplePlayEx(menuselect,(DWORD)0,-1,50,-1,loop);
	break;



	default:
		break;
	}
}
void SoundManager::stop(int sound){

switch(sound){
	case 0:
		BASS_SampleStop(menumusic);
		break;

	default:
		break;
	}

}
SoundManager::~SoundManager(void)
{
	BASS_SampleStop(menumusic);
	
	BASS_Stop();
	std::cout << "Sound Stopped..." << std::endl;
	BASS_Free();
	std::cout << "Sound Memory Freed" << std::endl;
}
