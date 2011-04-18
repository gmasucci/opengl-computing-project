#pragma once
#include "Toolbox.h"
#include "gameObject.h"
#include "loaders.h"
#include "player.h"
#include "terrain.h"

class Baddie : public GameObject
{
private:
	GLTriangleBatch *bodypart;
	int numParts;

public:
	Baddie();
	void render(GLMatrixStack *pMVM);
	Baddie(GLuint *tex,GLGeometryTransform *pGLGTin,Camera *camIn);
	void setup(GLuint *tex,GLGeometryTransform *pGLGTin);
	void renderMultiple(GLMatrixStack *pMVM,GLFrame &theFrame);
	~Baddie();
};

