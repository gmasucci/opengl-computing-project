#pragma once

#include "Camera.h"
#include "md2model.h"
#include "skybox.h"
#include "objectManager.h"
#include "baddie.h"
#include "soundManager.h"
#include <StopWatch.h>
#include "Controls.h"
#include "Camerak.h"
#include "SGLVSync.h"


class Game{
private:

#ifdef GL_VERSION_3_0
	SGLVSync*vs;
#endif

	SoundManager		*sndMan;

	GLMatrixStack		projectionMatrix;
	GLFrustum			viewFrustum;
	GLMatrixStack		modelViewMatrix;
	GLGeometryTransform	transformPipeline;

	
	GLuint				textures[15];
	GLuint				treetex[2];
	GLuint				house[4];
	GLuint				stumpTex[2];
	GLuint				hedgeTex[3];

	CStopWatch			updater;
	Camera				*myCam;
	Skybox				*mySky;
	ObjectManager		*mySM;
	GLuint				skyTex[6];
	Terrain				*hm;
	Player				*hayden;

	Overlay				*loading;
	Overlay				*loaded;
	Overlay				*hud;
	Overlay				*eHint;
	Overlay				*startHelp;
	Overlay				*menu;
	Overlay				*pause;
	Overlay				*about;

	Numbers				*counters;
	HSAMPLE				music;
	HCHANNEL			musicChannel;
	Controls			input;
	Camerak				*theRealCam;
	int lastMouseX;
	int lastMouseY;
	int mouseX;
	int mouseY;
	bool grabMouse;
	unsigned int CURRENT_STATE,PREV_STATE;
	enum state {MENU=0,QUICKHELP,PLAYING,ABOUT,PAUSED};

public:
	Game(int argc,char*argv[]);
	void display();
	void init();
	void update();
	void reshape(int w,int h);
	void gameMain();
	void mouseMove(int x,int y)							{input.mouseMotion(x,y);}
	void keysUp(unsigned char key, int x, int y)		{input.keyboardRelease(key,x,y);}
	void keysDn(unsigned char key, int x, int y)		{input.keyboardPress(key,x,y);}
	void mouseClick(int button,int state,int x,int y)	{input.mouseClick(button,state,x,y);}
	~Game();

};