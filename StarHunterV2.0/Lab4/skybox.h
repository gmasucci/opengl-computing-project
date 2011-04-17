#pragma once
#include "loaders.h"
#include "Toolbox.h"

class Skybox
{
public:
	Skybox();
	void draw(GLGeometryTransform *pGLGT);
	~Skybox();

private:
	GLFrame myFrame;
	char *filenames[6];
	GLBatch skyCube;
	GLenum  cube[6];
	GLint	locMVPSkyBox,skyboxShader;
	GLuint	skyTexture;

};

