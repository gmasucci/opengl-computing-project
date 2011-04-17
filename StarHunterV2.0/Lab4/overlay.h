#pragma once
#include "Toolbox.h"
#include "loaders.h"

class Overlay
{
protected:
	GLBatch rect;
	M3DMatrix44f screenspace;
	GLuint *tex;
	int width,height,xpos,ypos;
	GLint	myShader;
	GLint	locRectMVP;
	GLint	locRectTexture;
public:
	Overlay();
	Overlay(GLuint *texIN,int wd,int ht,int xIn, int yIn);
	void render();
	~Overlay(void);

};
