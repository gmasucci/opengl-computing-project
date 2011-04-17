#pragma once
#include "Toolbox.h"
#include "gameObject.h"
#include "loaders.h"
#include "player.h"
#include "terrain.h"

class baddie : public GameObject
{
private:
	GLTriangleBatch *bodypart;
	int numParts;

public:
	baddie();
	void render(GLMatrixStack *pMVM);
	baddie(GLuint *tex,GLGeometryTransform *pGLGTin,Camera *camIn);
	void setup(GLuint *tex,GLGeometryTransform *pGLGTin);
	void renderMultiple(GLMatrixStack *pMVM,GLFrame &theFrame);
	~baddie();
};

