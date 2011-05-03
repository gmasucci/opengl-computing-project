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




class Game{
private:

	SoundManager		*sndMan;
	GLMatrixStack		projectionMatrix;
	GLFrustum			viewFrustum;
	GLMatrixStack		modelViewMatrix;
	GLGeometryTransform	transformPipeline;
	GLfloat				vLightPos[3];
	GLfloat				vWhite[4];
	
	GLuint				textures[15];
	GLuint				treetex[2];
	GLuint				house[4];
	GLuint				stumpTex[2];

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
	unsigned int CURRENT_STATE;
	enum state {MENU=0,LOADING,QUICKHELP,PLAYING,ABOUT};

public:
	Game(int argc,char*argv[]);
	void display();
	void init();
	void update();
	void reshape(int w,int h);
	void gameMain();
	void mouseMove(int x,int y)						{input.mouseMotion(x,y);}
	void keysUp(unsigned char key, int x, int y)	{input.keyboardRelease(key,x,y);}
	void keysDn(unsigned char key, int x, int y)	{input.keyboardPress(key,x,y);}

	~Game();

};