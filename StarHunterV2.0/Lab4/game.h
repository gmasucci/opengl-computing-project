#pragma once

#include "Camera.h"
#include "md2model.h"
#include "skybox.h"
#include "objectManager.h"
#include "baddie.h"
#include "soundManager.h"
#include <StopWatch.h>





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

	GLint w;
	GLint s;
	GLint a;
	GLint d;
	GLint k;
	GLint l;
	GLint p;


public:
	game(int argc,char*argv[]);
	void display();
	void init();
	void update();
	void updateOverlays();
	void reshape(int w,int h);
	void gameMain();
	void keys(unsigned char key, int x, int y);
	void keysup(unsigned char key, int x, int y);
	~game();

};