#pragma once
#include "overlay.h"
class numbers :	public overlay
{
private:
	GLBatch			*theNumbers;
	M3DMatrix44f	translate;
	float			x,y,z;

public:
	numbers(GLuint *texIN);
	void render(int numberToRender,int x,int y);
	~numbers(void);
};

