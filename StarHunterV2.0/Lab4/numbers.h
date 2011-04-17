#pragma once
#include "overlay.h"
class Numbers :	public Overlay
{
private:
	GLBatch			*theNumbers;
	M3DMatrix44f	translate;
	float			x,y,z;

public:
	Numbers(GLuint *texIN);
	void render(int numberToRender,int x,int y);
	~Numbers(void);
};

