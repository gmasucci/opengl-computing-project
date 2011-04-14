#pragma once
#include "Bass.h"
#include "Toolbox.h"
#define SM_FLOPPY 0
#define SM_MUSIC 1
#define SM_LF 2
#define SM_RF 3
#define SM_HEART 4
#define SM_PING 5
#define SM_PAIN 6
#define SM_DEAD 7
#define SM_WIN 8

class soundManager
{
private:
	HSAMPLE loading;
	HSAMPLE music;
	HSAMPLE footleft;
	HSAMPLE footright;
	HSAMPLE heart;
	HSAMPLE starPing;
	HSAMPLE pain;
	HSAMPLE death;
	HSAMPLE win;
	bool deathHSPLD;
	bool winHSPLD;

public:
	void play(int sound,int loop = 0);
	void stop(int sound);
	soundManager(void);
	~soundManager(void);
};

