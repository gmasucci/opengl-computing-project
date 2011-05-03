#pragma once
#include "Bass.h"
#include "Toolbox.h"
#define MUSIC_MENU 0
#define	FX_MENU_SELECT 1


class SoundManager
{
private:
	HSAMPLE menumusic;
	HSAMPLE menuselect;

	bool deathHSPLD;
	bool winHSPLD;

public:
	void play(int sound,int loop = 0);
	void stop(int sound);
	SoundManager(void);
	~SoundManager(void);
};

