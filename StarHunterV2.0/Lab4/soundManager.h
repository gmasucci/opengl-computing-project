#pragma once
#include "Bass.h"
#include "Toolbox.h"
#define MUSIC_MENU 0
#define	FX_MENU_SELECT 1
#define FX_GAME_AMBIENT 2
#define MUSIC_GAME 3
#define FX_WALK 4

class SoundManager
{
private:
	HSAMPLE menumusic;
	HSAMPLE menuselect;
	HSAMPLE ambientGame;
	HSAMPLE musicGame;
	HSAMPLE gameFeet;

	bool deathHSPLD;
	bool winHSPLD;

public:
	void play(int sound,int loop = 0);
	void stop(int sound);
	SoundManager(void);
	~SoundManager(void);
};

