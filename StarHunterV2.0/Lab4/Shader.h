#pragma once

#include "grid.h"
#include "loaders.h"
#include "Camera.h"

class Shader
{
private:
	GLuint *myTex;
	GLuint me;
	GLint locAmbient;
	GLint locDiffuse;
	GLint locSpecular;
	GLint locLight ;
	GLint locMVP;
	GLint locMV ;
	GLint locNM  ;
	GLint locTexture;
	GLfloat *vAmbC;
	GLfloat *vDifC;
	GLfloat *vSpecC;
	Camera	*theCam;
public:
	Shader(){}
	Shader(Camera *camIn, const char* vert,const char* frag,GLuint *tex = 0);
	void useTex(GLuint *texture);
	void bind(GLMatrixStack *pMVM, GLGeometryTransform *pGLGTin);

	~Shader(void);
};

