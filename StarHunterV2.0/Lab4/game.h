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




class game{
private:

	soundManager		*sndMan;
	GLMatrixStack		projectionMatrix;
	GLFrustum			viewFrustum;
	GLMatrixStack		modelViewMatrix;
	GLGeometryTransform	transformPipeline;
	GLfloat				vLightPos[3];
	GLfloat				vWhite[4];
	GLuint				textures[15];
	GLuint				treetex[2];
	GLuint				house[4];
	CStopWatch			updater;
	Camera				*myCam;
	skybox				*mySky;
	objectManager		*mySM;
	GLuint				skyTex[6];
	terrain				*hm;
	player				*hayden;
	overlay				*loading;
	overlay				*loaded;
	overlay				*oHeart;
	overlay				*oStar;
	overlay				*oGameOver;
	overlay				*oWinner;
	numbers				*counters;
	HSAMPLE				music;
	HCHANNEL			musicChannel;
	Controls			input;
	Camerak				*theRealCam;
	int lastMouseX;
	int lastMouseY;
	int mouseX;
	int mouseY;

public:
	game(int argc,char*argv[]);
	void display();
	void init();
	void update();
	void updateOverlays();
	void reshape(int w,int h);
	void gameMain();
	void mouseMove(int x,int y);
	void keysUp(unsigned char key, int x, int y);
	void keysDn(unsigned char key, int x, int y);

	~game();

};