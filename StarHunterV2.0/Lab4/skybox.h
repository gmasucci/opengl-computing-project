#pragma once
#include "loaders.h"
#include "Toolbox.h"

class skybox
{
public:
	skybox();
	void draw(GLGeometryTransform *pGLGT);
	~skybox();

private:
	GLFrame myFrame;
	char *filenames[6];
	GLBatch skyCube;
	GLenum  cube[6];
	GLint	locMVPSkyBox,skyboxShader;
	GLuint	skyTexture;

};

