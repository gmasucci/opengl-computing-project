#include "soundManager.h"


SoundManager::SoundManager(void)
{
	if (!BASS_Init(-1, 44100, BASS_DEVICE_NOTHREAD, NULL))
	{
		MessageBox(NULL,"Couldn't initialize Bass.dll.","Error!",MB_OK|MB_ICONEXCLAMATION);
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
	
	m = ".\\Sounds\\gameambient.mp3\0";
	ambientGame = BASS_SampleLoad(false,m,0,0,1,0);
	if (!ambientGame)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	
	m = ".\\Sounds\\gameMusic.mp3\0";
	musicGame = BASS_SampleLoad(false,m,0,0,1,0);
	if (!musicGame)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);

	m = ".\\Sounds\\feet.mp3\0";
	gameFeet = BASS_SampleLoad(false,m,0,0,1,0);
	if (!gameFeet)
		MessageBox(NULL, "Couldn't load sound file.", m, MB_OK|MB_ICONEXCLAMATION);
	BASS_Start();
	
}

void SoundManager::play(int sound,int loop){
	switch(sound){
	case MUSIC_MENU:
		BASS_SamplePlayEx(menumusic,(DWORD)0,-1,50,-1,loop);
		break;
	case FX_MENU_SELECT:
		BASS_SamplePlayEx(menuselect,(DWORD)0,-1,50,-1,loop);
		break;
	case FX_GAME_AMBIENT:
		BASS_SamplePlayEx(ambientGame,(DWORD)0,-1,50,-1,loop);
		break;
	case MUSIC_GAME:
		BASS_SamplePlayEx(musicGame,(DWORD)0,-1,35,-1,loop);
		break;
	case FX_WALK:
		BASS_SamplePlayEx(gameFeet,(DWORD)0,-1,50,-1,loop);
		break;




	default:
		break;
	}
}
void SoundManager::stop(int sound){

switch(sound){
	case MUSIC_MENU:
		BASS_SampleStop(menumusic);
		break;
	case FX_MENU_SELECT:
		BASS_SampleStop(menuselect);
		break;
	case FX_GAME_AMBIENT:
		BASS_SampleStop(ambientGame);
		break;
	case MUSIC_GAME:
		BASS_SampleStop(musicGame);
		break;
	case FX_WALK:
		BASS_SampleStop(gameFeet);
		break;

	default:
		break;
	}

}
SoundManager::~SoundManager(void)
{
	BASS_Stop();
	std::cout << "All Sound Stopped" << std::endl;
	BASS_Free();
	std::cout << "Sound Memory Freed" << std::endl;
}
