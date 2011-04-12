#pragma once
#include "gameObject.h"
class health :	public gameObject
{
public:
	health(void);
	health(GLuint *tex,GLGeometryTransform *pGLGTin,Camera *camIn);
	~health(void);
};

