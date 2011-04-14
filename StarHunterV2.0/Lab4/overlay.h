#pragma once
#include "Toolbox.h"
#include "loaders.h"

class overlay
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
	overlay();
	overlay(GLuint *texIN,int wd,int ht,int xIn, int yIn);
	void render();
	~overlay(void);

};
